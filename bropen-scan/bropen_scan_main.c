#include <stdio.h>

#include <bropen_ec.h>
#include <bropen_types.h>
#include <bropen_yuv.h>

#include <bropen_debug.h>

#include <libusb-1.0/libusb.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h> //usleep

#define RCV_BUFFER_SIZE 16384
#define RCV_TIMEOUT_DEFAULT 5000
#define SEND_TIMEOUT_DEFAULT 1000

#define PROGRAME_NAME "bropen-scan"

static uint16_t PRODUCT_ID_INVALID = 0xFFFF;

static const char FORMAT_OPTION[] = "format=";
static const char HELP_OPTION[] = "help";
static const char OVERWRITE_OPTION[] = "force";
static const char PRODUCT_OPTION[] = "product=";

typedef struct bropen_usb_config_t
{
    uint8_t dth; // device to host
    uint8_t htd; // host to device
    uint8_t interface;
} bropen_usb_config_t;

static void bropen_usb_config_init(bropen_usb_config_t *uc)
{
    uc->dth = 0xFF;
    uc->htd = 0xFF;
    uc->interface = 0xFF;
}

static void bropen_usb_config_end(bropen_usb_config_t *uc)
{
    uc->dth = 0xFF;
    uc->htd = 0xFF;
    uc->interface = 0xFF;
}

typedef struct bropen_usb_device_t
{
    libusb_device *libusb_device;
    libusb_device_handle *libusb_handle;
    bropen_usb_config_t usb_config;
} bropen_usb_device_t;

static void bropen_usb_device_init(bropen_usb_device_t *d)
{
    d->libusb_device = NULL;
    d->libusb_handle = NULL;
    bropen_usb_config_init(&d->usb_config);
}

static void bropen_usb_device_end(bropen_usb_device_t *d)
{
    bropen_usb_config_end(&d->usb_config);
    d->libusb_handle = NULL;
    d->libusb_device = NULL;
}

typedef enum bropen_scan_format_t
{
    BROPEN_SCAN_FORMAT_BMP,
    BROPEN_SCAN_FORMAT_JPEG,
} bropen_scan_format_t;

typedef struct bropen_user_options_t
{
    uint16_t product;
    bropen_scan_format_t format;
    const char *out_path;
    uint8_t out_overwrite;
    uint8_t help;
} bropen_user_options_t;

static void bropen_user_options_init(bropen_user_options_t *uo)
{
    uo->product = PRODUCT_ID_INVALID;
    uo->format = BROPEN_SCAN_FORMAT_JPEG;
    uo->out_path = NULL;
    uo->out_overwrite = 0;
    uo->help = 0;
}

static void bropen_user_options_end(bropen_user_options_t *uo)
{
    uo->help = 0;
    uo->out_overwrite = 0;
    uo->out_path = NULL;
    uo->format = BROPEN_SCAN_FORMAT_JPEG;
    uo->product = PRODUCT_ID_INVALID;
}

typedef struct bropen_buffer_helper_t
{
    uint8_t *data;
    uint32_t capacity;
    uint32_t used;
} bropen_buffer_helper_t;

static bropen_ec_t bropen_buffer_helper_init(bropen_buffer_helper_t *buffer_helper, uint32_t size)
{
    bropen_ec_t ec = BROPEN_EC_KO;

    buffer_helper->used = 0;
    buffer_helper->data = malloc(size);
    if (buffer_helper->data != NULL)
    {
        buffer_helper->capacity = size;
        ec = BROPEN_EC_OK;
    }
    else
    {
        buffer_helper->capacity = 0;
        ec = BROPEN_EC_MEMORY;
    }

    return ec;
}

static void bropen_buffer_helper_end(bropen_buffer_helper_t *buffer_helper)
{
    if (buffer_helper->data != NULL)
    {
        free(buffer_helper->data);
        buffer_helper->data = NULL;
    }
    buffer_helper->capacity = 0;
    buffer_helper->used = 0;
}

typedef struct bropen_yuv_data_helper_t
{
    uint16_t channel_size_left;
    uint8_t channel_index;
} bropen_yuv_data_helper_t;

static void bropen_yuv_data_helper_init(bropen_yuv_data_helper_t *yuv_data_helper)
{
    yuv_data_helper->channel_size_left = 0;
    yuv_data_helper->channel_index = 0xFF;
}

static void bropen_yuv_data_helper_end(bropen_yuv_data_helper_t *yuv_data_helper)
{
    yuv_data_helper->channel_size_left = 0;
    yuv_data_helper->channel_index = 0xFF;
}

typedef struct bropen_jpeg_data_helper_t
{
    uint16_t chunk_size_left;
} bropen_jpeg_data_helper_t;

static void bropen_jpeg_data_helper_init(bropen_jpeg_data_helper_t *jpeg_data_helper)
{
    jpeg_data_helper->chunk_size_left = 0;
}

static void bropen_jpeg_data_helper_end(bropen_jpeg_data_helper_t *jpeg_data_helper)
{
    jpeg_data_helper->chunk_size_left = 0;
}

static bropen_ec_t helper_bmp_write(FILE *out, const bropen_scanpage_info_t *scanpage_info, bropen_buffer_helper_t *buffer_helper)
{
    bropen_ec_t ec = BROPEN_EC_KO;

    const uint32_t bytes_per_line = scanpage_info->width * 3;
    const uint32_t lines_count = buffer_helper->used / bytes_per_line;
    const uint32_t lines_garbage = buffer_helper->used % bytes_per_line;
    const uint32_t bytes_count_expected = bytes_per_line * scanpage_info->height;

    BROPEN_DEBUG_SCAN("start - [width: %u] [height: %u] [buffer size: %u] [buffer capacity: %u] [bytes per line: %u] [lines: %u] [garbage: %u] [size expected: %u]",
        scanpage_info->width, scanpage_info->height, buffer_helper->used, buffer_helper->capacity, bytes_per_line, lines_count, lines_garbage, bytes_count_expected);

    if (lines_garbage == 0 && lines_count <= scanpage_info->height)
    {
        const uint32_t line_padding = 0;
        const uint8_t line_padding_size = bytes_per_line % 4;

        const uint16_t file_header_size = 14;
        const uint32_t dib_header_size = 40;
        const int32_t file_width = scanpage_info->width;
        const int32_t file_height = lines_count;
        const uint16_t color_planes = 1;
        const uint16_t bits_per_pixel = 24;
        const uint32_t compression_used = 0;
        const uint32_t image_size = buffer_helper->used;
        const int32_t h_res = scanpage_info->width * 39.370079;
        const int32_t v_res = scanpage_info->height * 39.370079;
        const uint32_t colors = 0;
        {
            // file header
            uint32_t bmp_size = file_header_size + dib_header_size + image_size;
            uint32_t data_offset = file_header_size + dib_header_size;
            const uint16_t reserved = 0;
            fwrite("BM", 2, 1, out);
            fwrite(&bmp_size, 4, 1, out);
            fwrite(&reserved, 2, 1, out);
            fwrite(&reserved, 2, 1, out);
            fwrite(&data_offset, 4, 1, out);

            // dib header
            fwrite(&dib_header_size, 4, 1, out);
            fwrite(&file_width, 4, 1, out);
            fwrite(&file_height, 4, 1, out);
            fwrite(&color_planes, 2, 1, out);
            fwrite(&bits_per_pixel, 2, 1, out);
            fwrite(&compression_used, 4, 1, out);
            fwrite(&image_size, 4, 1, out);
            fwrite(&h_res, 4, 1, out);
            fwrite(&v_res, 4, 1, out);
            fwrite(&colors, 4, 1, out);
            fwrite(&colors, 4, 1, out);
        }

        // rgb => bgr
        for (uint32_t i = 0; i < buffer_helper->used; i += 3)
        {
            uint8_t tmp = buffer_helper->data[i];
            buffer_helper->data[i] = buffer_helper->data[i + 2];
            buffer_helper->data[i + 2] = tmp;
        }

        ec = BROPEN_EC_OK;
        for (uint32_t to_write_end = buffer_helper->used; to_write_end > 0;)
        {
            size_t local_w = fwrite(buffer_helper->data + (to_write_end - bytes_per_line), 1, bytes_per_line, out);
            if (local_w == bytes_per_line)
            {
                to_write_end -= bytes_per_line;
                if (line_padding_size > 0)
                {
                    local_w = fwrite(&line_padding, 1, line_padding_size, out);
                    if (local_w != line_padding_size)
                    {
                        ec = BROPEN_EC_KO;
                        break;
                    }
                }
            }
            else
            {
                ec = BROPEN_EC_KO;
                break;
            }
        }
    }

    BROPEN_DEBUG_SCAN("done - [ec: %u]", ec);

    return ec;
}

static bropen_ec_t helper_buffer_insert_str(uint8_t *buffer, uint16_t size, uint16_t *pos, const char *to_append)
{
    bropen_ec_t ec = BROPEN_EC_OK;
    const size_t size_to_append = strlen(to_append); // TODO pass it as argument? size is known at compile time

    if (*pos + size_to_append <= size)
    {
        memcpy(buffer + *pos, to_append, size_to_append);
        *pos += size_to_append;
    }
    else
    {
        ec = BROPEN_EC_BUFFER_SIZE;
    }

    return ec;
}

static bropen_ec_t helper_buffer_insert_u16(uint8_t *buffer, uint16_t size, uint16_t *pos, uint16_t to_append)
{
    bropen_ec_t ec = BROPEN_EC_KO;

    char to_append_buffer[512];
    int count = snprintf(to_append_buffer, sizeof to_append_buffer, "%u", to_append);
    if (count < (int) sizeof to_append_buffer)
    {
        ec = helper_buffer_insert_str(buffer, size, pos, to_append_buffer);
    }

    return ec;
}

static bropen_ec_t helper_destination_open(FILE **dst, const bropen_user_options_t *user_options)
{
    bropen_ec_t ec = BROPEN_EC_OK;

    *dst = NULL;

    if (user_options->out_path == NULL)
    {
        ec = BROPEN_EC_KO;
        BROPEN_PRINT_USER_ERROR("no output file provided");
    }

    if (ec == BROPEN_EC_OK && user_options->out_overwrite == 0)
    {
        FILE *f = fopen(user_options->out_path, "r");
        if (f != NULL)
        {
            ec = BROPEN_EC_KO;
            BROPEN_PRINT_USER_ERROR("output file [%s] already exists", user_options->out_path);
            fclose(f);
        }
    }

    if (ec == BROPEN_EC_OK)
    {
        *dst = fopen(user_options->out_path, "w");
        if (*dst == NULL)
        {
            ec = BROPEN_EC_KO;
            BROPEN_PRINT_USER_ERROR("failed to open output file [%s]", user_options->out_path);
        }
    }

    return ec;
}

static int helper_error_code_shell(bropen_ec_t bropen_ec, uint8_t silent)
{
    int shell_ec = -1;

    switch (bropen_ec)
    {
        case BROPEN_EC_OK:
        case BROPEN_EC_DATA_EOF:
            shell_ec = 0;
            if (!silent)
            {
                BROPEN_PRINT_USER_INFO("success");
            }
            break;
        default:
            shell_ec = bropen_ec;
            BROPEN_PRINT_USER_ERROR("scan failed [error code: %d]", bropen_ec);
            break;
    }

    return shell_ec;
}

static void helpert_usage_print()
{
    printf("Overview: Brother scanner USB scan tool\n\n");
    printf("Usage: %s [options] output\n", PROGRAME_NAME);
    printf("Options:\n");
    printf("  --%s\t\toverwrite output if it already exists\n", OVERWRITE_OPTION);
    printf("  --%sbmp|jpeg\tselect scan mode and output file format\n", FORMAT_OPTION);
    printf("  --%s\t\tprint this message\n", HELP_OPTION);
    printf("  --%sid\t\tspecify scanner usb product id (hexadecimal), e.g.: --%s03b5\n", PRODUCT_OPTION, PRODUCT_OPTION);
    printf("\n");
}

static void helper_shift_array(const char *array[], uint8_t index, uint8_t size)
{
    const char *tmp = array[index];

    for (; index < size - 1; ++index)
    {
        array[index] = array[index + 1];
    }

    array[size - 1] = tmp;
}

static bropen_ec_t helper_parse_hex2(uint16_t *out, const char *str)
{
    bropen_ec_t ec = BROPEN_EC_KO;

    char *end = NULL;
    unsigned long ul = strtoul(str, &end, 16);
    if (end && *end == '\0' && ul <= UINT16_MAX)
    {
        *out = ul;
        ec = BROPEN_EC_OK;
    }

    return ec;
}

static bropen_ec_t helper_parse_format(bropen_scan_format_t *format, const char *str)
{
    bropen_ec_t ec = BROPEN_EC_OK;

    if (strcmp(str, "bmp") == 0)
    {
        *format = BROPEN_SCAN_FORMAT_BMP;
    }
    else if (strcmp(str, "jpeg") == 0)
    {
        *format = BROPEN_SCAN_FORMAT_JPEG;
    }
    else
    {
        ec = BROPEN_EC_KO;
    }

    return ec;
}

static bropen_ec_t helper_parse_u16(uint16_t *value_out, uint16_t *buffer_consumed, const uint8_t *buffer, uint16_t buffer_size)
{
    bropen_ec_t ec = BROPEN_EC_KO;
    uint16_t separator_pos = 0;

    *buffer_consumed = 0;

    for (; separator_pos < buffer_size; ++separator_pos)
    {
        if (buffer[separator_pos] == ',')
        {
            break;
        }
    }
    if (separator_pos < buffer_size && separator_pos > 0)
    {
        uint16_t mult = 1;
        *value_out = 0;
        ec = BROPEN_EC_OK;

        for (uint16_t i = separator_pos; i > 0 && ec == BROPEN_EC_OK; --i)
        {
            const char c = buffer[i - 1];
            if (c >= '0' && c <= '9')
            {
                *value_out += (c - '0') * mult;
                mult *= 10;
            }
            else
            {
                ec = BROPEN_EC_KO;
            }
        }

        if (ec == BROPEN_EC_OK)
        {
            *buffer_consumed = separator_pos + 1;
        }
    }

    return ec;
}

static bropen_ec_t helper_parse_user_options(bropen_user_options_t *user_options, int *argc, const char *argv[])
{
    bropen_ec_t ec = BROPEN_EC_OK;

    for (int i = 1; i < *argc && ec == BROPEN_EC_OK;)
    {
        const char *arg = argv[i];
        if (arg[0] != '-' || arg[1] != '-')
        {
            i += 1;
        }
        else
        {
            if (strcmp(&arg[2], HELP_OPTION) == 0)
            {
                user_options->help = 1;
            }
            else if (strcmp(&arg[2], OVERWRITE_OPTION) == 0)
            {
                user_options->out_overwrite = 1;
            }
            else if (strncmp(&arg[2], FORMAT_OPTION, sizeof FORMAT_OPTION - 1) == 0)
            {
                ec = helper_parse_format(&user_options->format, &arg[2 + sizeof FORMAT_OPTION - 1]);
                if (ec != BROPEN_EC_OK)
                {
                    BROPEN_PRINT_USER_ERROR("unsuppoted format [%s]", &arg[2 + sizeof FORMAT_OPTION - 1]);
                }
            }
            else if (strncmp(&arg[2], PRODUCT_OPTION, sizeof FORMAT_OPTION - 1) == 0)
            {
                ec = helper_parse_hex2(&user_options->product, &arg[2 + sizeof PRODUCT_OPTION - 1]);
                if (ec != BROPEN_EC_OK)
                {
                    BROPEN_PRINT_USER_ERROR("invalid product id [%s], 4 hex digit expected (e.g. )", &arg[2 + sizeof PRODUCT_OPTION - 1]);
                }
            }
            else
            {
                ec = BROPEN_EC_KO;
                BROPEN_PRINT_USER_ERROR("invalid options [%s]", arg);
            }

            if (ec == BROPEN_EC_OK)
            {
                helper_shift_array(argv, i, *argc);
                *argc -= 1;
            }
        }
    }

    return ec;
}

static bropen_ec_t usb_com_rcv(bropen_usb_device_t *usb_device, uint8_t *data, uint16_t size, uint16_t *transfered, uint32_t timeout)
{
    BROPEN_DEBUG_COM("start - [size: %u] [endpoint: 0x%2.2X] [timeout: %d]", size, usb_device->usb_config.dth, timeout);

    *transfered = 0;

    {
        int libusb_rv = 0;
        uint32_t try_max = timeout / 100 + (timeout % 100 > 0 ? 1 : 0);
        uint32_t try_count = 0;
        int local_transfered = 0;

        libusb_claim_interface(usb_device->libusb_handle, usb_device->usb_config.interface);

        do
        {
            if (try_count++ > 0)
            {
                // sleep
                usleep(100000);
            }
            libusb_rv = libusb_bulk_transfer(usb_device->libusb_handle, usb_device->usb_config.dth, data, size, &local_transfered, 10);
        } while (libusb_rv == 0 && local_transfered == 0 && try_count < try_max);

        libusb_release_interface(usb_device->libusb_handle, usb_device->usb_config.interface);

        BROPEN_DEBUG_COM("done - [received: %d] [ec: %d] [try: %d]", local_transfered, libusb_rv, try_count);
        if (local_transfered > 0)
        {
            *transfered = local_transfered;
        }

        return *transfered > 0 ? BROPEN_EC_OK : BROPEN_EC_KO;
    }
}

static bropen_ec_t usb_com_send(bropen_usb_device_t *usb_device, uint8_t *data, uint16_t size, const char *id)
{
    BROPEN_DEBUG_COM("start - sending [%s] [size: %u] [endpoint: 0x%2.2X]", id, size, usb_device->usb_config.htd);
    BROPEN_DEBUG_COM_BUFFER(id, data, size, size);

    {
        int transfered = 0;
        int libusb_rv = 0;

        libusb_claim_interface(usb_device->libusb_handle, usb_device->usb_config.interface);
        libusb_rv = libusb_bulk_transfer(usb_device->libusb_handle, usb_device->usb_config.htd, data, size, &transfered, SEND_TIMEOUT_DEFAULT);
        libusb_release_interface(usb_device->libusb_handle, usb_device->usb_config.interface);

        BROPEN_DEBUG_COM("done - [sent: %d] [ec: %d] [id: %s]", transfered, libusb_rv, id);

        if (size != transfered)
        {
            BROPEN_PRINT_ERROR("failed to send command [%s]", id);
        }

        return transfered == size ? BROPEN_EC_OK : BROPEN_EC_KO;
    }
}

static bropen_ec_t usb_conf_interface_find_endpoints(bropen_usb_config_t *config_out, const struct libusb_interface_descriptor *desc)
{
    bropen_ec_t ec = BROPEN_EC_UNSET;

    uint8_t dth_endpoint_tmp = 0xFF;
    uint8_t htd_endpoint_tmp = 0xFF;
    uint8_t interface_tmp = 0;

    BROPEN_DEBUG_USB("start - [interface: %u] [endpoints: %u] [dth: 0x%2.2X (%u)] [htd: 0x%2.2X (%u)]",
        desc->bInterfaceClass, desc->bNumEndpoints, config_out->dth, config_out->dth, config_out->htd, config_out->htd);

    if ((desc->bInterfaceClass == LIBUSB_CLASS_VENDOR_SPEC) && (desc->bNumEndpoints >= 2))
    {
        interface_tmp = desc->bInterfaceNumber;
        for (int i = 0; i < desc->bNumEndpoints && ec == BROPEN_EC_UNSET; ++i)
        {
            const struct libusb_endpoint_descriptor *endpoint = &desc->endpoint[i];
            if ((endpoint->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK) == LIBUSB_ENDPOINT_TRANSFER_TYPE_BULK)
            {
                switch (endpoint->bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK)
                {
                    case LIBUSB_ENDPOINT_IN:
                    {
                        if (dth_endpoint_tmp == 0xFF)
                        {
                            dth_endpoint_tmp = endpoint->bEndpointAddress;
                        }
                        else
                        {
                            ec = BROPEN_EC_USB_ERROR;
                        }
                    }
                    break;
                    case LIBUSB_ENDPOINT_OUT:
                    {
                        if (htd_endpoint_tmp == 0xFF)
                        {
                            htd_endpoint_tmp = endpoint->bEndpointAddress;
                        }
                        else
                        {
                            ec = BROPEN_EC_USB_ERROR;
                        }
                    }
                    break;
                    default:
                        break;
                }
            }
        }
    }

    if ((ec == BROPEN_EC_UNSET) && (htd_endpoint_tmp != 0xFF) && (dth_endpoint_tmp != 0xFF))
    {
        config_out->dth = dth_endpoint_tmp;
        config_out->htd = htd_endpoint_tmp;
        config_out->interface = interface_tmp;
        ec = BROPEN_EC_OK;
    }
    else
    {
        ec = BROPEN_EC_KO;
    }

    BROPEN_DEBUG_USB("done - [dth: 0x%2.2X (%u)] [htd: 0x%2.2X (%u)] [interface: %u]",
        config_out->dth, config_out->dth, config_out->htd, config_out->htd, config_out->interface);

    return ec;
}

static bropen_ec_t usb_conf_config_guess_endpoints(bropen_usb_config_t *config_out, const struct libusb_config_descriptor *config)
{
    uint8_t found_count = 0;

    for (uint8_t i = 0; i < config->bNumInterfaces; ++i)
    {
        const struct libusb_interface *interface = &config->interface[i];
        for (int j = 0; j < interface->num_altsetting; ++j)
        {
            if (usb_conf_interface_find_endpoints(config_out, &interface->altsetting[j]) == BROPEN_EC_OK)
            {
                ++found_count;
            }
        }
    }

    return found_count == 1 ? BROPEN_EC_OK : BROPEN_EC_KO;
}

static bropen_ec_t usb_conf_device_guess_endpoints(bropen_usb_config_t *config_out, libusb_device *dev)
{
    bropen_ec_t ec = BROPEN_EC_UNSET;

    BROPEN_DEBUG_USB("start - [dth: 0x%2.2X (%u)] [htd: 0x%2.2X (%u)]",
        config_out->dth, config_out->dth, config_out->htd, config_out->htd);

    {
        struct libusb_device_descriptor desc;
        int libusb_rv = libusb_get_device_descriptor(dev, &desc);

        if (libusb_rv == LIBUSB_SUCCESS)
        {
            if (desc.bNumConfigurations == 1)
            {
                struct libusb_config_descriptor *config = NULL;
                libusb_rv = libusb_get_config_descriptor(dev, 0, &config);
                if (libusb_rv == LIBUSB_SUCCESS)
                {
                    ec = usb_conf_config_guess_endpoints(config_out, config);
                    libusb_free_config_descriptor(config);
                }
                else
                {
                    BROPEN_PRINT_ERROR("failed call [fn: libusb_get_config_descriptor] [rv: %d]", libusb_rv);
                    ec = BROPEN_EC_USB_ERROR;
                }
            }
        }
        else
        {
            BROPEN_PRINT_ERROR("failed call [fn: libusb_get_device_descriptor] [rv: %d]", libusb_rv);
            ec = BROPEN_EC_USB_ERROR;
        }

        BROPEN_DEBUG_USB("guessed [ec: %d] [dth: 0x%2.2X (%u)] [htd: 0x%2.2X (%u)] [interface: %u]",
            ec, config_out->dth, config_out->dth, config_out->htd, config_out->htd, config_out->interface);
    }

    return ec;
}

static bropen_ec_t usb_conf_device_release(libusb_device *device)
{
    libusb_unref_device(device);
    return BROPEN_EC_OK;
}

static bropen_ec_t usb_conf_device_select(libusb_device **device, uint16_t vendor_id, uint16_t product_id)
{
    bropen_ec_t ec = BROPEN_EC_KO;
    libusb_device **devs = NULL;
    ssize_t cnt = 0;

    BROPEN_DEBUG_USB("start - [vendor: 0x%4.4X] [product: 0x%4.4X] [device: %p]", vendor_id, product_id, *device);

    *device = NULL;

    cnt = libusb_get_device_list(NULL, &devs);
    if (cnt > 0)
    {
        libusb_device **devs_i = devs;
        for (libusb_device *dev = *devs_i; dev != NULL && (*device) == NULL; ++devs_i, dev = *devs_i)
        {
            struct libusb_device_descriptor desc;
            int r = libusb_get_device_descriptor(dev, &desc);
            if (r != 0)
            {
                BROPEN_PRINT_ERROR("failed call (non fatal) [fn: libusb_get_device_descriptor] [rv: %d]", r);
                continue;
            }

            if ((desc.idVendor == vendor_id) && (desc.idProduct == product_id))
            {
                *device = dev;
            }
        }

        if (*device != NULL)
        {
            libusb_ref_device(*device);
            ec = BROPEN_EC_OK;
        }

        libusb_free_device_list(devs, 1);
    }
    else if (cnt < 0)
    {
        BROPEN_PRINT_ERROR("failed call [fn: libusb_get_device_list] [rv: %zd]", cnt);
        ec = BROPEN_EC_USB_ERROR;
    }

    BROPEN_DEBUG_USB("done - [vendor: 0x%4.4X] [product: 0x%4.4X] [device: %p]", vendor_id, product_id, *device);

    return ec;
}

static bropen_ec_t usb_device_close(bropen_usb_device_t *usb_device)
{
    bropen_ec_t ec = BROPEN_EC_UNSET;

    libusb_close(usb_device->libusb_handle);
    ec = usb_conf_device_release(usb_device->libusb_device);

    return ec;
}

static bropen_ec_t usb_device_open(bropen_usb_device_t *usb_device, uint16_t vendor_id, uint16_t product_id)
{
    bropen_ec_t ec = BROPEN_EC_UNSET;

    BROPEN_DEBUG_USB("start");

    ec = usb_conf_device_select(&usb_device->libusb_device, vendor_id, product_id);

    if (ec == BROPEN_EC_OK)
    {
        ec = usb_conf_device_guess_endpoints(&usb_device->usb_config, usb_device->libusb_device);
        if (ec == BROPEN_EC_OK)
        {
            int libusb_rv = libusb_open(usb_device->libusb_device, &usb_device->libusb_handle);
            if (libusb_rv != LIBUSB_SUCCESS)
            {
                BROPEN_PRINT_ERROR("failed libusb call [fn: libusb_open] [rv: %d]", libusb_rv);
                ec = BROPEN_EC_COM_ERROR;
            }
        }
    }

    if (ec == BROPEN_EC_OK)
    {
        int libusb_rv = libusb_kernel_driver_active(usb_device->libusb_handle, usb_device->usb_config.interface);
        switch (libusb_rv)
        {
            case 0:
                break; // no driver, all good
            /* code */
            case 1:
            {
                libusb_rv = libusb_detach_kernel_driver(usb_device->libusb_handle, usb_device->usb_config.interface);
                BROPEN_DEBUG_USB("kernel driver detach [rv: %d]", libusb_rv);
            }
            break;
            default:
                BROPEN_PRINT_ERROR("unhandled kernel driver active [rv: %d]", libusb_rv);
                break;
        }
        // for now we don't tag/stop on errors related to the kernel driver
    }

    if (ec == BROPEN_EC_OK)
    {
        int config = 0;
        int libusb_rv = libusb_get_configuration(usb_device->libusb_handle, &config);
        if (libusb_rv != LIBUSB_SUCCESS || config != 1)
        {
            libusb_rv = libusb_set_configuration(usb_device->libusb_handle, 1);
            if (libusb_rv != LIBUSB_SUCCESS)
            {
                BROPEN_PRINT_ERROR("failed libusb call [fn: libusb_set_configuration] [rv: %d]", libusb_rv);
                // ec = BROPEN_EC_USB_ERROR;
            }
        }
    }

    if (ec == BROPEN_EC_OK)
    {
        int libusb_rv = libusb_claim_interface(usb_device->libusb_handle, usb_device->usb_config.interface);
        if (libusb_rv != LIBUSB_SUCCESS)
        {
            BROPEN_PRINT_ERROR("failed libusb call [fn: libusb_claim_interface] [rv: %d]", libusb_rv);
            ec = BROPEN_EC_USB_ERROR;
        }
    }

    if (ec == BROPEN_EC_OK)
    {
        int libusb_rv = libusb_clear_halt(usb_device->libusb_handle, usb_device->usb_config.dth);
        if (libusb_rv != LIBUSB_SUCCESS)
        {
            BROPEN_PRINT_ERROR("failed libusb call [fn: libusb_clear_halt] [endpoint: 0x%2.2X] [rv: %d]", usb_device->usb_config.dth, libusb_rv);
            ec = BROPEN_EC_USB_ERROR;
        }
    }

    if (ec == BROPEN_EC_OK)
    {
        int libusb_rv = libusb_clear_halt(usb_device->libusb_handle, usb_device->usb_config.htd);
        if (libusb_rv != LIBUSB_SUCCESS)
        {
            BROPEN_PRINT_ERROR("failed libusb call [fn: libusb_clear_halt] [endpoint: 0x%2.2X] [rv: %d]", usb_device->usb_config.htd, libusb_rv);
            ec = BROPEN_EC_USB_ERROR;
        }
    }

    if (ec == BROPEN_EC_OK)
    {
        int libusb_rv = libusb_release_interface(usb_device->libusb_handle, usb_device->usb_config.interface);
        if (libusb_rv != LIBUSB_SUCCESS)
        {
            BROPEN_PRINT_ERROR("failed libusb call [fn: libusb_release_interface] [rv: %d]", libusb_rv);
            ec = BROPEN_EC_USB_ERROR;
        }
    }

    BROPEN_DEBUG_USB("done - [device: %p] [handle: %p] [ec: %d]", usb_device->libusb_device, usb_device->libusb_handle, ec);

    return ec;
}

static bropen_ec_t usb_device_find(bropen_usb_device_t *usb_device, uint16_t vendor_id)
{
    bropen_ec_t ec = BROPEN_EC_KO;

    libusb_device **devs = NULL;
    const ssize_t cnt = libusb_get_device_list(NULL, &devs);
    if (cnt > 0)
    {
        libusb_device **devs_i = devs;
        for (libusb_device *dev = *devs_i; dev != NULL && ec != BROPEN_EC_OK; ++devs_i, dev = *devs_i)
        {
            struct libusb_device_descriptor desc;
            int r = libusb_get_device_descriptor(dev, &desc);
            if (r == LIBUSB_SUCCESS && desc.idVendor == vendor_id)
            {
                ec = usb_device_open(usb_device, desc.idVendor, desc.idProduct);
            }
        }

        libusb_free_device_list(devs, 1);
    }

    return ec;
}

static bropen_ec_t data_decode_header_common(const uint8_t *buffer, uint16_t size, uint16_t *consumed)
{
    bropen_ec_t ec = BROPEN_EC_KO;
    const uint8_t header_tag = *buffer;

    switch (header_tag)
    {
        case 0x80:
            ec = BROPEN_EC_DATA_EOF;
            *consumed = 1;
            break;
        case 0x82:
        {
            ec = BROPEN_EC_DATA_UNSUFFICIENT;
            if (size >= 1 + 2)
            {
                const uint16_t info_size = *(uint16_t *)(buffer + 1);
                if (size >= 1 + 2 + info_size)
                {
                    BROPEN_DEBUG_DEC_BUFFER("ignored header [0x82]", buffer, size, 32);
                    ec = BROPEN_EC_OK;
                    *consumed = 1 + 2 + info_size;
                }
            }
        }
        break;
        default:
        {
            BROPEN_PRINT_ERROR("UNSUPORTED HEADER TAG/DATA [header: 0x%2.2X] [data size: %u]", header_tag, size);
            BROPEN_DEBUG_DEC_BUFFER("ignored header data]", buffer, size, 64);
            ec = BROPEN_EC_KO;
        }
        break;
    }

    return ec;
}

static bropen_ec_t jpeg_decode_header(bropen_jpeg_data_helper_t *data_helper, const uint8_t *buffer, uint16_t size, uint16_t *consumed)
{
    bropen_ec_t ec = BROPEN_EC_OK;
    const uint8_t header_tag = *buffer;

    BROPEN_DEBUG_DEC(
        "start - [buffer: %p] [size: %u] [consumed: %u] [header: 0x%2.2X] [ec: %d] [g_size_left: %u]", buffer, size, *consumed, header_tag, ec, data_helper->chunk_size_left);

    switch (header_tag)
    {
        case 0x64:
        {
            uint16_t info_size = *(uint16_t *)(buffer + 1);
            data_helper->chunk_size_left = *(uint16_t *)(buffer + 1 + 2 + info_size);
            *consumed = 1 + 2 + info_size + 2;
        }
        break;
        default:
            ec = data_decode_header_common(buffer, size, consumed);
            break;
    }

    BROPEN_DEBUG_DEC(
        "end   - [buffer: %p] [size: %u] [consumed: %u] [header: 0x%2.2X] [ec: %d] [g_size_left: %u]", buffer, size, *consumed, header_tag, ec, data_helper->chunk_size_left);
    return ec;
}

static bropen_ec_t jpeg_decode(FILE *out, bropen_jpeg_data_helper_t *data_helper, const uint8_t *buffer, uint16_t size)
{
    bropen_ec_t ec = BROPEN_EC_OK;

    BROPEN_DEBUG_DEC("start - [%u] bytes at [%p] [chunk size left: %d]", size, buffer, data_helper->chunk_size_left);
    BROPEN_DEBUG_DEC_BUFFER("decode jpg", buffer, size, 32);
    {
        while (ec == BROPEN_EC_OK && size > 0)
        {
            // header ?
            if (data_helper->chunk_size_left == 0)
            {
                uint16_t consumed = 0;
                ec = jpeg_decode_header(data_helper, buffer, size, &consumed);
                if (ec == BROPEN_EC_OK)
                {
                    buffer += consumed;
                    size -= consumed;
                }
            }

            if (data_helper->chunk_size_left > 0)
            {
                const uint16_t copy_size = data_helper->chunk_size_left > size ? size : data_helper->chunk_size_left;
                fwrite(buffer, copy_size, 1, out);
                buffer += copy_size;
                size -= copy_size;
                data_helper->chunk_size_left -= copy_size;
            }
        }
    }

    BROPEN_DEBUG_DEC("done - [%u] bytes at [%p] [data size left: %d]", size, buffer, data_helper->chunk_size_left);
    return ec;
}

static bropen_ec_t yuv_decode_header(bropen_yuv_data_helper_t *data_helper, const uint8_t *buffer, uint16_t size, uint16_t *consumed)
{
    bropen_ec_t ec = BROPEN_EC_OK;
    const uint8_t header_tag = *buffer;

    BROPEN_DEBUG_DEC("start - [header: 0x%2.2X] [size: %u] [yuv index: %u] [yuv chunk left: %u]", header_tag, size, data_helper->channel_index, data_helper->channel_size_left);

    *consumed = 0;

    if (data_helper->channel_size_left != 0 || size == 0)
    {
        ec = BROPEN_EC_INTERNAL_ERROR;
    }

    if (ec == BROPEN_EC_OK)
    {
        switch (header_tag)
        {
            case 0x44:
            case 0x48:
            case 0x4C:
            {
                if (size >= 3)
                {
                    const uint16_t info_size = *(uint16_t *)(buffer + 1);
                    if (size >= (1 + 2 + info_size + 2))
                    {
                        data_helper->channel_index = (header_tag - 0x44) / 4;
                        data_helper->channel_size_left = *(uint16_t *)(buffer + 1 + 2 + info_size);
                        // TODO add check that the size is the scanning area width?
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
            default:
                ec = data_decode_header_common(buffer, size, consumed);
                break;
        }
    }

    BROPEN_DEBUG_DEC("done - [ec: %d] [consumed: %u] [yuv index: %u] [yuv chunk left: %u]", ec, *consumed, data_helper->channel_index, data_helper->channel_size_left);

    return ec;
}

static bropen_ec_t yuv_decode(bropen_buffer_helper_t *rgb_buffer, bropen_yuv_data_helper_t *yuv_helper, const uint8_t *buffer_in, uint16_t in_size)
{
    bropen_ec_t ec = BROPEN_EC_OK;

    BROPEN_DEBUG_DEC("start - [size: %u] [yuv index: %u] [yuv chunk left: %u]", in_size, yuv_helper->channel_index, yuv_helper->channel_size_left);

    while (ec == BROPEN_EC_OK && in_size > 0)
    {
        if (yuv_helper->channel_size_left == 0)
        {
            uint16_t consumed = 0;
            ec = yuv_decode_header(yuv_helper, buffer_in, in_size, &consumed);
            if (ec == BROPEN_EC_OK)
            {
                if (consumed > 0)
                {
                    buffer_in += consumed;
                    in_size -= consumed;
                }
                else
                {
                    ec = BROPEN_EC_INTERNAL_ERROR;
                }
            }
        }

        if (ec == BROPEN_EC_OK && yuv_helper->channel_size_left > 0)
        {
            const uint16_t add_size = yuv_helper->channel_size_left > in_size ? in_size : yuv_helper->channel_size_left;
            uint16_t added = 0;
            ec = bropen_yuv_channel_data_add(yuv_helper->channel_index, buffer_in, add_size, &added);
            buffer_in += added;
            in_size -= added;
            yuv_helper->channel_size_left -= added;

            if (ec == BROPEN_EC_OK)
            {
                const uint16_t available = bropen_yuv_rgb_data_available_count();
                if (available > 0)
                {
                    if (available + rgb_buffer->used <= rgb_buffer->capacity)
                    {
                        uint16_t wrote = 0;
                        ec = bropen_yuv_rgb_data_write(rgb_buffer->data + rgb_buffer->used, available, &wrote);
                        if (wrote > 0)
                        {
                            rgb_buffer->used += wrote;
                        }
                    }
                    else
                    {
                        ec = BROPEN_EC_BUFFER_SIZE;
                        BROPEN_PRINT_ERROR("rgb buffer too small");
                    }
                }
            }
        }
    }

    BROPEN_DEBUG_DEC("done - [ec: %d] [yuv index: %u] [yuv chunk left: %u]", ec, yuv_helper->channel_index, yuv_helper->channel_size_left);

    return ec;
}

static bropen_ec_t scanner_protocol_code_page_query(uint16_t *buffer_wrote, uint8_t *buffer, uint16_t buffer_size, const bropen_scanner_info_t *scanner_info)
{
    bropen_ec_t ec = BROPEN_EC_OK;

    if (ec == BROPEN_EC_OK)
    {
        // start tag
        ec = helper_buffer_insert_str(buffer, buffer_size, buffer_wrote, "\x1BI\n");
    }

    if (ec == BROPEN_EC_OK)
    {
        // dpi
        switch (scanner_info->dpi_capabilities)
        {
            case 0x00:
                ec = BROPEN_EC_KO;
                break;
            case 0x04:
                ec = helper_buffer_insert_str(buffer, buffer_size, buffer_wrote, "R=300,300\n");
                break;
            default:
                ec = helper_buffer_insert_str(buffer, buffer_size, buffer_wrote, "R=100,100\n");
                break;
        }
    }

    if (ec == BROPEN_EC_OK)
    {
        // color
        ec = helper_buffer_insert_str(buffer, buffer_size, buffer_wrote, "M=CGRAY\n");
    }

    if (ec == BROPEN_EC_OK)
    {
        // end tag
        ec = helper_buffer_insert_str(buffer, buffer_size, buffer_wrote, "\x80");
    }

    return ec;
}

static bropen_ec_t scanner_protocol_code_scan_start(uint16_t *cmd_size, uint8_t *buffer, uint16_t size, const bropen_scanpage_info_t *page_info, bropen_scan_format_t format)
{
    bropen_ec_t ec = BROPEN_EC_OK;

    BROPEN_DEBUG_SCAN("start");

    *cmd_size = 0;

    if (ec == BROPEN_EC_OK)
    {
        // start tag
        ec = helper_buffer_insert_str(buffer, size, cmd_size, "\x1BX\n");
    }

    if (ec == BROPEN_EC_OK)
    {
        // dpi
        uint8_t dpi_buffer[512];
        uint16_t pos = 0;

        ec = helper_buffer_insert_str(dpi_buffer, sizeof dpi_buffer, &pos, "R=");
        if (ec == BROPEN_EC_OK)
        {
            ec = helper_buffer_insert_u16(dpi_buffer, sizeof dpi_buffer, &pos, page_info->w_dpi);
        }
        if (ec == BROPEN_EC_OK)
        {
            ec = helper_buffer_insert_str(dpi_buffer, sizeof dpi_buffer, &pos, ",");
        }
        if (ec == BROPEN_EC_OK)
        {
            ec = helper_buffer_insert_u16(dpi_buffer, sizeof dpi_buffer, &pos, page_info->h_dpi);
        }
        if (ec == BROPEN_EC_OK)
        {
            ec = helper_buffer_insert_str(dpi_buffer, sizeof dpi_buffer, &pos, "\n");
        }
        if (ec == BROPEN_EC_OK)
        {
            if (pos < sizeof dpi_buffer)
            {
                dpi_buffer[pos++] = '\0';
            }
            else
            {
                ec = BROPEN_EC_BUFFER_SIZE;
            }
        }

        if (ec == BROPEN_EC_OK)
        {
            ec = helper_buffer_insert_str(buffer, size, cmd_size, (const char *)dpi_buffer);
        }
    }

    if (ec == BROPEN_EC_OK)
    {
        // color
        ec = helper_buffer_insert_str(buffer, size, cmd_size, "M=CGRAY\n");
    }

    if (ec == BROPEN_EC_OK)
    {
        // compression
        switch (format)
        {
            case BROPEN_SCAN_FORMAT_BMP:
                ec = helper_buffer_insert_str(buffer, size, cmd_size, "C=NONE\n");
                break;
            default:
                ec = helper_buffer_insert_str(buffer, size, cmd_size, "C=JPEG\nJ=MID\n");
                break;
        }
    }

    if (ec == BROPEN_EC_OK)
    {
        // brightness
        ec = helper_buffer_insert_str(buffer, size, cmd_size, "B=100\n");
    }

    if (ec == BROPEN_EC_OK)
    {
        // contrast
        ec = helper_buffer_insert_str(buffer, size, cmd_size, "N=100\n");
    }

    if (ec == BROPEN_EC_OK)
    {
        // area
        uint8_t area_buffer[512];
        uint16_t pos = 0;

        ec = helper_buffer_insert_str(area_buffer, sizeof area_buffer, &pos, "A=0,0,");
        if (ec == BROPEN_EC_OK)
        {
            ec = helper_buffer_insert_u16(area_buffer, sizeof area_buffer, &pos, page_info->width);
        }
        if (ec == BROPEN_EC_OK)
        {
            ec = helper_buffer_insert_str(area_buffer, sizeof area_buffer, &pos, ",");
        }
        if (ec == BROPEN_EC_OK)
        {
            ec = helper_buffer_insert_u16(area_buffer, sizeof area_buffer, &pos, page_info->height);
        }
        if (ec == BROPEN_EC_OK)
        {
            ec = helper_buffer_insert_str(area_buffer, sizeof area_buffer, &pos, "\n");
        }
        if (ec == BROPEN_EC_OK)
        {
            if (pos < sizeof area_buffer)
            {
                area_buffer[pos++] = '\0';
            }
            else
            {
                ec = BROPEN_EC_BUFFER_SIZE;
            }
        }

        if (ec == BROPEN_EC_OK)
        {
            ec = helper_buffer_insert_str(buffer, size, cmd_size, (const char *)area_buffer);
        }
    }

    if (ec == BROPEN_EC_OK)
    {
        // end tag
        ec = helper_buffer_insert_str(buffer, size, cmd_size, "\x80");
    }

    return ec;
}

static bropen_ec_t scanner_protocol_decode_page_info(bropen_scanpage_info_t *page_info, const uint8_t *buffer, uint16_t buffer_size)
{
    bropen_ec_t ec = BROPEN_EC_KO;

    BROPEN_DEBUG_SCAN("start");
    BROPEN_DEBUG_SCAN_BUFFER("scanpage info", buffer, buffer_size, buffer_size);

    if (buffer_size > 2)
    {
        uint16_t value = 0;
        uint16_t consumed = 0;
        buffer += 3;
        buffer_size -= 3;

        // dpi
        ec = helper_parse_u16(&page_info->w_dpi, &consumed, buffer, buffer_size);
        if (ec == BROPEN_EC_OK)
        {
            buffer += consumed;
            buffer_size -= consumed;
            // dpi
            ec = helper_parse_u16(&page_info->h_dpi, &consumed, buffer, buffer_size);
        }

        if (ec == BROPEN_EC_OK)
        {
            buffer += consumed;
            buffer_size -= consumed;
            // ???
            ec = helper_parse_u16(&value, &consumed, buffer, buffer_size);
        }

        if (ec == BROPEN_EC_OK)
        {
            buffer += consumed;
            buffer_size -= consumed;
            // width in mm
            ec = helper_parse_u16(&value, &consumed, buffer, buffer_size);
        }

        if (ec == BROPEN_EC_OK)
        {
            buffer += consumed;
            buffer_size -= consumed;
            // width in pixels
            ec = helper_parse_u16( &page_info->width, &consumed, buffer, buffer_size);
        }

        if (ec == BROPEN_EC_OK)
        {
            buffer += consumed;
            buffer_size -= consumed;
            // heigth in mm
            ec = helper_parse_u16(&value, &consumed, buffer, buffer_size);
        }

        if (ec == BROPEN_EC_OK)
        {
            buffer += consumed;
            buffer_size -= consumed;
            // heigth in pixels
            ec = helper_parse_u16(&page_info->height, &consumed, buffer, buffer_size);
        }

        if (ec == BROPEN_EC_OK)
        {
            buffer += consumed;
            buffer_size -= consumed;
            if (buffer_size != 1 || buffer[0] != 0x00)
            {
                ec = BROPEN_EC_KO;
            }
        }
    }

    BROPEN_DEBUG_SCAN("done [ec: %d] [buffer: %p] [buffer size: %u]", ec, buffer, buffer_size);

    return ec;
}

static bropen_ec_t scanner_protocol_decode_scanner_info(bropen_scanner_info_t *scanner_info, const uint8_t *buffer, uint16_t buffer_size)
{
    bropen_ec_t ec = BROPEN_EC_KO;

    BROPEN_DEBUG_SCAN("start");
    BROPEN_DEBUG_SCAN_BUFFER("scanner info", buffer, buffer_size, buffer_size);

    if (buffer_size > 2)
    {
        const uint8_t info_size = *(uint8_t *)(buffer + 2);
        if (info_size + 2 == buffer_size)
        {
            buffer += 2; // device info id
            buffer += 1; // info size
            if (buffer_size > 16)
            {
                scanner_info->protocol_type = *buffer;
                buffer += 1; // protocol type
                buffer += 1; // video signal type
                scanner_info->color_capabilities.raw = *buffer;
                buffer += 1; // color type
                buffer += 2; // ntsc signal
                buffer += 2; // pal signal
                buffer += 2; // secam signal
                buffer += 1; // hardware type
                buffer += 1; // hardware version
                scanner_info->dpi_capabilities = *buffer;
                buffer += 1; // main dpi
                buffer += 1; // paper size max

                (void)buffer;
            }

            ec = BROPEN_EC_OK;
        }
    }

    BROPEN_DEBUG_SCAN("done [ec: %d]", ec);

    return ec;
}

static bropen_ec_t scanner_scan_cancel(bropen_usb_device_t *usb_device)
{
    bropen_ec_t ec = BROPEN_EC_UNSET;
    uint8_t cancel_cmd[] = {0x1B, 0x52};

    BROPEN_DEBUG_SCAN("start");

    ec = usb_com_send(usb_device, cancel_cmd, sizeof cancel_cmd, "cancel");

    if (ec == BROPEN_EC_OK)
    {
        // flush
        bropen_ec_t rcv_ec = BROPEN_EC_UNSET;
        uint16_t transfered = 0;
        do
        {
            uint8_t buffer[RCV_BUFFER_SIZE];
            rcv_ec = usb_com_rcv(usb_device, buffer, sizeof buffer, &transfered, 1000);
        } while (rcv_ec == BROPEN_EC_OK && transfered > 0);
    }

    BROPEN_DEBUG_SCAN("done [ec: %d]", ec);

    return ec;
}

static bropen_ec_t scanner_scan_query_page(bropen_usb_device_t *usb_device, const bropen_scanner_info_t *scanner_info, bropen_scanpage_info_t *scanpage_info)
{
    bropen_ec_t ec = BROPEN_EC_UNSET;
    uint8_t command[512];
    uint16_t command_len = 0;

    BROPEN_DEBUG_SCAN("start");

    ec = scanner_protocol_code_page_query(&command_len, command, sizeof command, scanner_info);
    if (ec == BROPEN_EC_KO)
    {
        ec = BROPEN_EC_INTERNAL_ERROR;
    }

    if (ec == BROPEN_EC_OK)
    {
        ec = usb_com_send(usb_device, command, command_len, "scanpage query info");
    }

    if (ec == BROPEN_EC_OK)
    {
        uint8_t buffer[RCV_BUFFER_SIZE];
        uint16_t transfered = 0;
        ec = usb_com_rcv(usb_device, buffer, sizeof buffer, &transfered, RCV_TIMEOUT_DEFAULT);
        if (transfered > 0 && ec == BROPEN_EC_OK)
        {
            ec = scanner_protocol_decode_page_info(scanpage_info, buffer, transfered);
        }
    }

    BROPEN_DEBUG_SCAN("done [ec: %d] [scanner protocol: 0x%2.2X] [scanner color: 0x%2.2X] [scanner dpi: 0x%2.2X]", ec, scanner_info->protocol_type,
        scanner_info->color_capabilities.raw, scanner_info->dpi_capabilities);

    return ec;
}

static bropen_ec_t scanner_scan_query_scanner(bropen_usb_device_t *usb_device, bropen_scanner_info_t *scanner_info)
{
    bropen_ec_t ec = BROPEN_EC_UNSET;
    uint8_t query_info[] = {0x1b, 0x51, 0x0a, 0x80};

    BROPEN_DEBUG_SCAN("start");

    ec = usb_com_send(usb_device, query_info, sizeof query_info, "scanner query info");
    if (ec == BROPEN_EC_OK)
    {
        uint8_t buffer[RCV_BUFFER_SIZE];
        uint16_t transfered = 0;
        ec = usb_com_rcv(usb_device, buffer, sizeof buffer, &transfered, RCV_TIMEOUT_DEFAULT);
        if (transfered > 0 && ec == BROPEN_EC_OK)
        {
            ec = scanner_protocol_decode_scanner_info(scanner_info, buffer, transfered);
        }
    }

    BROPEN_DEBUG_SCAN("done [ec: %d] [scanner protocol: 0x%2.2X] [scanner color: 0x%2.2X] [scanner dpi: 0x%2.2X]", ec, scanner_info->protocol_type,
        scanner_info->color_capabilities.raw, scanner_info->dpi_capabilities);

    return ec;
}

static bropen_ec_t scanner_scan_scanning_bmp(FILE *dst, bropen_usb_device_t *usb_device, const bropen_scanpage_info_t *scanpage_info)
{
    BROPEN_DEBUG_SCAN("start");

    {
        bropen_buffer_helper_t rgb_buffer;
        bropen_ec_t ec = bropen_buffer_helper_init(&rgb_buffer, scanpage_info->width * scanpage_info->height * 3);
        if (ec == BROPEN_EC_OK)
        {
            uint8_t rcv_buffer[RCV_BUFFER_SIZE];
            uint16_t transfered = 0;

            ec = usb_com_rcv(usb_device, rcv_buffer, sizeof rcv_buffer, &transfered, RCV_TIMEOUT_DEFAULT);
            if (transfered > 0)
            {
                bropen_yuv_data_helper_t yuv_helper;
                bropen_yuv_data_helper_init(&yuv_helper);

                ec = yuv_decode(&rgb_buffer, &yuv_helper, rcv_buffer, transfered);
                while (ec == 0 && transfered > 0)
                {
                    ec = usb_com_rcv(usb_device, rcv_buffer, sizeof rcv_buffer, &transfered, RCV_TIMEOUT_DEFAULT);
                    if (transfered > 0)
                    {
                        ec = yuv_decode(&rgb_buffer, &yuv_helper, rcv_buffer, transfered);
                    }
                }

                bropen_yuv_data_helper_end(&yuv_helper);
            }

            if (ec == BROPEN_EC_OK || ec == BROPEN_EC_DATA_EOF)
            {
                helper_bmp_write(dst, scanpage_info, &rgb_buffer);
            }

            bropen_buffer_helper_end(&rgb_buffer);
        }

        BROPEN_DEBUG_SCAN("done - [ec: %d]", ec);

        return ec;
    }
}

static bropen_ec_t scanner_scan_scanning_jpeg(FILE *dst, bropen_usb_device_t *usb_device)
{
    BROPEN_DEBUG_SCAN("start");

    {
        uint8_t rcv_buffer[RCV_BUFFER_SIZE];
        uint16_t transfered = 0;
        bropen_ec_t ec = usb_com_rcv(usb_device, rcv_buffer, sizeof rcv_buffer, &transfered, RCV_TIMEOUT_DEFAULT);
        if (transfered > 0)
        {
            bropen_jpeg_data_helper_t jpeg_helper;
            bropen_jpeg_data_helper_init(&jpeg_helper);

            ec = jpeg_decode(dst, &jpeg_helper, rcv_buffer, transfered);
            while (ec == BROPEN_EC_OK && transfered > 0)
            {
                ec = usb_com_rcv(usb_device, rcv_buffer, sizeof rcv_buffer, &transfered, RCV_TIMEOUT_DEFAULT);
                if (transfered > 0)
                {
                    ec = jpeg_decode(dst, &jpeg_helper, rcv_buffer, transfered);
                }
            }

            bropen_jpeg_data_helper_end(&jpeg_helper);
        }

        BROPEN_DEBUG_SCAN("end [ec: %d]", ec);
        return ec;
    }
}

static bropen_ec_t scanner_scan_start(bropen_usb_device_t *usb_device, const bropen_scanner_info_t *, const bropen_scanpage_info_t *scanpage_info, bropen_scan_format_t format)
{
    uint8_t buffer[512];
    uint16_t cmd_size = 0;

    bropen_ec_t ec = scanner_protocol_code_scan_start(&cmd_size, buffer, sizeof buffer, scanpage_info, format);
    if (ec == BROPEN_EC_OK)
    {
        ec = usb_com_send(usb_device, buffer, cmd_size, "scan start");
    }

    return ec;
}

static bropen_ec_t scanner_control_get(bropen_usb_device_t *usb_device)
{
    bropen_ec_t ec = BROPEN_EC_KO;

    BROPEN_DEBUG_COM("command [control get]");

    {
        uint8_t control_data[5] = {0, 0, 0, 0, 0};
        int libusb_rv = libusb_control_transfer(usb_device->libusb_handle, 0xC0, 0x01, 0x02, 0, control_data, sizeof(control_data), 2000);
        BROPEN_DEBUG_COM_BUFFER("control get", control_data, sizeof control_data, sizeof control_data);
        if (libusb_rv == sizeof(control_data))
        {
            if (control_data[0] != sizeof control_data)
            {
                ec = BROPEN_EC_COM_ERROR;
            }
            else if (control_data[1] != 0x10)
            {
                ec = BROPEN_EC_COM_ERROR;
            }
            else if (control_data[2] != 0x01)
            {
                ec = BROPEN_EC_COM_ERROR;
            }
            else if (control_data[3] != 0x02)
            {
                ec = BROPEN_EC_COM_ERROR;
            }
            else if (control_data[4] != 0x00)
            {
                ec = BROPEN_EC_COM_ERROR;
            }
            else
            {
                ec = BROPEN_EC_OK;
            }
        }
    }

    return ec;
}

static bropen_ec_t scanner_control_release(bropen_usb_device_t *usb_device)
{
    BROPEN_DEBUG_COM("command [control release]");
    {
        uint8_t control_data[5] = {0, 0, 0, 0, 0};
        int ec = libusb_control_transfer(usb_device->libusb_handle, 0xC0, 0x02, 0x02, 0, control_data, sizeof(control_data), 2000);
        BROPEN_DEBUG_COM_BUFFER("control release", control_data, sizeof control_data, sizeof control_data);
        return ec == sizeof control_data ? BROPEN_EC_OK : BROPEN_EC_KO;
    }
}

static bropen_ec_t scanner_close(bropen_usb_device_t *device)
{
    return usb_device_close(device);
}

static bropen_ec_t scanner_open(bropen_usb_device_t *device, uint16_t product_id)
{
    bropen_ec_t ec = BROPEN_EC_UNSET;

    if (product_id != PRODUCT_ID_INVALID)
    {
        ec = usb_device_open(device, 0x04f9, product_id);
    }
    else
    {
        ec = usb_device_find(device, 0x04f9);
    }

    if (ec == BROPEN_EC_OK)
    {
        struct libusb_device_descriptor desc;
        libusb_get_device_descriptor(device->libusb_device, &desc);

        BROPEN_PRINT_USER_INFO("selected scanner: [vendor: 0x%4.4x] [product: 0x%4.4x] [interface: %u] [dth: %u] [htd: %u]",
            desc.idVendor, desc.idProduct, device->usb_config.interface, device->usb_config.dth, device->usb_config.htd);
    }
    else
    {
        BROPEN_PRINT_USER_INFO("no suitable scanner found");
    }

    return ec;
}

static bropen_ec_t scanner_reset(bropen_usb_device_t *usb_device)
{
    bropen_ec_t ec = BROPEN_EC_UNSET;

    BROPEN_DEBUG_COM("start");

    ec = scanner_control_get(usb_device);
    if (ec == BROPEN_EC_OK)
    {
        ec = scanner_scan_cancel(usb_device);

        if (ec == BROPEN_EC_OK)
        {
            ec = scanner_control_release(usb_device);
        }
        else
        {
            scanner_control_release(usb_device);
        }
    }
    else
    {
        ec = BROPEN_EC_COM_ERROR;
    }

    BROPEN_DEBUG_COM("done - [ec: %d]", ec);

    return ec;
}

static bropen_ec_t scanner_scan(FILE *dst, bropen_scan_format_t format, bropen_usb_device_t *usb_device)
{
    bropen_ec_t ec = BROPEN_EC_OK;
    bropen_scanner_info_t scanner_info;
    bropen_scanpage_info_t scanpage_info;

    BROPEN_DEBUG_SCAN("start [format: %d]", format);

    bropen_scanner_info_init(&scanner_info);
    bropen_scanpage_info_init(&scanpage_info);

    if (ec == BROPEN_EC_OK)
    {
        ec = scanner_control_get(usb_device);
    }

    if (ec == BROPEN_EC_OK)
    {
        ec = scanner_scan_query_scanner(usb_device, &scanner_info);
    }

    if (ec == BROPEN_EC_OK)
    {
        ec = scanner_control_release(usb_device);
    }

    if (ec == BROPEN_EC_OK)
    {
        ec = scanner_control_get(usb_device);
    }

    if (ec == BROPEN_EC_OK)
    {
        ec = scanner_scan_query_page(usb_device, &scanner_info, &scanpage_info);
    }

    if (ec == BROPEN_EC_OK)
    {
        ec = scanner_scan_start(usb_device, &scanner_info, &scanpage_info, format);
    }

    if (ec == BROPEN_EC_OK)
    {
        switch (format)
        {
            case BROPEN_SCAN_FORMAT_BMP:
                ec = scanner_scan_scanning_bmp(dst, usb_device, &scanpage_info);
                break;
            case BROPEN_SCAN_FORMAT_JPEG:
                ec = scanner_scan_scanning_jpeg(dst, usb_device);
                break;
        }
    }

    if (ec == BROPEN_EC_OK || ec == BROPEN_EC_DATA_EOF)
    {
        ec = scanner_scan_cancel(usb_device);
    }

    if (ec == BROPEN_EC_OK)
    {
        ec = scanner_control_release(usb_device);
    }

    bropen_scanpage_info_end(&scanpage_info);
    bropen_scanner_info_end(&scanner_info);

    return ec;
}

int main(int argc, const char *argv[])
{
    bropen_ec_t ec = 0;

    bropen_user_options_t user_options;

    bropen_user_options_init(&user_options);
    ec = helper_parse_user_options(&user_options, &argc, argv);

    if (user_options.help != 0)
    {
        helpert_usage_print();
        ec = BROPEN_EC_OK;
    }
    else if (ec != BROPEN_EC_OK)
    {
        printf("\n");
        helpert_usage_print();
    }
    else if (argc < 2)
    {
        ec = BROPEN_EC_KO;
        BROPEN_PRINT_USER_ERROR("fatal error: no output file provided");
        printf("\n");
        helpert_usage_print();
    }
    else if (argc > 2)
    {
        ec = BROPEN_EC_KO;
        BROPEN_PRINT_USER_ERROR("fatal error: too much arguments");
        printf("\n");
        helpert_usage_print();
    }
    else
    {
        FILE *dst = NULL;
        
        user_options.out_path = argv[1];

        ec = helper_destination_open(&dst, &user_options);
        if (ec == BROPEN_EC_OK)
        {
            int libusb_ec = libusb_init(NULL);
            if (libusb_ec == 0)
            {
                bropen_usb_device_t device;

                bropen_usb_device_init(&device);

                ec = scanner_open(&device, user_options.product);
                if (ec == BROPEN_EC_OK)
                {
                    ec = scanner_reset(&device);
                    if (ec == BROPEN_EC_OK)
                    {
                        ec = scanner_scan(dst, user_options.format, &device);
                    }

                    if (ec == BROPEN_EC_OK)
                    {
                        ec = scanner_close(&device);
                    }
                    else
                    {
                        scanner_close(&device);
                    }
                }

                bropen_usb_device_end(&device);

                libusb_exit(NULL);
            }
            else
            {
                ec = BROPEN_EC_USB_ERROR;
                BROPEN_PRINT_ERROR("failed to initialize libsub");
            }
            fclose(dst);
        }
    }

    bropen_user_options_end(&user_options);

    return helper_error_code_shell(ec, !user_options.help);
}
