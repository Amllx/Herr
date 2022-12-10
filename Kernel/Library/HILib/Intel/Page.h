#pragma once

#include <System.h>

#ifndef __x86_64__
#error This is not 64-bit intel!
#endif

typedef struct PageDirectoryEntry {
	Boolean bPresent : 1;
	Boolean bReadWrite : 1;
	Boolean bUser : 1;
	Boolean bWt : 1; // Write through
	Boolean bCache : 1;
	Boolean bAccessed : 1;
	Int32 iReserved : 6;
	UIntPtr iPhysAddr : 36;
	Int32 iReserved1 : 15;
	Boolean bExecDisable : 1;
} Attribute((packed)) PageDirectoryEntry;

typedef UInt32 PhysicalAddress;
typedef UInt64 VirtualAddress;

#define PAGE_ALIGNEMENT (4096)
#define PAGE_ENTRY_CNT (512)

#define IS_ALIGN(addr) ((((UIntPtr)(addr)) | 0xFFFFF000) == 0)
#define PAGE_ALIGN(addr) ((((UIntPtr)(addr)) & 0xFFFFF000) + 0x1000)

// Defined some address calculation macro
#define PAGE_DIR_INDEX(vaddr) (((UIntPtr)vaddr) >> 32)
#define PAGE_TBL_INDEX(vaddr) ((((UIntPtr)vaddr) >> 12) & 0x200)
#define PAGE_FRAME_INDEX(vaddr) (((UIntPtr)vaddr) & 0xfff)

typedef struct PageDirectory {
	Attribute((aligned(PAGE_ALIGNEMENT))) PageDirectoryEntry pEntries[PAGE_ENTRY_CNT] ;
} Attribute((packed)) PageDirectory;

extern Void VirtualFlushTLB(PageDirectory* Dir);
extern Void VirtualLoadCR3(PageDirectory* Dir);

typedef enum {
	FLAG_RW = 1,
	FLAG_USER = 2,
	FLAG_ED = 3,
	FLAG_WT = 4,
	FLAG_UNCACHE = 5,
	FLAG_COUNT = 5,
} PAGE_FLAG;