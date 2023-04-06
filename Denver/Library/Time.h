#pragma once

#include <System.h>
#include <Prekernel/EKBoot.h>

typedef Int64 Epoch;

#define EPOCH_1970 (UInt64)0
#define EPOCH_MAX (UInt64)__SIZEOF_LONG__

Epoch BootloaderEpoch(BootloaderHeader* Header);