#include "bropen_scan.h"

#include "bropen_ec.h"
#include "bropen_debug.h"
#include "bropen_types.h"

#include <usb.h>

#include <stddef.h>

static const int RCV_DEFAULT_TIMEOUT_MS = 10000; // EOF header can take up to 5 seconds
static const int READ_DEFAULT_TIMEOUT_MS = 100;
static const int READ_TRY_COUNT = RCV_DEFAULT_TIMEOUT_MS / READ_DEFAULT_TIMEOUT_MS;

bropen_ec_t bropen_com_receive(uint16_t *received_size, uint8_t *buffer, uint16_t buffer_size, bropen_com_params_t *params)
{
    bropen_ec_t ec = BROPEN_EC_UNSET;

    BROPEN_DEBUG_COM("start - [size: %u] [received: %u] [buffer: %p] ", buffer_size, *received_size, buffer);

    if (ec == BROPEN_EC_UNSET && (params->usb_handle == NULL || params->endpoint_dth == 0xFF))
    {
        ec = BROPEN_EC_INTERNAL_ERROR;
    }

    if (ec == BROPEN_EC_UNSET)
    {
        int libusb_rv = usb_bulk_read(params->usb_handle, params->endpoint_dth, (char *)buffer, buffer_size, 0);
        BROPEN_DEBUG_COM("inital read - [usb_bulk_read: %d]", libusb_rv);

        if (libusb_rv <= 0)
        {
            int try_count = 1;
            while (try_count++ < READ_TRY_COUNT && libusb_rv <= 0)
            {
                usleep(READ_DEFAULT_TIMEOUT_MS * 1000);
                libusb_rv = usb_bulk_read(params->usb_handle, params->endpoint_dth, (char *)buffer, buffer_size, 0);
                BROPEN_DEBUG_COM("read [%d] - [usb_bulk_read: %d]", try_count, libusb_rv);
            }
        }

        if (libusb_rv > 0)
        {
            if (libusb_rv <= UINT16_MAX)
            {
                *received_size = (uint16_t)libusb_rv;
                ec = BROPEN_EC_OK;
            }
            else
            {
                ec = BROPEN_EC_COM_ERROR;
            }
        }
        else if (libusb_rv == 0)
        {
            // TODO - HACK ... check that scanner really doesn't send the EOF header?
            //ec = BROPEN_EC_DATA_EOF;

            ec = BROPEN_EC_KO;
        }
        else
        {
            ec = BROPEN_EC_COM_ERROR;
        }
    }

    BROPEN_DEBUG_COM("done - [received: %u] [size: %u] [buffer: %p] ", *received_size, buffer_size, buffer);

    return ec;
}