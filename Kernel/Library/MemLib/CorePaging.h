#pragma once

#ifdef __x86_64__
#include <HILib/Intel/Page.h>
#else
#error Unsupported arch.
#endif

#ifndef __NO_PAGING_MACROS__
#include <MemLib/CorePagingMacros.h>
#endif

#define PAGE_ADDR_NULL 	(-1)
#define PAGE_DIR_NULL 	(-2)
#define PAGE_NOT_FOUND 	(-3)
#define PAGE_DIR_LOCKED (-4)

Boolean VirtualIsRw(PageDirectoryEntry* pPte);
Boolean VirtualIsPresent(PageDirectoryEntry* pPte);

Boolean
VirtualEnablePaging(Void);

PageDirectory* 
VirtualCreateMemoryEx(VoidPtr Address, UInt32 flags);

VoidPtr 
VirtualMemoryMapEx(PageDirectory* page, VoidPtr Address, UInt32 flags);

#define MEM_OFFSET_BEGIN ((UIntPtr)8 * MIB)
#define MEM_FOOTER_END (48 * MIB)

PageDirectory*
VirtualAllocatePageBytes(VoidPtr Base);