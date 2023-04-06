#pragma once

#include <MemLib/CorePaging.h>
#include <IOLib/ACPI.h>
#include <Defs.h>

#ifndef MEM_IO_BASE
#define MEM_IO_BASE ((UIntPtr)0xffff800000000000)
#endif

typedef enum {
    MMIO_PAGE_PROTECT,
    MMIO_PAGE_NO_PROTECT,
} MMIO_PAGE;

#define MMIO_PAGE_PROTECT_CNT (MMIO_PAGE_NO_PROTECT + 1)

VoidPtr MemAllocIO(UIntPtr off, MMIO_PAGE isPageProtected);