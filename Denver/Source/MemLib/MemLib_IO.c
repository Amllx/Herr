#include <MemLib/IO.h>

VoidPtr 
MemAllocIO(UIntPtr off, MMIO_PAGE isPageProtected)
{
    if (off == 0) return NULL;
    VoidPtr ptr = (VoidPtr)(off);

    if (isPageProtected)
        VirtualMemoryMapEx(ptr, ptr, FLAG_RW | FLAG_UNCACHE | FLAG_ED);
    
    return ptr;
}