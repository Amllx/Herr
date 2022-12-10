#pragma once

#define SetPageFlag(PDE, OFFSET, ENABLE) (PDE->OFFSET = ENABLE)
#define VirtualCreateMemory(ADDR) VirtualCreateMemoryEx(ADDR, 0)
#define VirtualMemoryMap(DIR, ADDR) VirtualMemoryMapEx(DIR, ADDR, 0)

#ifndef VirtualGetPhysicalAddress
#define VirtualGetPhysicalAddress(ADDR)\
    ADDR >>= 12; ADDR &= 0x1ff;\



#endif

