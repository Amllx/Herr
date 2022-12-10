#pragma once

#include <GraphicsLib/Terminal.h>
#include <MemLib/CoreAlloc.h>
#include <HDILib/HDI.h>
#include <IOLib/Pci.h>
#include <System.h>

#include <SATA/Write.h>
#include <SATA/Read.h>

#define DISK_DRIVER_NAME "SATA Driver"
#define DISK_DRIVER_AUTHOR "Amlal El Mahrouss from ME"
#define DISK_DRIVER_VERSION "1.2.2"

Boolean AhciInitDriver(Void);
Boolean  AhciDetected(Void);
Boolean AhciDiskWrite(Char* bytes, HDISize lba);
Char* AhciDiskRead(HDISize lba, HDISize length);