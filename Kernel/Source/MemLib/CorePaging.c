
#include <GraphicsLib/Terminal.h>
#include <MemLib/CorePaging.h>
#include <MemLib/CoreAlloc.h>
#include <DT/Result.h>

VoidPtr gPageBase = NULL;
Boolean gPageEnabled = False;

Boolean
VirtualEnablePaging(Void)
{
	Check(!gPageEnabled, "Virtual Memory is already enabled!");
	gPageBase = (VoidPtr)MEM_KERNEL_BASE;
	gPageEnabled = True;

	return True;
}

PageDirectory* 
VirtualAllocatePageBytes(VoidPtr Base) 
{
	Check((UIntPtr) Base < (MEM_OFFSET_BEGIN + MEM_KERNEL_BASE), "[VirtualAllocatePageBytes] Too high.");
	return (PageDirectory*)(Base);
}

Boolean 
VirtualIsRw(PageDirectoryEntry* pPte) 
{
	return pPte->bReadWrite; 
}

Boolean 
VirtualIsPresent(PageDirectoryEntry* pPte) 
{ 
	return pPte->bPresent; 
}

PageDirectory* 
VirtualCreateMemoryEx(VoidPtr vaddr, UInt32 flags) 
{
	PageDirectory* page = (PageDirectory*)(MemEnabled() ? VirtualAllocatePageBytes(gPageBase) : MemAlloc(sizeof(PageDirectory)));
	gPageBase = (VoidPtr)((UIntPtr)page + sizeof(PageDirectory));

	if (!page) return NULL;

	page->pEntries[0].bPresent = True;
	page->pEntries[0].iPhysAddr = (UIntPtr)vaddr >> 12; 
	page->pEntries[0].bReadWrite = ((flags & FLAG_RW) == FLAG_RW);
	page->pEntries[0].bWt = ((flags & FLAG_WT) == FLAG_WT);
	page->pEntries[0].bUser = ((flags & FLAG_USER) == FLAG_USER);
	page->pEntries[0].bExecDisable = ((flags & FLAG_ED) == FLAG_ED);
	page->pEntries[0].bCache = ((flags & FLAG_UNCACHE) != FLAG_UNCACHE);
	
	return page;
}

VoidPtr 
VirtualMemoryMapEx(PageDirectory* page, VoidPtr vaddr, UInt32 flags) 
{
	for (SizeT i = 0; i < PAGE_ENTRY_CNT; i++)
	{
		if (!page->pEntries[i].bPresent) {
			page->pEntries[i].iPhysAddr = (((UIntPtr)vaddr << 12) + PAGE_FRAME_INDEX(vaddr)) >> 12;
			page->pEntries[i].bUser = ((flags & FLAG_USER) == FLAG_USER);
			page->pEntries[i].bReadWrite = ((flags & FLAG_RW) == FLAG_RW);
			page->pEntries[i].bWt = ((flags & FLAG_WT) == FLAG_WT);
			page->pEntries[i].bExecDisable = ((flags & FLAG_ED) == FLAG_ED);
			page->pEntries[i].bCache = ((flags & FLAG_UNCACHE) != FLAG_UNCACHE);
			
			return &page->pEntries[i];
		}
	}
	
	return NULL;
}
