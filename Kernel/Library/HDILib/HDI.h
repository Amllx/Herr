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
