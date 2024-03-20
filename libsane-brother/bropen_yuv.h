#ifndef _BROPEN_DECODE_H_
#define _BROPEN_DECODE_H_

#include "bropen_ec.h"

#include <stdint.h>

uint16_t bropen_yuv_rgb_data_available_count();
bropen_ec_t bropen_yuv_channel_data_add(uint8_t channel, const uint8_t *data, uint16_t size, uint16_t *added_count);
void bropen_yuv_reset();
bropen_ec_t bropen_yuv_rgb_data_write(uint8_t *buffer_out, uint16_t size, uint16_t *wrote_count);

#endif /* _BROPEN_DECODE_H_ */
