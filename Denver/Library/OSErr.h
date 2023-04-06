#pragma once

#include <Lib.h>

static inline void Hang(Void) { Asm ("int $23"); }
static inline void Throw(Void) { Asm ("int $22"); }

#ifndef __cplusplus
#define kthrow Throw()
#endif

typedef Int32 OSErr;