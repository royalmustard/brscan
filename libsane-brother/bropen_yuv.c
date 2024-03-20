#include "bropen_yuv.h"

#include "bropen_ec.h"

#include "bropen_debug.h"

#include <stddef.h>
#include <string.h>

#define BROPEN_G_CHANNEL_BUFFER_SIZE 16384

static uint8_t g_y_buffer[BROPEN_G_CHANNEL_BUFFER_SIZE];
static uint8_t g_u_buffer[BROPEN_G_CHANNEL_BUFFER_SIZE];
static uint8_t g_v_buffer[BROPEN_G_CHANNEL_BUFFER_SIZE];
static uint16_t g_y_buffer_used = 0;
static uint16_t g_u_buffer_used = 0;
static uint16_t g_v_buffer_used = 0;
static uint8_t g_rgb_buffer[] = {0xFF, 0xFF, 0xFF};
static uint8_t g_rgb_buffer_index = 0xFF;
static uint32_t g_debug_added_total = 0;

static void move_buffer(uint8_t *buffer, uint16_t delete_size, uint16_t *buffer_used)
{
    *buffer_used -= delete_size;
    memmove(buffer, buffer + delete_size, *buffer_used);
}

static void clamp(float *f, float min, float max)
{
    if (*f < min)
    {
        *f = min;
    }
    else if (*f > max)
    {
        *f = max;
    }
}

static void yuv_to_rgb(uint8_t y, uint8_t u, uint8_t v, uint8_t rgb[3])
{
    float r = y + (1.370705 * (v - 128));
    float g = y - (0.698001 * (v - 128)) - (0.337633 * (u - 128));
    float b = y + (1.732446 * (u - 128));
    clamp(&r, 0.f, 255.f);
    clamp(&g, 0.f, 255.f);
    clamp(&b, 0.f, 255.f);
    rgb[0] = b * 220 / 256;
    rgb[1] = g * 220 / 256;
    rgb[2] = r * 220 / 256;
}

uint16_t bropen_yuv_rgb_data_available_count()
{
    uint16_t count = g_y_buffer_used > g_u_buffer_used ? g_u_buffer_used : g_y_buffer_used;
    count = count > g_v_buffer_used ? g_v_buffer_used : count;
    count *= 3;

    if (g_rgb_buffer_index != 0xFF)
    {
        count += 3 - g_rgb_buffer_index;
    }

    BROPEN_DEBUG_DEC(
        "done - [available: %u] [rgb index: %u] [y: %u] [u: %u] [v: %u]", count, g_rgb_buffer_index, g_y_buffer_used, g_u_buffer_used, g_v_buffer_used);

    return count;
}

bropen_ec_t bropen_yuv_channel_data_add(uint8_t channel, const uint8_t *data, uint16_t data_size, uint16_t *added_count)
{
    bropen_ec_t ec = BROPEN_EC_OK;

    uint8_t *buffer = NULL;
    uint16_t *used = NULL;

    BROPEN_DEBUG_DEC("start - [channel: %u] [size: %u] [data: %p] [r: %u] [g: %u] [b: %u] [total: %u]",
        channel, data_size, data, g_y_buffer_used, g_u_buffer_used, g_v_buffer_used, g_debug_added_total);

    *added_count = 0;

    switch (channel)
    {
        case 1:
            buffer = g_y_buffer;
            used = &g_y_buffer_used;
            break;
        case 0:
            buffer = g_u_buffer;
            used = &g_u_buffer_used;
            break;
        case 2:
            buffer = g_v_buffer;
            used = &g_v_buffer_used;
            break;
        default:
            ec = BROPEN_EC_INTERNAL_ERROR;
            break;
    }

    if (ec == BROPEN_EC_OK)
    {
        if (data_size < (BROPEN_G_CHANNEL_BUFFER_SIZE - (*used)))
        {
            memcpy(buffer + (*used), data, data_size);
            *used += data_size;
            *added_count = data_size;
        }
        else
        {
            ec = BROPEN_EC_BUFFER_SIZE;
        }
    }

    g_debug_added_total += *added_count;
    BROPEN_DEBUG_DEC(
        "done - [ec: %d] [added: %u] [r: %u] [g: %u] [b: %u] [total: %u]", ec, *added_count, g_y_buffer_used, g_u_buffer_used, g_v_buffer_used, g_debug_added_total);

    return ec;
}

void bropen_yuv_reset()
{
    g_y_buffer_used = 0;
    g_u_buffer_used = 0;
    g_v_buffer_used = 0;
    g_rgb_buffer[0] = 0xFF;
    g_rgb_buffer[1] = 0xFF;
    g_rgb_buffer[2] = 0xFF;
    g_rgb_buffer_index = 0xFF;
    g_debug_added_total = 0;
}

bropen_ec_t bropen_yuv_rgb_data_write(uint8_t *buffer_out, uint16_t buffer_size, uint16_t *wrote_count)
{
    bropen_ec_t ec = BROPEN_EC_OK;
    uint16_t buffer_size_left = buffer_size;

    BROPEN_DEBUG_DEC("start - [buffer size: %u] [rgb buffer index: %u] [buffer out: %p] [wrote: %u]",
        buffer_size, g_rgb_buffer_index, buffer_out, *wrote_count);

    *wrote_count = 0;

    // write buffered (r)gb
    {
        if ((g_rgb_buffer_index == 1) && (buffer_size_left > 0))
        {
            buffer_out[0] = g_rgb_buffer[1];
            buffer_out += 1;
            buffer_size_left -= 1;
            g_rgb_buffer_index = 2;
        }

        if ((g_rgb_buffer_index == 2) && (buffer_size_left > 0))
        {
            buffer_out[0] = g_rgb_buffer[2];
            buffer_out += 1;
            buffer_size_left -= 1;
            g_rgb_buffer_index = 0xFF;
        }
    }

    // write data by chunks of 3
    uint16_t yuv_buffer_index = 0;
    if ((ec == BROPEN_EC_OK) && (buffer_size_left > 0))
    {
        const uint16_t available = bropen_yuv_rgb_data_available_count();
        const uint16_t copy_size = available > buffer_size_left ? buffer_size_left : available;
        const uint16_t chunks_count = copy_size / 3;

        if (g_y_buffer_used < chunks_count)
        {
            ec = BROPEN_EC_INTERNAL_ERROR;
        }
        else if (g_u_buffer_used < chunks_count)
        {
            ec = BROPEN_EC_INTERNAL_ERROR;
        }
        else if (g_v_buffer_used < chunks_count)
        {
            ec = BROPEN_EC_INTERNAL_ERROR;
        }

        if (ec == BROPEN_EC_OK)
        {
            for (; yuv_buffer_index < chunks_count; ++yuv_buffer_index)
            {
                yuv_to_rgb(g_y_buffer[yuv_buffer_index], g_u_buffer[yuv_buffer_index], g_v_buffer[yuv_buffer_index], buffer_out);
                buffer_out += 3;
            }
            buffer_size_left -= (3 * chunks_count);
        }
    }

    // copy data that were outside of chunks
    if ((buffer_size_left > 0) && ((yuv_buffer_index < g_y_buffer_used) && (yuv_buffer_index < g_u_buffer_used) && (yuv_buffer_index < g_v_buffer_used)))
    {
        yuv_to_rgb(g_y_buffer[yuv_buffer_index], g_u_buffer[yuv_buffer_index], g_v_buffer[yuv_buffer_index], g_rgb_buffer);
        buffer_out[0] = g_rgb_buffer[0];
        buffer_out++;
        buffer_size_left -= 1;
        g_rgb_buffer_index = 1;

        if (buffer_size_left > 0)
        {
            buffer_out[0] = g_rgb_buffer[1];
            buffer_out++;
            buffer_size_left -= 1;
            g_rgb_buffer_index = 2;

            if (buffer_size_left > 0)
            {
                ec = BROPEN_EC_INTERNAL_ERROR;
            }
        }

        yuv_buffer_index += 1;
    }

    move_buffer(g_y_buffer, yuv_buffer_index, &g_y_buffer_used);
    move_buffer(g_u_buffer, yuv_buffer_index, &g_u_buffer_used);
    move_buffer(g_v_buffer, yuv_buffer_index, &g_v_buffer_used);

    {
        const uint16_t available = bropen_yuv_rgb_data_available_count();
        if ((buffer_size_left > 0) && (available > 0))
        {
            ec = BROPEN_EC_INTERNAL_ERROR;
        }

        *wrote_count = buffer_size - buffer_size_left;

        BROPEN_DEBUG_DEC("done - [ec: %d] [wrote count: %u] [rgb buffer index: %u] [available: %u] [buffer out: %p]",
            ec, *wrote_count, g_rgb_buffer_index, available, buffer_out);
    }

    return ec;
}
