#pragma once

#include <Lib.h>

#define GPT_PRIMARY_MBR_LBA (0)
#define GPT_PRIMARY_HEADER_LBA (1)
#define GPT_PRIMARY_ENTRIES_LBA (2)
#define GPT_PRIMARY_NUM_LBA_ENTRIES (32)

#define GPT_SECONDARY_MBR_LBA (-0)
#define GPT_SECONDARY_HEADER_LBA (-1)
#define GPT_SECONDARY_ENTRIES_LBA (-2)
#define GPT_SECONDARY_NUM_LBA_ENTRIES (-32)

// HDI flags.
#define HDI_DONT_NEED_COMPLEX_LAYOUT (1)
#define HDI_PRIVILEGED_DRIVE (0x5C)

#define HDIIsSuperDrive(PTR) (PTR->magic == HDI_PRIVILEGED_DRIVE)

struct HDIContext;
struct HDIVolume;

typedef Int32 HDIInt32;
typedef UInt32 HDILba; // virtual or real lba.
typedef Int64 HDIInt64;
typedef SizeT HDISize;
typedef UInt16 HDIChar;
typedef Int16 HDIInt16;

typedef UInt16* (*HDIReadT)(struct HDIContext* drive, HDILba lba, UInt8 sectorCount);
typedef Boolean (*HDIWriteT)(struct HDIContext* drive, HDILba lba, UInt8 sectorCount, UInt16* buf);

typedef Boolean (*HDIWriteExT)(UInt16* bytes, HDISize pos);
typedef UInt16* (*HDIReadExT)(HDISize pos, HDISize length);

typedef struct HDIContext {
    HDIChar strDriveFs[32]; // SATA-AHCI, IDE-IO
    HDIChar strName[16];
    HDIInt32 iFlags; // it set: has the right to lookup on GPT
    HDIInt32 iStatus;
    HDIReadT fRead;
    HDIWriteT fWrite;
    HDIReadExT fReadEx;
    HDIWriteExT fWriteEx;
} HDIContext;

typedef struct BootVol HDIBootVol; // physical boot volume.

typedef struct HDIVolume {
	Boolean isGpt;
	HDIContext* hdInterface;
	HDIBootVol* bootVolume;
} Attribute((packed)) HDIVolume;

#define HDISetInterfaceFlag(INTERFACE, FLAG) (INTERFACE->iFlags |= FLAG)
#define HDIHasInterfaceFlag(INTERFACE, FLAG) (((INTERFACE->iFlags & FLAG) == FLAG) || (INTERFACE->iFlags == FLAG))