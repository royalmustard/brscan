#ifndef _BROPEN_DEBUG_H_
#define _BROPEN_DEBUG_H_

#include <assert.h>
#include <stdio.h>

#ifndef __FILE_NAME__
#define __FILE_NAME__ __FILE__
#endif

#define BROPEN_ASSERT(condition)                                                                                                                               \
    do                                                                                                                                                         \
    {                                                                                                                                                          \
        if (!(condition))                                                                                                                                      \
        {                                                                                                                                                      \
            printf("!!!! ensure failed (%s:%u) => [%s]\n", __FILE__, __LINE__, #condition);                                                                    \
            assert(0);                                                                                                                                         \
        }                                                                                                                                                      \
    } while (0)

#define BROPEN_PRINT(fmt, ...)                                                                                                                                                     \
    do                                                                                                                                                                             \
    {                                                                                                                                                                              \
        printf(fmt, ##__VA_ARGS__);                                                                                                                                                \
    } while (0)

#define BROPEN_PRINT_ERROR(fmt, ...) \
do \
{ \
    fprintf(stderr, "!!!! error: " fmt "\n", ##__VA_ARGS__); \
} while (0)

#define BROPEN_PRINT_DEBUG(category, fmt, ...)                                                                                                                                     \
    do                                                                                                                                                                             \
    {                                                                                                                                                                              \
        BROPEN_PRINT("[bropen::%s] %s - " fmt " (%s:%d)\n", category, __func__, ##__VA_ARGS__, __FILE_NAME__, __LINE__);                                                           \
    } while (0)

#define BROPEN_PRINT_DEBUG_BUFFER(category, id, buffer, buf_size, print_size)                                                                                        \
    do                                                                                                                                                         \
    {                                                                                                                                                          \
        uint32_t bropenprintbuffersize = buf_size > print_size ? print_size : buf_size;                                                                        \
        printf("[bropen::%s] -- ", category);                                                                                                                  \
        for (uint32_t i = 0; i < bropenprintbuffersize;)                                                                                                       \
        {                                                                                                                                                      \
            for (uint32_t j = i; j < i + 4 && j < bropenprintbuffersize; ++j)                                                                                  \
            {                                                                                                                                                  \
                printf("%2.2x", (unsigned char)buffer[j]);                                                                                                     \
            }                                                                                                                                                  \
            printf(" ");                                                                                                                                       \
            i += 4;                                                                                                                                            \
        }                                                                                                                                                      \
        printf(" [%s] [%p] (%s:%d)\n", id, buffer, __FILE__, __LINE__);                                                                                        \
        printf("[bropen::%s] -- ", category);                                                                                                                  \
        for (uint32_t i = 0; i < bropenprintbuffersize; ++i)                                                                                                   \
        {                                                                                                                                                      \
            char c = buffer[i];                                                                                                                                \
            if (c == '\n')                                                                                                                                     \
            {                                                                                                                                                  \
                printf("\\n");                                                                                                                                 \
            }                                                                                                                                                  \
            else if ((c >= 'A' && c <= 'Z') || c == ' ' || c == '=' || c == ',' || (c >= '0' && c <= '9'))                                                     \
            {                                                                                                                                                  \
                printf("%c", c);                                                                                                                               \
            }                                                                                                                                                  \
            else                                                                                                                                               \
            {                                                                                                                                                  \
                printf("?");                                                                                                                                   \
            }                                                                                                                                                  \
        }                                                                                                                                                      \
        printf(" [%s] [%p] (%s:%d)\n", id, buffer, __FILE__, __LINE__);                                                                                        \
    } while (0)

#define BROPEN_PRINT_USER_ERROR(fmt, ...) \
do \
{\
    fprintf(stderr, fmt "\n", ##__VA_ARGS__); \
}\
while (0)

#define BROPEN_PRINT_USER_INFO(fmt, ...) \
do \
{\
    fprintf(stdout, fmt "\n", ##__VA_ARGS__); \
}\
while (0)

#define BROPEN_PRINT_INTERNAL BROPEN_PRINT_ERROR

#ifndef BROPEN_G_DEBUG_ALL
#define BROPEN_G_DEBUG_ALL 0
#endif

#ifndef BROPEN_G_DEBUG_COM
#define BROPEN_G_DEBUG_COM 0
#endif

#define BROPEN_DEBUG_COM(fmt, ...)                                                                                                                             \
    do                                                                                                                                                         \
    {                                                                                                                                                          \
        if (BROPEN_G_DEBUG_COM || BROPEN_G_DEBUG_ALL)                                                                                                                                \
        {                                                                                                                                                      \
            BROPEN_PRINT_DEBUG("com", fmt, ##__VA_ARGS__);                                                                                                                  \
        }                                                                                                                                                      \
    } while (0)

#define BROPEN_DEBUG_COM_BUFFER(id, buffer, buffer_size, print_size)                                                                                                  \
    do                                                                                                                                                         \
    {                                                                                                                                                          \
        if (BROPEN_G_DEBUG_COM|| BROPEN_G_DEBUG_ALL)                                                                                                                                \
        {                                                                                                                                                      \
            BROPEN_PRINT_DEBUG_BUFFER("com", id, buffer, buffer_size, print_size);                                                                                          \
        }                                                                                                                                                      \
    } while (0)

#ifndef BROPEN_G_DEBUG_DEC
#define BROPEN_G_DEBUG_DEC 0
#endif

#define BROPEN_DEBUG_DEC(fmt, ...)                                                                                                                             \
    do                                                                                                                                                         \
    {                                                                                                                                                          \
        if (BROPEN_G_DEBUG_DEC || BROPEN_G_DEBUG_ALL)                                                                                                                                \
        {                                                                                                                                                      \
            BROPEN_PRINT_DEBUG("dec", fmt, ##__VA_ARGS__);                                                                                                                  \
        }                                                                                                                                                      \
    } while (0)

#define BROPEN_DEBUG_DEC_BUFFER(id, buf, buf_size, print_size)                                                                                                 \
    do                                                                                                                                                         \
    {                                                                                                                                                          \
        if (BROPEN_G_DEBUG_DEC || BROPEN_G_DEBUG_ALL)                                                                                                                                \
        {                                                                                                                                                      \
            BROPEN_PRINT_DEBUG_BUFFER("dec", id, buf, buf_size, print_size);                                                                                         \
        }                                                                                                                                                      \
    } while (0)

#ifndef BROPEN_G_DEBUG_USB
#define BROPEN_G_DEBUG_USB 0
#endif

#define BROPEN_DEBUG_USB(fmt, ...)                                                                                                                             \
    do                                                                                                                                                         \
    {                                                                                                                                                          \
        if (BROPEN_G_DEBUG_USB || BROPEN_G_DEBUG_ALL)                                                                                                                                \
        {                                                                                                                                                      \
            BROPEN_PRINT_DEBUG("usb", fmt, ##__VA_ARGS__);                                                                                                           \
        }                                                                                                                                                      \
    } while (0)

#ifndef BROPEN_G_DEBUG_SANE
#define BROPEN_G_DEBUG_SANE 0
#endif

#define BROPEN_DEBUG_SANE(fmt, ...)                                                                                                                            \
    do                                                                                                                                                         \
    {                                                                                                                                                          \
        if (BROPEN_G_DEBUG_SANE || BROPEN_G_DEBUG_ALL)                                                                                                                               \
        {                                                                                                                                                      \
            BROPEN_PRINT_DEBUG("sane", fmt, ##__VA_ARGS__);                                                                                                          \
        }                                                                                                                                                      \
    } while (0)

#define BROPEN_DEBUG_SANE_BUFFER(id, buf, buf_size, print_size)                                                                                                \
    do                                                                                                                                                         \
    {                                                                                                                                                          \
        if (BROPEN_G_DEBUG_SANE|| BROPEN_G_DEBUG_ALL)                                                                                                                               \
        {                                                                                                                                                      \
            BROPEN_PRINT_DEBUG_BUFFER("sane", id, buf, buf_size, print_size);                                                                                        \
        }                                                                                                                                                      \
    } while (0)

#ifndef BROPEN_G_DEBUG_SCAN
#define BROPEN_G_DEBUG_SCAN 0
#endif

#define BROPEN_DEBUG_SCAN(fmt, ...)                                                                                                                            \
    do                                                                                                                                                         \
    {                                                                                                                                                          \
        if (BROPEN_G_DEBUG_SCAN || BROPEN_G_DEBUG_ALL)                                                                                                                               \
        {                                                                                                                                                      \
            BROPEN_PRINT_DEBUG("scan", fmt, ##__VA_ARGS__);                                                                                                                 \
        }                                                                                                                                                      \
    } while (0)

#define BROPEN_DEBUG_SCAN_BUFFER(id, buf, buf_size, print_size)                                                                                                \
    do                                                                                                                                                         \
    {                                                                                                                                                          \
        if (BROPEN_G_DEBUG_SCAN || BROPEN_G_DEBUG_ALL)                                                                                                                               \
        {                                                                                                                                                      \
            BROPEN_PRINT_DEBUG_BUFFER("scan", id, buf, buf_size, print_size);                                                                                        \
        }                                                                                                                                                      \
    } while (0)

#endif //_BROPEN_DEBUG_H_