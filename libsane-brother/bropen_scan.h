#ifndef _BROPEN_SCAN_H_
#define _BROPEN_SCAN_H_

#include "bropen_ec.h"

#include "stdint.h"

typedef struct bropen_com_params_t bropen_com_params_t;

bropen_ec_t bropen_scan_data(uint16_t *size_wrote, uint8_t *buffer, uint16_t buffer_size, bropen_com_params_t *);
void bropen_scan_reset();

#endif /* _BROPEN_SCAN_H_ */
