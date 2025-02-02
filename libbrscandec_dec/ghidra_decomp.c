#include <stdint.h>
#include <string.h>
__int128 ZEXT816(int64_t val)
{
    return (__int128)val;
}

__int128 ZEXT416(int32_t val)
{
    return (__int128)val;
}

int64_t SUB168(__int128 val1, int64_t n_lsb)
{
    return (int64_t)(val1 >> n_lsb);
}

__int128 CONCAT88(int64_t val1, int64_t val2)
{
    return (__int128)((__int128)val1 <<8 + (__int128)val2 );
}

__int128 movlpd(__int128 val1, double val2)
{
    __int128 tmp = 0;
    memcpy(&tmp, &val2, 8);
    tmp = tmp >> 8 + (val1 & 0xffffffff00000000);
    return tmp;
}
