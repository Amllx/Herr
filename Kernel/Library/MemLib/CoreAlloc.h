#pragma once

#include <Prekernel/EKBoot.h>
#include <Number.h>
#include <Bool.h>
#include <Defs.h>

#define MEM_BAD_IDENT (-2)
#define MEM_NOT_ENABLED (-3)
#define MEM_INVALID_SIZE (-4)
#define MEM_POSSIBLE_OVERFLOW (-5)
#define MEM_UNAVAILABLE (-6)
#define MEM_OUT_OF_MEMORY (-7)
#define MEM_BAD_ARG (-9)

#define MEM_MAGIC (0xAA5577)

#define MEM_MAX_HEADERS (512U)

typedef struct AllocBlockHeader {
	VoidPtr VirtualAddress;
	Int32 Magic;
	UInt8 Used;
    SizeT Size;
} Attribute((packed)) AllocBlockHeader;

typedef struct AllocBlock {
    Attribute((aligned(16))) AllocBlockHeader Index[MEM_MAX_HEADERS];

    struct AllocBlock* Prev;
    struct AllocBlock* Next;
} Attribute((packed)) AllocBlock;

Boolean MemEnabled(void);
VoidPtr MemAlloc(SizeT size);
Int32   MemFree(VoidPtr pointer);
VoidPtr MemResize(VoidPtr pointer, SizeT sz);
Boolean MemInit(BootloaderHeader* bootHeader);
VoidPtr MemStart(Void);
VoidPtr MemEnd(Void);

#ifndef ToIOBaseAddress
#define ToIOBaseAddress(BAR) (BAR + MEM_IO_BASE)
#endif