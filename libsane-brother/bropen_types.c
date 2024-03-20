#include "bropen_types.h"

#include <stddef.h>

void bropen_com_params_init(bropen_com_params_t *params)
{
    params->usb_handle = NULL;
    params->endpoint_dth = 0xFF;
    params->endpoint_htd = 0xFF;
}

void bropen_com_params_end(bropen_com_params_t *params)
{
    params->usb_handle = NULL;
    params->endpoint_dth = 0xFF;
    params->endpoint_htd = 0xFF;
}

void bropen_scanner_info_init(bropen_scanner_info_t *si)
{
    si->dpi_capabilities = 0x00;
    si->color_capabilities.raw = 0x0;
    ;
    si->protocol_type = 0x00;
    ;
}

void bropen_scanner_info_end(bropen_scanner_info_t *si)
{
    si->protocol_type = 0x00;
    ;
    si->color_capabilities.raw = 0x0;
    ;
    si->dpi_capabilities = 0x00;
}

void bropen_scanpage_info_init(bropen_scanpage_info_t *sp)
{
    sp->width = 0;
    sp->height = 0;
    sp->w_dpi = 0;
    sp->h_dpi = 0;
}

void bropen_scanpage_info_end(bropen_scanpage_info_t *sp)
{
    sp->width = 0;
    sp->height = 0;
    sp->w_dpi = 0;
    sp->h_dpi = 0;
}
