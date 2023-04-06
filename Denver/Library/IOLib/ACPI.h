#pragma once

#include <Lib.h>

typedef struct SDT {
	Char signature[4];
	UInt32 length;
	Char revision;
	Char checksum;
	Char oemId[6];
	Char oemTableId[8];
	UInt32 oemRev;
	UInt32 creatorID;
	UInt32 creatorRevision;
} Attribute((packed)) SDT;

typedef struct RSDP {
	Char Signature[8];
	UInt8 Checksum;
	Char OEMID[6];
	UInt8 Revision;
	UInt32 RsdtAddress;

#ifdef __x86_64__
	UInt32 Length;
	UInt64 XsdtAddress;
	UInt8 ExtendedChecksum;
	UInt8 reserved[3];
#endif
} Attribute((packed)) RSDP;

typedef struct ACPIConfigHeader {
	UIntPtr BaseAddress;
	UInt16  GroupNumber;
	UInt8   StartBus;
	UInt8   EndBus;
	UInt32  Reserved;
} Attribute((packed)) ACPIConfigHeader, *AcpiConfigHeaderPtr;

typedef struct ACPIConfigSpace {
	UInt8 AddressSpaceId;
	UInt8 RegisterBitWidth;
	UInt8 RegisterBitOffset;
	UInt8 Reserved;
	UIntPtr Address;
} Attribute((packed)) ACPIConfigSpace;

typedef enum {
	ADDRESS_SPACE_SYSTEM_MEMORY = 0,
	ADDRESS_SPACE_SYSTEM_IO = 1,
	ADDRESS_SPACE_COUNT,
} ADDRESS_SPACE;

Boolean 
AcpiChecksum(SDT* tableHeader);

VoidPtr 
AcpiFind(RSDP* rsdt, Char* signature);

RSDP* 
AcpiGetRsdp(BootloaderHeader * pBootHdr);
