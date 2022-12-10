#include <MemLib/CoreMMIO.h>

VoidPtr 
MemAllocIO(UIntPtr iOff, MMIO_PAGE isPageProtected)
{
    if (iOff == 0) return NULL;
    VoidPtr ptr = (VoidPtr)(iOff);
    if (isPageProtected)
        VirtualMemoryMapEx(ptr, ptr, FLAG_RW | FLAG_UNCACHE | FLAG_ED);
    
    return ptr;
}