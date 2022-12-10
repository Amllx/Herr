#ifndef SYS_ENDIAN_H
#define SYS_ENDIAN_H

#include <System.h>

typedef enum {
    LITTLE_ENDIAN,
    BIG_ENDIAN,
    ENDIAN_COUNT
} ENDIAN;

#define ENDIAN(address, value) ((Char*)address[0] == value) ? BIG_ENDIAN : LITTLE_ENDIAN

#endif /* SYS_ENDIAN_H */