#pragma once

#include <MemLib/CorePaging.h>
#include <IOLib/ACPI.h>
#include <Defs.h>

#ifndef MEM_IO_BASE
#define MEM_IO_BASE ((UIntPtr)0xffff800000000000)
#endif

typedef enum {
    MMIO_PAGE_PROTECTED,
    MMIO_PAGE_UNPROTECTED,
} MMIO_PAGE;

VoidPtr 
MemAllocIO(UIntPtr iOff, MMIO_PAGE isPageProtected);