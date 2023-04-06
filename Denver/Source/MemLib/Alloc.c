#include <GraphicsLib/Terminal.h>
#include <MemLib/Alloc.h>
#include <HILib/Intel/Page.h>
#include <StringUtils.h>
#include <DTLib/Result.h>

static Boolean kAllocationEnabled = False;
static MemBlk* kHighestAddress = NULL;
static MemBlk* kBaseAddress = NULL;
static SizeT kMemorySize = 0;

Boolean MemEnabled(void)
{
	return kAllocationEnabled;
}

VoidPtr MemStart(Void)
{
	return kBaseAddress;
}

VoidPtr MemEnd(Void)
{
	return kHighestAddress;
}

Boolean MemInit(BootloaderHeader* bootHeader)
{
    if (kAllocationEnabled)
        return True;

	struct TagMemmap* entries = BootloaderTag(bootHeader, EKBOOT_STRUCT_TAG_MEM_ID);

	for (SizeT i = 0; i < entries->entries; i++)
	{
		if (entries->memmap[i].type == EKBOOT_MEM_USABLE)
		{
			if (IsNull(kBaseAddress))
			{
				kBaseAddress = (VoidPtr)(entries->memmap[i].base);
				kBaseAddress->Next = NULL;

				continue;
			} else if (IsNull(kBaseAddress))
			{
				kHighestAddress = (VoidPtr)(entries->memmap[i].base);
				kHighestAddress->Next = NULL;

				continue;
			}

			break;
		}
	}

	kMemorySize = ( -((UIntPtr)&kBaseAddress - (UIntPtr)&kHighestAddress)) * sizeof(struct MemBlk);

	kHighestAddress->Prev = kBaseAddress;
    kHighestAddress->Next = kBaseAddress;

	kBaseAddress->Prev = kHighestAddress;

    kAllocationEnabled = True;

	for (SizeT i = 0; i < MEM_MAX_HEADERS; ++i){
		kBaseAddress->Index[i].Used = False;
		kHighestAddress->Index[i].Used = False;
		kBaseAddress->Index[i].Magic = MEM_MAGIC;
		kHighestAddress->Index[i].Magic = MEM_MAGIC;
	}

	ConsoleLog("%s %x %n", "Memory Size: ", kMemorySize);
	ConsoleLog("%s %x %s %x %n", "From: ", (UIntPtr)&kBaseAddress, " To: ", (UIntPtr)&kHighestAddress);

    return True;
}

/*
     Name: AllocLock
     Purpose: Allocation locker.
*/

typedef struct AllocLock {
	Boolean Locked;
	SizeT LastLockedSize;
	UIntPtr LastLockedAddr;
} Attribute((packed)) AllocLock;

static struct AllocLock gAllocLock = {
		.LastLockedAddr = 0,
		.LastLockedSize = 0,
		.Locked = False,
};

static Void MemInitAllocationList(MemBlk* block) {
	Check(NotNull(block), "Block is NULL! (MemInitAllocationList)");

	if (block->Index[0].Magic != MEM_MAGIC) {
		for (SizeT i = 0; i < MEM_MAX_HEADERS; ++i){
			Check(!block->Index[i].Used, "Critical Issue detected in the allocator! Entry is Used, while being set the Magic bit.");
			block->Index[i].Magic = MEM_MAGIC;
		}
	}
}

/*
 *  Reserves a VirtualAddress according an offset.
 *  Where offset == block -> true
 *  Returns null if no free headers were found.
 *  Also returns null if the Index pointer is not valid.
 */

static
VoidPtr 
MemReserveBlock(MemBlk* block, SizeT index, SizeT Size) {
	if (gAllocLock.Locked)
        return NULL;

	if (block->Index[index].Size > (KIB * 4)) {
        block->Index[index].Used = True;
        Result = ERR_BAD_ACCESS;
        return NULL;
    }

    if (Size > MEM_MAX_SZ) return NULL;

	gAllocLock.Locked = True;

	block->Index[index].VirtualAddress = (VoidPtr)(((UIntPtr)block + (UIntPtr)kBaseAddress) / PAGE_ENTRY_CNT);
	block->Index[index].Magic = MEM_MAGIC;
	block->Index[index].Size += Size;
	block->Index[index].Used = True;

	gAllocLock.LastLockedAddr = (UIntPtr)(block->Index[index].VirtualAddress);
	gAllocLock.LastLockedSize = Size;

	gAllocLock.Locked = False;

	Result = ERR_SUCCESS;

	return block->Index[index].VirtualAddress;
}

static VoidPtr MemAllocBlock(MemBlk* block, SizeT Size) {
    Result = MEM_NOT_ENABLED;
    if (!kAllocationEnabled) return NULL;

    Result = ERR_NULL;
    if (block == NULL) return NULL;

	SizeT index = 0;

	SizeT indexLeft = 0;
	SizeT indexRight = MEM_MAX_HEADERS;

	while (indexLeft < indexRight) {
			VoidPtr result = MemReserveBlock(block, index, Size);

			if (result != NULL) {
				Result = ERR_SUCCESS;
				return result;
			} else {
				if (index == indexLeft) {
					++indexLeft;
					index = indexRight;
				} else {
					--indexRight;
					index = indexLeft;
				}
			}
	}

    Result = MEM_UNAVAILABLE; // we are out of free allocation table
    return NULL;
}

static MemBlk* MemAllocNextBlock(MemBlk* block) {
    if (block == NULL) {
		ConsoleLog("!!!!OUT OF MEMORY!!!!");

        Result = MEM_OUT_OF_MEMORY;
        return False;
    } // We are out of memory

    if (block->Next != NULL) {
        Result = ERR_NOT_NULL;
        return False;
    } // Skip this block

    // Or if empty, fill this block
    block->Next = block + sizeof(MemBlk);
    block->Next->Prev = block;

    block = block->Next;

    Result = ERR_SUCCESS;

    return block;
}

static Void MemExpandBlock(MemBlk* Current) {
	if ((Current = MemAllocNextBlock(Current)) == NULL) {
		Int32 tmp = Result; // stock the Result

		switch (tmp) { // Fetch why it failed.
			case (MEM_OUT_OF_MEMORY): {
				ConsoleLog("Out Of Memory!");
				Asm ("int $25");
				break;
			} default:
				Current = Current->Next;
				break;

		}
	}
}

VoidPtr MemAlloc(SizeT Size) {
    Result = ERR_FAILURE;
    if (!kAllocationEnabled) return NULL;

	Result = MEM_INVALID_SIZE;
    if (Size < 1) return NULL;

	VoidPtr allocatedBlock = NULL;
	MemBlk* currentBlock = kBaseAddress;
	MemInitAllocationList(currentBlock);

	// seek and obtain strategy.
	while (allocatedBlock == NULL) {
		for (SizeT i = 0; i < MEM_MAX_HEADERS; ++i) {
			Check(currentBlock->Index[i].Magic == MEM_MAGIC, "Possible memory corruption detected in the kernel heap! Aborting system...");
		}
		
		allocatedBlock = MemAllocBlock(currentBlock, Size);
		if (Result == MEM_UNAVAILABLE) MemExpandBlock(allocatedBlock);
	}

	// ConsoleLog(Ternary(NotNull(currentBlock), "%s, %x, %n ", "%s, %n") , Ternary(NotNull(currentBlock), "Allocated Block at: ", (VoidPtr)&currentBlock));
	return allocatedBlock;
}

VoidPtr MemResize(VoidPtr Pointer, SizeT iSize) {
	Result = MEM_BAD_ARG;

	if (!kAllocationEnabled) return NULL;
	if (Pointer == NULL) return NULL;
	if (iSize == 0) return NULL;

	MemBlk* currentBlock = kBaseAddress;

	while (currentBlock != NULL) {
		for (SizeT index = 0; index < MEM_MAX_HEADERS; ++index) {
			if (currentBlock->Index[index].VirtualAddress != Pointer) continue;

			char* values = Pointer;
			char barrier = values[iSize];

			SizeT count = iSize;

			while (count) {
				if (values[count] != barrier)
					values[count] = 0;
				else
					break; // Return null, don't touch that

				--count;
			}

			currentBlock->Index[index].VirtualAddress = (VoidPtr)values;
			currentBlock->Index[index].Size += iSize;

			Result = ERR_SUCCESS;
			return currentBlock->Index[index].VirtualAddress;
		}

		currentBlock = currentBlock->Next;
	}

	Result = MEM_NOT_ENABLED;
	return NULL;
}

static Int32 MemFreeBlock(MemBlk* currentBlock, SizeT index, VoidPtr ptr) {
	if (!currentBlock->Index[index].Used) return -1;
	if (currentBlock->Index[index].VirtualAddress != ptr) return -1;

	SetMem(ptr, 0, StringLength(ptr));
	currentBlock->Index[index].Size -= StringLength(ptr);

	if (currentBlock->Index[index].Size < 1) currentBlock->Index[index].Used = False; // we're not using it anymore.

	return 0;
}

Int32 MemFree(VoidPtr Alloc) {
    Result = ERR_FAILURE;
    if (!kAllocationEnabled) return MEM_NOT_ENABLED;
	
	Result = ERR_NULL;
	if (Alloc == NULL) return MEM_BAD_ARG;

    MemBlk* currentBlock = kBaseAddress;

    while (currentBlock != NULL) {
        for (SizeT index = 0; index < MEM_MAX_HEADERS; ++index) {
           	if (MemFreeBlock(currentBlock, index, Alloc) == -1) continue;

			Result = ERR_SUCCESS;
			Alloc = NULL; // Alloc points now to an invalid VirtualAddress.
            return ERR_SUCCESS;
        }

        currentBlock = currentBlock->Next;
    }

    Result = MEM_UNAVAILABLE;
    return MEM_UNAVAILABLE; // Fatal! Check Result when -2 is being pushed on the stack.
}
