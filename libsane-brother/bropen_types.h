#ifndef _BROPEN_TYPES_H_
#define _BROPEN_TYPES_H_

#include <stdint.h>

// TODO remove that pragma and undef
// usb_dev_handle is a libusb type and is messed around in brother.h
#pragma push_macro("usb_dev_handle")
#undef usb_dev_handle

typedef struct usb_dev_handle usb_dev_handle;

typedef struct bropen_com_params_t
{
    usb_dev_handle *usb_handle;
    uint8_t endpoint_dth; // device to host
    uint8_t endpoint_htd; // host to device

} bropen_com_params_t;

void bropen_com_params_init(bropen_com_params_t *);
void bropen_com_params_end(bropen_com_params_t *);

#pragma pop_macro("usb_dev_handle")

typedef union bropen_scanner_color_capabilities_t {
    union {
        struct
        {
            uint8_t black_white : 1;
            uint8_t error_diffusion : 1;
            uint8_t true_gray : 1;
            uint8_t color_256 : 1;
            uint8_t color_24b : 1;
            uint8_t color_256i : 1;
        };
        uint8_t raw;
    };
} bropen_scanner_color_capabilities_t;

typedef struct bropen_scanner_info_t
{
    uint8_t dpi_capabilities;
    bropen_scanner_color_capabilities_t color_capabilities;
    uint8_t protocol_type;
} bropen_scanner_info_t;

void bropen_scanner_info_init(bropen_scanner_info_t *);
void bropen_scanner_info_end(bropen_scanner_info_t *);

typedef struct bropen_scanpage_info_t
{
    uint16_t width;
    uint16_t height;
    uint16_t w_dpi;
    uint16_t h_dpi;
} bropen_scanpage_info_t;

void bropen_scanpage_info_init(bropen_scanpage_info_t *);
void bropen_scanpage_info_end(bropen_scanpage_info_t *);

#endif /* _BROPEN_TYPES_H_ */
