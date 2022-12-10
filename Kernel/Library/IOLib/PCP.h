#pragma once

#include <Lib.h>
#include <CRC32.h>
#include <IOLib/ACPI.h>

/*
 * File: PCP.h
 * Purpose: PCI Control Protocol
 * 
 * Copyright Amlal El Mahrouss, all rights reserved.
 */

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
	
	UInt64 callbackSlot1;
	UInt64 callbackSlot2;
	UInt64 callbackSlot3;
	UInt64 callbackSlot4;
	UInt64 callbackSlot5;
	UInt64 callbackSlot6;
	UInt64 callbackSlot7;
	UInt64 callbackSlot8;
	UInt64 callbackSlot9;
	UInt64 callbackSlot10;
} Attribute((packed)) PciResponse;

VoidPtr PcpMakeRequest(const Char* strArgs, VoidPtr AdditionalStruct);
