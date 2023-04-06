#pragma once

#include <Lib.h>

#define SMP_ProcessorName(INT) "CPU_" #INT

typedef struct ProcessorInfo {
	UInt16 strCpuId[16]; 			// Cpu id? (0, 1, 2, 3) alongside (CPU_*)
	UInt16 iPriority; 				// Should we give more jobs to this guy??
	UIntPtr GotoAddress; 		// Local APIC address
	UIntPtr ProcessorStruct; // Optional processor struct
	Boolean bIsBootProcessor; // Is boot processor?
} Attribute((packed)) ProcessorInfo;
				 				
typedef enum { 
	PRIORITY_FREE,// CPU is not busy at all
	PRIORITY_CAN_TRY, // cpu can still be used
	PRIORITY_BUSY, // cpu is very busy
	PRIORITY_COUNT 
} DENVER_PROCESSOR_PRIORITY;

Int32 SmpProbeApic(BootloaderHeader* pBootHdr);

struct ProcessorEntry {
    UInt8  iApicType;
    UInt64 LocalApic;       // pointer to the Local APIC MMIO registers
    UInt64 IoApic;      // pointer to the IO APIC MMIO registers
};

const UInt8 SmpNumCores(Void);
struct ProcessorEntry* SmpProcessorEntryGet(SizeT Index);