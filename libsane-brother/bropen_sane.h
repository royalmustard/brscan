#ifndef _BROPEN_SANE_H_
#define _BROPEN_SANE_H_

#include "bropen_types.h"

#include <sane/sane.h>

void bropen_sane_cancel(SANE_Handle handle);
SANE_Status bropen_sane_read(SANE_Handle handle, SANE_Byte *buf, SANE_Int maxlen, SANE_Int *len, bropen_com_params_t *);

#endif /* _BROPEN_SANE_H_ */
