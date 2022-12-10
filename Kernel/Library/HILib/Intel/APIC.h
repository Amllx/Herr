#pragma once

#include <System.h>
#include <HILib/Intel/Cpu.h>
#include <IOLib/ACPI.h>
#include <Prekernel/EKBoot.h>

#define APIC_IDENT "APIC"

typedef struct APICHeader {
    Char signature[4];
    UInt32 length;
    UInt8 revision;
    UInt8 crc32;
    Char oemId[6];
    Char oemTableId[8];

    UInt32 oemRevision;
    UInt32 creatorId;
    UInt32 creatorRevision;
    UInt32 apicAddress;
    UInt32 apicFlags;
    UInt8 entryType;
    UInt32 recordLength;
} Attribute((packed)) APICHeader;

Boolean ApicIsDetected(void);
APICHeader* ApicGetMadt(BootloaderHeader* pBootHdr);

typedef enum {
    LOCAL_APIC_TYPE,
    IO_APIC_TYPE,
    TYPE_64BIT_APIC,
    APIC_TYPE_COUNT,
} APIC_TYPE;
