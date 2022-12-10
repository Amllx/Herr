#pragma once

#include <Lib.h>
#include <SchedLib/ExecSignature.h>

#define HPOS_PROCESS_NAME_LEN (256)

typedef struct ProcessSelector {
	Boolean ProcDebuggable; // has debug symbols?
	Boolean ProcSigned; // is this signed
	UInt16 ProcType;
	UInt16 CodeLevel; // Code level
	UInt32 ProgramId;
	UIntPtr PhysicalAddress; // Proc's physical address, where to jump
	UIntPtr VirtualAddress; // Proc Virtual Address, where to jump
	UIntPtr StackPhysicalAddress; // Stack's physical address, where to place it.
	UIntPtr StackVirtualAddress; // Stack Virtual Address, where to place it.
	UIntPtr MemUsage; // Proc Memory usage
} Attribute((packed)) ProcessSelector;

enum PROC_CPL {
	PROC_CPL_USER = 3,
	PROC_CPL_DRIVER = 2,
	PROC_CPL_SYSTEM = 1,
	PROC_CPL_KERNEL = 0xff,
};

enum PROC_TYPE {
	PROC_TYPE_EXEC = 1,
	PROC_TYPE_RTLIB = 2,
	PROC_TYPE_SERVICE = 4,
};

struct Attribute((packed)) ProcessHeader {
	UInt16 Name[HPOS_PROCESS_NAME_LEN];
	UInt64 ProcessId;
	UInt64 Status;
	UInt64 Type;
	VoidPtr Threads;
	UInt64 NumThreads;
	UInt64 NumAllocations;
	VoidPtr AllocatedLinkedList;
	struct ProcessSelector* Selector;
};

Int32 SchedCurrentPid(Void);
