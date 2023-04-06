/*
 *	========================================================
 *
 *	DENVER
 * 	Date Added: 08/01/2023
 * 	Copyright XPX Technologies, all rights reserved.
 *
 * 	========================================================
 */

#pragma once

#include <Prekernel/EKBoot.h>
#include <Number.h>
#include <Bool.h>
#include <Defs.h>

// memory allocator error ids.
#define MEM_BAD_IDENT (-2)
#define MEM_NOT_ENABLED (-3)
#define MEM_INVALID_SIZE (-4)
#define MEM_POSSIBLE_OVERFLOW (-5)
#define MEM_UNAVAILABLE (-6)
#define MEM_OUT_OF_MEMORY (-7)
#define MEM_BAD_ARG (-9)

#define MEM_MAGIC       (0x225533)
#define MEM_MAX_HEADERS (128U)

typedef struct BlkHdr {
    Int32 Magic;
    UInt8 Used;
    SizeT Size;
	VoidPtr VirtualAddress;
} Attribute((packed)) BlkHdr;

typedef struct MemBlk {
    BlkHdr Index[MEM_MAX_HEADERS];
    struct MemBlk* Prev;
    struct MemBlk* Next;
} Attribute((packed)) MemBlk;

Boolean MemInit(BootloaderHeader* bootHeader);
VoidPtr MemResize(VoidPtr pointer, SizeT sz);
Int32   MemFree(VoidPtr pointer);
VoidPtr MemAlloc(SizeT size);
Boolean MemEnabled(void);
VoidPtr MemStart(Void);
VoidPtr MemEnd(Void);

#define MEM_MAX_SZ (4096U)