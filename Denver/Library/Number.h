#pragma once

#ifndef __x86_64__
#error Bad Architecture
#endif

typedef unsigned UInt32;
typedef unsigned short UInt16;
typedef unsigned char UInt8;

typedef int Int32;
typedef short Int16;
typedef char Char;
typedef Char Int8;

typedef unsigned long int UInt64;
typedef long int Int64;

typedef Int32 Number;
typedef Int64 LongNumber;