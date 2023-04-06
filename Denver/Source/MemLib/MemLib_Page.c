#include <GraphicsLib/Terminal.h>
#include <MemLib/Page.h>
#include <MemLib/Alloc.h>

Boolean VirtualIsRw(PageDirectoryEntry* pte)
{
	return pte->bReadWrite;
}

Boolean VirtualIsPresent(PageDirectoryEntry* pte)
{ 
	return pte->bPresent;
}

PageDirectory* VirtualCreateMemoryEx(VoidPtr vaddr, UInt32 flags)
{
    if (vaddr == NULL) return NULL;

	PageDirectory* page = (PageDirectory*)(vaddr);

	page->pEntries[0].bPresent = True;
	page->pEntries[0].bReadWrite = ((flags & FLAG_RW) == FLAG_RW);
	page->pEntries[0].bWt = ((flags & FLAG_WT) == FLAG_WT);
	page->pEntries[0].bUser = ((flags & FLAG_USER) == FLAG_USER);
	page->pEntries[0].bExecDisable = ((flags & FLAG_ED) == FLAG_ED);
	page->pEntries[0].bCache = ((flags & FLAG_UNCACHE) != FLAG_UNCACHE);
	
	return page;
}

VoidPtr 
VirtualMemoryMapEx(PageDirectory* page, VoidPtr paddr, UInt32 flags)
{
	for (SizeT i = 0; i < PAGE_ENTRY_CNT; i++)
	{
		if (!page->pEntries[i].bPresent) {
			page->pEntries[i].iPhysAddr = (((UIntPtr)paddr << 12) + PAGE_FRAME_INDEX(paddr)) >> 12;
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
