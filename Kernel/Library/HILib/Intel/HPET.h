#pragma once

#include <Prekernel/EKBoot.h>
#include <HILib/Intel/Cpu.h>
#include <IOLib/ACPI.h>

#define CLOCK_HPET_NOT_PRESENT (17)
#define CLOCK_HPET_PRESENT (18)
#define CLOCK_HPET_ENABLED (19)
#define CLOCK_HPET_BAD_PERIOD (20)
#define CLOCK_HPET_SYS_MEM (0)
#define CLOCK_HPET_SYS_IO (1)
#define CLOCK_HPET_SIGNATURE "HPET"

typedef struct HPETClockHeader {
    Char signature[4]; // 'HPET' in case of HPET table
    UInt32 length;
    UInt8 revision;
    UInt8 checksum;
    Char oemid[6];
    UInt64 oemTableid;
    UInt32 oem_Revision;
    UInt32 creatorId;
    UInt32 creatorRevision;
    UInt32 eventTimerBlockId;
    UInt32 baseAddress;
    UInt8  hpetNumber;
    UInt16 mainCounterMin;
} __attribute__((packed)) HPETClockHeader;

#define CLOCK_HPET_LEGACY_REPLACEMENT (0x3F)

typedef SizeT ClockPeriod;

Boolean HPETClockCreate(BootloaderHeader* bs);
Boolean HPETClockEnabled(void);

typedef void(*OnFinish)(Int32 iArgs, VoidPtr* Args);
void HPETSleep(ClockPeriod iDelay);

