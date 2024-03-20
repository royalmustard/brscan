#include "bropen_scan.h"

#include "bropen_com.h"
#include "bropen_ec.h"
#include "bropen_types.h"
#include "bropen_yuv.h"

#include "bropen_debug.h"

#include <stddef.h>
#include <string.h>

#define USB_CHUNK_SIZE 16384 // watchout changing this value, libusb 0.1 seems to mess up buffer if not power of 2
#define G_BUFFER_SIZE (USB_CHUNK_SIZE * 2)
static uint8_t g_persistent_buffer[G_BUFFER_SIZE];
static uint16_t g_persistent_buffer_use = 0;
static uint16_t g_next_header_offset = 0;
static uint8_t g_yuv_write_index = 0xFF;

static bropen_ec_t decode_header(uint8_t *buffer, uint16_t buffer_size, uint16_t *consumed)
{
    bropen_ec_t ec = BROPEN_EC_UNSET;

    const uint8_t header_tag = *buffer;
    BROPEN_DEBUG_SCAN("start - [header: 0x%2.2X] [size: %u] [consumed: %u] [next header offset: %u] [yuv index: %u]",
        header_tag, buffer_size, *consumed, g_next_header_offset, g_yuv_write_index);

    *consumed = 0;

    switch (header_tag)
    {
        case 0x44:
        case 0x48:
        case 0x4C:
        {
            g_yuv_write_index = (header_tag - 0x44) / 4;
            if (buffer_size >= 3)
            {
                const uint16_t info_size = *(uint16_t *)(buffer + 1);
                if (buffer_size >= (1 + 2 + info_size + 2))
                {
                    g_next_header_offset = *(uint16_t *)(buffer + 1 + 2 + info_size);
                    // TODO add check that the size is the scanning area width
                    *consumed = 1 + 2 + info_size + 2;
                    ec = BROPEN_EC_OK;
                }
                else
                {
                    ec = BROPEN_EC_DATA_UNSUFFICIENT;
                }
            }
            else
            {
                ec = BROPEN_EC_DATA_UNSUFFICIENT;
            }
        }
        break;
        case 0x64: // JFIF
            ec = BROPEN_EC_DATA_UNSUPPORTED_FORMAT;
            break;
        case 0x82: // TODO can we really skip that one?
            if (buffer_size >= 3)
            {
                const uint16_t info_size = *(uint16_t *)(buffer + 1);
                if (buffer_size >= (1 + 2 + info_size))
                {
                    *consumed = 1 + 2 + info_size;
                    ec = BROPEN_EC_OK;
                }
                else
                {
                    ec = BROPEN_EC_DATA_UNSUFFICIENT;
                }
            }
            else
            {
                ec = BROPEN_EC_DATA_UNSUFFICIENT;
            }
            break;
        case 0x80:
            ec = BROPEN_EC_DATA_EOF;
            *consumed = 1;
            break;
        default:
            ec = BROPEN_EC_DATA_UNSUPPORTED_HEADER;
            BROPEN_PRINT_INTERNAL("unexpected data header: 0x%2.2X", header_tag);
            break;
    }

    BROPEN_DEBUG_SCAN("done - [ec: %d] [header: 0x%2.2X] [consumed: %u] [next header offset: %u] [yuv index: %u]",
        ec, header_tag, *consumed, g_next_header_offset, g_yuv_write_index);

    return ec;
}

static bropen_ec_t decode_buffer(uint8_t *buffer_in, uint16_t buffer_in_size, uint16_t *buffer_in_consumed)
{
    bropen_ec_t ec = BROPEN_EC_OK;

    BROPEN_DEBUG_SCAN("start - [buffer in: %p] [in size: %u] [in consumed: %u] [ec: %d]", buffer_in, buffer_in_size, *buffer_in_consumed, ec);

    *buffer_in_consumed = 0;

    // decode data in
    while ((ec == BROPEN_EC_OK) && (buffer_in_size > 0))
    {
        BROPEN_DEBUG_SCAN("loop iter start - [ec: %d] [size to decode: %u] [next header offset: %u] [yuv channel index: %u] [size consumed: %u]",
            ec, buffer_in_size, g_next_header_offset, g_yuv_write_index, *buffer_in_consumed);

        if (g_next_header_offset > 0)
        {
            const uint16_t available_data_size = g_next_header_offset > buffer_in_size ? buffer_in_size : g_next_header_offset;
            uint16_t added = 0;
            ec = bropen_yuv_channel_data_add(g_yuv_write_index, buffer_in, available_data_size, &added);
            buffer_in += added;
            buffer_in_size -= added;
            *buffer_in_consumed += added;
            g_next_header_offset -= added;
        }
        else
        {
            uint16_t consumed = 0;
            ec = decode_header(buffer_in, buffer_in_size, &consumed);
            buffer_in += consumed;
            buffer_in_size -= consumed;
            *buffer_in_consumed += consumed;

            if (ec == BROPEN_EC_OK && consumed == 0)
            {
                ec = BROPEN_EC_INTERNAL_ERROR;
            }
        }

        BROPEN_DEBUG_SCAN("loop iter done  - [ec: %d] [size to decode: %u] [next header offset: %u] [yuv channel index: %u] [size consumed: %u]",
            ec, buffer_in_size, g_next_header_offset, g_yuv_write_index, *buffer_in_consumed);
    }

    BROPEN_DEBUG_SCAN("done - [buffer in: %p] [in size: %u] [in consumed: %u] [ec: %d]", buffer_in, buffer_in_size, *buffer_in_consumed, ec);

    return ec;
}

static bropen_ec_t copy_available(uint8_t *buffer_out, uint16_t buffer_out_size, uint16_t *buffer_out_wrote)
{
    bropen_ec_t ec = BROPEN_EC_OK;

    const uint16_t available = bropen_yuv_rgb_data_available_count();
    *buffer_out_wrote = 0;

    BROPEN_DEBUG_SCAN("start - [ec: %d] [buffer: %p] [size: %u] [wrote: %u] [available: %u]", ec, buffer_out, buffer_out_size, *buffer_out_wrote, available);

    if (available > 0)
    {
        const uint16_t copy_size = available > buffer_out_size ? buffer_out_size : available;
        ec = bropen_yuv_rgb_data_write(buffer_out, copy_size, buffer_out_wrote);
    }

    BROPEN_DEBUG_SCAN("end   - [ec: %d] [buffer: %p] [size: %u] [wrote: %u] [available: %u]",
        ec, buffer_out, buffer_out_size, *buffer_out_wrote, bropen_yuv_rgb_data_available_count());

    return ec;
}

bropen_ec_t bropen_scan_data(uint16_t *buffer_out_wrote, uint8_t *buffer_out, uint16_t buffer_out_size, bropen_com_params_t *com_params)
{
    bropen_ec_t ec = BROPEN_EC_OK;

    BROPEN_DEBUG_SCAN("start - [size: %u] [wrote: %u] [g_persistent_buffer_use: %u]",
        buffer_out_size, *buffer_out_wrote, g_persistent_buffer_use);

    *buffer_out_wrote = 0;

    ec = copy_available(buffer_out, buffer_out_size, buffer_out_wrote);
    if (ec == BROPEN_EC_OK)
    {
        buffer_out += *buffer_out_wrote;
        buffer_out_size -= *buffer_out_wrote;
    }

    // need fesh data?
    while ((ec == BROPEN_EC_OK) && (buffer_out_size > 0))
    {
        uint8_t local_buffer[USB_CHUNK_SIZE];
        uint16_t received = 0;

        BROPEN_DEBUG_SCAN("loop iter start - [ec: %d] [wrote: %u] [size left: %u] [persistent buffer use: %u]",
            ec, *buffer_out_wrote, buffer_out_size, g_persistent_buffer_use);

        ec = bropen_com_receive(&received, local_buffer, sizeof local_buffer, com_params);
        BROPEN_DEBUG_SCAN("received fresh data - [ec: %d] [size: %u]", ec, received);
        BROPEN_DEBUG_SCAN_BUFFER("received", local_buffer, received, 32);
        if (received > 0)
        {
            uint8_t *buffer_to_decode = NULL;
            uint16_t buffer_to_decode_size = 0;
            uint16_t consumed = 0;
            bropen_ec_t decode_ec = BROPEN_EC_UNSET;

            if (g_persistent_buffer_use > 0)
            {
                // there's still data from previous read
                // TODO check overflow?
                memcpy(g_persistent_buffer + g_persistent_buffer_use, local_buffer, received);
                g_persistent_buffer_use += received;
                buffer_to_decode = g_persistent_buffer;
                buffer_to_decode_size = g_persistent_buffer_use;
            }
            else
            {
                // no previous data, just use the local buffer to decode
                buffer_to_decode = local_buffer;
                buffer_to_decode_size = received;
            }
            decode_ec = decode_buffer(buffer_to_decode, buffer_to_decode_size, &consumed);
            if (consumed > 0)
            {
                buffer_to_decode += consumed;
                buffer_to_decode_size -= consumed;
                g_persistent_buffer_use = buffer_to_decode_size;
                memmove(g_persistent_buffer, buffer_to_decode, g_persistent_buffer_use);
            }
            if (ec == BROPEN_EC_OK && decode_ec != BROPEN_EC_OK)
            {
                ec = decode_ec;
            }
            if (ec == BROPEN_EC_OK)
            {
                uint16_t wrote = 0;
                ec = copy_available(buffer_out, buffer_out_size, &wrote);
                if (wrote > 0)
                {
                    buffer_out += wrote;
                    buffer_out_size -= wrote;
                    *buffer_out_wrote += wrote;
                }
            }

        }
        else
        {
            ec = BROPEN_EC_COM_ERROR;
        }

        BROPEN_DEBUG_SCAN("loop iter done  - [ec: %d] [wrote: %u] [size left: %u] [persistent buffer use: %u]",
            ec, *buffer_out_wrote, buffer_out_size, g_persistent_buffer_use);
    }

    BROPEN_DEBUG_SCAN("done - [ec: %d] [wrote: %u] [size left: %u] [g_persistent_buffer_use: %u]",
        ec, *buffer_out_wrote, buffer_out_size, g_persistent_buffer_use);

    return ec;
}

void bropen_scan_reset()
{
    bropen_yuv_reset();

    g_persistent_buffer_use = 0;
    g_next_header_offset = 0;
    g_yuv_write_index = 0xFF;
}