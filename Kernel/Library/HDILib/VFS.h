#pragma once

#include <HDILib/ACL.h>
#include <HDILib/Core.h>

// Virtual FileSystem
// Organized in directories and files.

#define HDI_USED (104)
#define HDI_NOT_FOUND (103)
#define HDI_LBA_START (1024U)
#define HDI_COMMON_SECTOR_FACTOR (256) /* Each byte must perfect chunks of 256. */
#define HDI_MAX_FILES (512)
#define HDI_MAX_IDS 	 (10000)
#define HDI_UNATTRIBUTED (-2)

typedef struct HDIFile {
	UInt16 strFileName[256];
	Int32 iStatus;
	HDILba iStart; // where?
	SizeT szFile;
	Boolean bLocked;
} HDIFile;

typedef struct HDIDirectory {
	Int64 iId;
	Int32 iStatus;
	Boolean bLocked;
	HDILba iWhere;

	struct HDIFile* Files[HDI_MAX_FILES];
	struct HDIDirectory* Child;
	struct HDIDirectory* Root;
} HDIDirectory;

typedef struct FileHandle { UInt32 __id; } FileHandle, *PtrFileHandle;

PtrFileHandle CreateVirtualHandle(void);
Void SetCurrentVirtualIO(HDIContext* interface);

UInt16* VirtualRead(PtrFileHandle FileHandle, UInt16* strName);
Boolean VirtualWrite(PtrFileHandle FileHandle, UInt16* strName, UInt16* buffer);