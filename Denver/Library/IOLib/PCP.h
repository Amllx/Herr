#pragma once

#include <Lib.h>
#include <CRC32.h>
#include <IOLib/ACPI.h>

#ifndef PCP_FAIL
#define PCP_FAIL (0xDEEDBEEF)
#endif

#ifndef PCP_OK
#define PCP_OK (0x00000200)
#endif

typedef struct PciReq {
	UIntPtr address;
	struct PciResponse* response;
} PciReq;

typedef struct PciResponse {
	UInt8 Class;
	UInt8 ProgIf;
	UInt8 SubClass;
	UInt16 DeviceId;
	UInt16 VendorId;
	
	UInt64 CallbackSlot1;
	UInt64 CallbackSlot2;
	UInt64 CallbackSlot3;
	UInt64 CallbackSlot4;
	UInt64 CallbackSlot5;
	UInt64 CallbackSlot6;
	UInt64 CallbackSlot7;
	UInt64 CallbackSlot8;
	UInt64 CallbackSlot9;
	UInt64 CallbackSlot10;
} Attribute((packed)) PciResponse;

VoidPtr PcpMakeRequest(const Char* strArgs, VoidPtr AdditionalStruct);
