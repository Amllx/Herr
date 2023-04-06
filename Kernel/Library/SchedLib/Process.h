#pragma once

#include <Lib.h>
#include <System.h>

#define SCHED_PROCESS_NAME_LEN (256)
#define SCHED_PROCESS_STACK_LEN (4096U)

typedef UInt32 ProgramID;

typedef struct ProcessSelector
{
	Boolean ProcDebuggable; // has debug symbols?
	Boolean ProcSigned; // is this signed
	UInt16 ProcType;
	UInt16 CodeLevel; // Code level
    ProgramID ProgramId;
	UIntPtr PhysicalAddress; // Process's physical address, where to jump
	UIntPtr VirtualAddress; // Process Virtual Address, where to jump
	Char Stack[SCHED_PROCESS_STACK_LEN];
} Attribute((packed)) ProcessSelector;

enum PROC_CPL {
	PROC_CPL_USER = 3,
	PROC_CPL_DRIVER = 2,
	PROC_CPL_SYSTEM = 1,
	PROC_CPL_KERNEL = 0xff,
};

#define PROC_CPL_COUNT (4)

enum PROC_TYPE {
	PROC_TYPE_EXEC = 1,
	PROC_TYPE_LIB = 2,
	PROC_TYPE_SERVICE = 4,
};

#define PROC_TYPE_COUNT (PROC_TYPE_SERVICE + 1)

struct Attribute((packed)) ProcessHeader
        {
	UInt16 Name[SCHED_PROCESS_NAME_LEN];
    struct ProcessSelector Selector;
    VoidPtr HeapPtr;
    UInt64 HeapSize;
    UInt64 Status;
    UInt64 Type;
};

