/*
 *	========================================================
 *
 *	DENVER
 * 	Date Added: 08/01/2023
 * 	Copyright XPX Technologies, all rights reserved.
 *
 * 	========================================================
 */

#pragma once

#include <Key16.h>
#include <HDILib/Core.h>

Boolean HDIEnabled(Void);
UInt16 HDIGetType(Void);
HDIVolume* HDIBootVolume(void);
Boolean OpenHDI(struct BootStruct* bs);
Boolean HDIGetPIODiskIO(HDIContext* hdinterface);
HDILba HDITranslateChs(HDIInt64 cylinder, HDIInt64 head, HDIInt64 sector);
HDIContext* HDICreateContext(HDIChar* hdName, UInt16 iFlags, Boolean isPrivileged);

UInt16* HDIRead_PIO(struct HDIContext* drive, HDILba lba, UInt8 sectorCount);
Boolean HDIWrite_PIO(struct HDIContext* drive, HDILba lba, UInt8 sectorCount, UInt16* buf);
