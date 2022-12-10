#pragma once

typedef __SIZE_TYPE__ SizeT;
typedef SizeT UIntPtr;
typedef UIntPtr PtrDiff;
typedef void* VoidPtr;
typedef void Void;

#ifndef NULL
#define NULL ((VoidPtr)0)
#endif // NULL

#ifdef __NEED_PRODUCTION__
#define __PRODUCTION__ (1)
#endif

#define Ternary(BOOL, YES, NO) ((BOOL) ? (YES) : (NO))

#define KIB 1024
#define MIB (1024 * KIB)
#define GIB (1024 * MIB)

#define __Denver__ (202206)

#define NanoSeconds (1000)
#define MilliSeconds (NanoSeconds * 1000)
#define Seconds (MilliSeconds * 1000)

#define OffsetOf(STRUCT, FIELD) __builtin_offsetof(STRUCT, FIELD)
#define NoRet __attribute__((noreturn))
#define Attribute(Attr) __attribute__(Attr)

#define Assert(BOOL) _Static_assert(BOOL, "")

#define IsNull(x) (x == NULL)
#define NotNull(x) (x != NULL)

#define STACK_SIZE (8096)

#ifdef __i386__
#define Split64(NAME) \
    union {                    \
        UInt32 NAME;         \
        UInt32 NAME##Low;    \
    };                         \
    UInt32 NAME##High
#elif defined(__x86_64__)
#define Split64(NAME) \
    UInt64 NAME
#else
#error You cannot cross compile for ARM, it is not our focus
#endif

extern UIntPtr MEM_KERNEL_BASE;