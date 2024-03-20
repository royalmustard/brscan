#include "bropen_sane.h"

#include "bropen_ec.h"
#include "bropen_scan.h"

#include "bropen_debug.h"

static SANE_Status convert_status(bropen_ec_t bropen_status)
{
    SANE_Status sane_status;

    switch (bropen_status)
    {
        case BROPEN_EC_OK:
            sane_status = SANE_STATUS_GOOD;
            break;
        case BROPEN_EC_DATA_EOF:
            sane_status = SANE_STATUS_EOF;
            break;
        case BROPEN_EC_DATA_UNSUPPORTED_HEADER:
        case BROPEN_EC_COM_ERROR:
        case BROPEN_EC_USB_ERROR:
            BROPEN_PRINT_USER_ERROR("error communicating with the scanner, try restarting it to fix the issue [code: %d]", bropen_status);
            sane_status = SANE_STATUS_INVAL;
            break;
        default:
            // codes that should never get here
            BROPEN_PRINT_INTERNAL("bropen status conversion error [bropen status: %d]", bropen_status);
            sane_status = SANE_STATUS_INVAL;
            break;
    }

    return sane_status;
}

void bropen_sane_cancel(SANE_Handle handle)
{
    (void) handle;
    bropen_scan_reset();
}

SANE_Status bropen_sane_read(SANE_Handle handle, SANE_Byte *buf, SANE_Int size, SANE_Int *size_done, bropen_com_params_t *com_params)
{
    bropen_ec_t ec;
    uint16_t wrote = 0;

    (void) handle;

    *size_done = 0;

    if (BROPEN_G_DEBUG_SANE)
    {
        printf("\n>>>>>>>>\n");
    }

    BROPEN_DEBUG_SANE("start - [dst: %p] [size: %d] [wrote: %d]", buf, size, *size_done);
    BROPEN_DEBUG_SANE_BUFFER("buffer", buf, size, 16);

    ec = bropen_scan_data(&wrote, buf, size, com_params);
    if ((ec == BROPEN_EC_OK) || (ec == BROPEN_EC_DATA_EOF))
    {
        *size_done = wrote;
    }

    BROPEN_DEBUG_SANE("end   - [dst: %p] [size: %d] [wrote: %d] [ec: %d]", buf, size, *size_done, ec);
    BROPEN_DEBUG_SANE_BUFFER("bufffer", buf, size, 16);

    if (BROPEN_G_DEBUG_SANE)
    {
        printf("---------\n");
    }

    return convert_status(ec);
}
