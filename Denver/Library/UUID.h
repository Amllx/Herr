#pragma once

#include <System.h>

struct UUID {
	UInt32 a;
	UInt16 b;
	UInt16 c;
	UInt8 d[8];
};

typedef struct UUID GUID;
typedef struct UUID UUID;