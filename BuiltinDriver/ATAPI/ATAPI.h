#pragma once

#include <System.h>
#include <ATA/ATA.h>

#define ATAPI_SECTOR_SIZE 	(2048U)

#define ATAPI_CMD_READ      (0xA8)
#define ATAPI_CMD_EJECT     (0x1B)

UInt8* ATAPIRead(UInt16 Bus, UInt8 Drive, UInt8 Lba);