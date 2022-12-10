#pragma once

#include <Lib.h>
#include <CRC32.h>
#include <IOLib/ACPI.h>

#define PCI_CONFIG_ADDRESS (0xCF8)
#define PCI_CONFIG_DATA (0xCFC)

#define PCI_DEVICE_COUNT (32)
#define PCI_FUNC_COUNT  (8)
#define PCI_BUS_COUNT  (255)

#define PCI_FAILURE (-1)
#define PCI_SUCCESS (250)
#define PCI_INVALID_VENDOR (251)
#define PCI_INVALID_DESCRIPTOR (252)
#define PCI_INVALID (0xFFFF)

#define PciCheck(TO_CHECK) (TO_CHECK != PCI_INVALID)
#define PciCrc32(HEADER) CRC32(HEADER->checksum, HEADER->length)

typedef struct PciDeviceHeader {
    UInt16 VendorId;
    UInt16 DeviceId;

    UInt8 Command;
    UInt8 Status;
    UInt8 RevisionId;

    UInt8 ProgIf;
    UInt8 SubClass;
    UInt8 Class;

    UInt8 CacheLineSz;
    UInt8 LatencyTimer;
    UInt8 HeaderType;
    UInt8 BIST; // built in self test register

	UInt8 Bus;
	UInt8 Device;
	UInt8 Function;
} PciDeviceHeader;

typedef struct PciBarHeader {
	UInt32 BAR1;
    UInt32 BAR2;
    UInt32 BAR3;
    UInt32 BAR4;
    UInt32 BAR5;
} PciBarHeader;

typedef struct PciInfoHeader {
    UInt32 CardCisStr;
    UInt16 SubsystemVendorId;
    UInt16 SubsystemId;
    UInt16 ExpansionRomBase;
    UInt8 CapsPtr;

    UInt8 Reserved;
    UInt16 Reserved1;
    UInt32 Reserved2;

    UInt8 InterruptLine;
    UInt8 InterruptPin;

    UInt8 MinGrant;
    UInt8 MaxLatency;
} Attribute((packed)) PciInfoHeader;

typedef struct MCFGHeader {
    Char Signature[4];
    UInt32 Length;
    Char  Rev;
    Char  ChkSum;
    Char OemId[4];
    Char OemTblId[8];
    UInt32 CreatorId;
    UInt32 CreatorRev;
    UInt64 Reserved;
} Attribute((packed)) MCFGHeader, *MCFGHeaderPtr;

MCFGHeader* PciGetMcfg(Void);
Boolean PciFindMcfg(BootloaderHeader* pBootHdr);
UInt32 PciReadCfg(UInt16 bus, UInt16 device, UInt8 function, UInt16 bar, SizeT size);
Void PciWriteCfg(UInt16 bus, UInt16 device, UInt8 function, UInt16 bar, SizeT data, SizeT size);
UInt32 PciInternalReadRaw(UInt16 bus, UInt16 device, UInt8 function, UInt16 bar);
UInt16 PciVendorId(UInt8 bus, UInt8 device, UInt8 function);
UInt16 PciInterfaceId(UInt8 bus, UInt8 device, UInt8 function);
UInt8 PciClass(UInt8 bus, UInt8 device, UInt8 function);
UInt8 PciSubclass(UInt8 bus, UInt8 device, UInt8 function);
UInt8 PciProgIf(UInt8 bus, UInt8 device, UInt8 function);
UInt8 PciHeaderType(UInt8 bus, UInt8 device, UInt8 function);
Void PciEnableMmio(UInt8 bus, UInt8 device, UInt8 function, UInt32 bar);
Void PciBecomeBusMaster(UInt16 bus, UInt16 device, UInt8 function, UInt32 bar);
UInt16 PciVendor(UInt16 bus, UInt16 device, UInt8 function);

