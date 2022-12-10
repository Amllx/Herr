#pragma once

#include <System.h>

typedef struct MemoryDump {
    Char name[128];
    UInt64 err; /* Error Code */
	UInt64 address; /* The Dumped Address */
    StackFrame* stackFrame; /* StackFrame to use */
} Attribute((packed)) MemoryDump;

extern Void DumpStack(Void);
extern UIntPtr MemDumpStackInternal(UIntPtr rsp);
MemoryDump* MemDump(UIntPtr address, UIntPtr rsp);
