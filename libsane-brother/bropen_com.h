#ifndef _BROPEN_COM_H_
#define _BROPEN_COM_H_

#include "bropen_ec.h"

#include "stdint.h"

struct bropen_com_params_t;

bropen_ec_t bropen_com_receive(uint16_t *received_size, uint8_t *buffer, uint16_t buffer_size, const bropen_com_params_t *);

#endif /* _BROPEN_COM_H_ */