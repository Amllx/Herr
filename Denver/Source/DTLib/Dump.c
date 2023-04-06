#include <StringUtils.h>
#include <MemLib/Alloc.h>
#include <DTLib/Dump.h>

#define PAGE_ERR_OK             (0x0)
#define PAGE_ERR_PRESENT        (0x1)
#define PAGE_ERR_RW             (0x2)
#define PAGE_ERR_USER           (0x4)
#define PAGE_ERR_RESERVED       (0x8)
#define PAGE_ERR_INST           (0x10)

MemoryDump* MemDump(UIntPtr address, UIntPtr rsp) {
	if (rsp == 0) return NULL;
	if (address == 0) return NULL;

	MemoryDump* dumped = MemAlloc(sizeof(MemoryDump));
	if (dumped == NULL) return NULL;

	dumped->address = address;
	dumped->stackFrame = (StackFrame*)rsp;

	UInt8 list[5] = { PAGE_ERR_PRESENT, PAGE_ERR_RESERVED, PAGE_ERR_USER, PAGE_ERR_RW, PAGE_ERR_INST };

	for (UInt8 index = 0; index < 5; ++index) {
		if (address & list[index]) {
			dumped->err |= list[index]; break;
		} /* Check for any page faults */
	}

	MemDumpStackInternal(rsp);
	return dumped;
}

#include <GraphicsLib/Terminal.h>

extern UIntPtr MemDumpStackInternal(UIntPtr rsp) {
	StackFrame* stackFrame = (StackFrame*)rsp;

	ConsoleLog("%s %x %n", "RSP: ", stackFrame->rsp);
	ConsoleLog("%s %x %n", "RBP: ", stackFrame->rbp);
	ConsoleLog("%s %x %n", "SIZE: ", STACK_SIZE);

	return rsp;	
}