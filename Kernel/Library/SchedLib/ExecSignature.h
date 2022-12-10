#pragma once

#include <System.h>

typedef struct Attribute((packed)) __ExecSignature__ {
	UInt64 iCreatedAt;
	UInt64 iExpiresAt;
	UInt32 iChecksum;
} ExecSignature;

