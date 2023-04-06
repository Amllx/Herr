#pragma once

#include <Number.h>

typedef enum {
	FIS_TYPE_REG_H2D	= 0x27,	// Register FIS - host to device
	FIS_TYPE_REG_D2H	= 0x34,	// Register FIS - device to host
	FIS_TYPE_DMA_ACT	= 0x39,	// DMA activate FIS - device to host
	FIS_TYPE_DMA_SETUP	= 0x41,	// DMA setup FIS - bidirectional
	FIS_TYPE_DATA		= 0x46,	// Data FIS - bidirectional
	FIS_TYPE_BIST		= 0x58,	// BIST activate FIS - bidirectional
	FIS_TYPE_PIO_SETUP	= 0x5F,	// PIO setup FIS - device to host
	FIS_TYPE_DEV_BITS	= 0xA1,	// Set device bits FIS - device to host
} FIS_TYPE;

typedef enum {
	ATA_CMD_IDENTIFY     = 0xEC,
	ATA_CMD_READ_DMA     = 0xC8,
	ATA_CMD_READ_DMA_EX  = 0x25,
	ATA_CMD_WRITE_DMA    = 0xCA,
 	ATA_CMD_WRITE_DMA_EX = 0x35
} FIS_COMMAND;


typedef struct FisRegH2d {
	// DWORD 0
	UInt8  fisType;	// FIS_TYPE_REG_H2D
 
	UInt8  portMul  : 4;	// Port multiplier
	UInt8  reserved0 : 3;		// Reserved
	UInt8  cmdOrCtrl : 1;		// 1: Command, 0: Control
 
	UInt8  command;	// Command register
	UInt8  featurel;	// Feature register, 7:0
 
	// DWORD 1
	UInt8  lba0;		// LBA low register, 7:0
	UInt8  lba1;		// LBA mid register, 15:8
	UInt8  lba2;		// LBA high register, 23:16
	UInt8  device;		// Device register
 
	// DWORD 2
	UInt8  lba3;		// LBA register, 31:24
	UInt8  lba4;		// LBA register, 39:32
	UInt8  lba5;		// LBA register, 47:40
	UInt8  featureHigh;	// Feature register, 15:8
 
	// DWORD 3
	UInt8  countLow;		// Count register, 7:0
	UInt8  countHigh;		// Count register, 15:8
	UInt8  icc;		// Isochronous command completion
	UInt8  control;	// Control register
 
	// DWORD 4
	UInt8  reserved1[4];	// Reserved
} FisRegH2d;


typedef struct FisRegD2h {
	// DWORD 0
	UInt8  fisType;    // FIS_TYPE_REG_D2H
 
	UInt8  portMul :4;    // Port multiplier
	UInt8  reserved0:2;      // Reserved
	UInt8  interruptBit:1;         // Interrupt bit
	UInt8  reserved1:1;      // Reserved
 
	UInt8  status;      // Status register
	UInt8  error;       // Error register
 
	// DWORD 1
	UInt8  lba0;        // LBA low register, 7:0
	UInt8  lba1;        // LBA mid register, 15:8
	UInt8  lba2;        // LBA high register, 23:16
	UInt8  device;      // Device register
 
	// DWORD 2
	UInt8  lba3;        // LBA register, 31:24
	UInt8  lba4;        // LBA register, 39:32
	UInt8  lba5;        // LBA register, 47:40
	UInt8  rsv2;        // Reserved
 
	// DWORD 3
	UInt8  countLow;      // Count register, 7:0
	UInt8  countHigh;      // Count register, 15:8
	UInt8  rsv3[2];     // Reserved
 
	// DWORD 4
	UInt8  rsv4[4];     // Reserved
} FisRegD2h;


typedef struct FisData
{
	// DWORD 0
	UInt8  fisType;	// FIS_TYPE_DATA
 
	UInt8  portMul :4;	// Port multiplier
	UInt8  reserved0:4;		// Reserved
 
	UInt8  reserved1[2];	// Reserved
 
	// DWORD 1 ~ N
	UInt32 data[1];	// Payload
} FisData;


typedef struct FisPioSetup
{
	// DWORD 0
	UInt8  fisType;	// FIS_TYPE_PIO_SETUP
 
	UInt8  portMul :4;	// Port multiplier
	UInt8  reserved0:1;		// Reserved
	UInt8  d:1;		// Data transfer direction, 1 - device to host
	UInt8  interruptBit:1;		// Interrupt bit
	UInt8  reserved1:1;
 
	UInt8  status;		// Status register
	UInt8  error;		// Error register
 
	// DWORD 1
	UInt8  lba0;		// LBA low register, 7:0
	UInt8  lba1;		// LBA mid register, 15:8
	UInt8  lba2;		// LBA high register, 23:16
	UInt8  device;		// Device register
 
	// DWORD 2
	UInt8  lba3;		// LBA register, 31:24
	UInt8  lba4;		// LBA register, 39:32
	UInt8  lba5;		// LBA register, 47:40
	UInt8  rsv2;		// Reserved
 
	// DWORD 3
	UInt8  countLow;		// Count register, 7:0
	UInt8  countHigh;		// Count register, 15:8
	UInt8  rsv3;		// Reserved
	UInt8  e_status;	// New value of status register
 
	// DWORD 4
	UInt16 tc;		// Transfer count
	UInt8  rsv4[2];	// Reserved
} FisPioSetup;


typedef struct FisDmaSetup
{
	// DWORD 0
	UInt8  fisType;	// FIS_TYPE_DMA_SETUP
 
	UInt8  portMul :4;	// Port multiplier
	UInt8  reserved0:1;		// Reserved
	UInt8  d:1;		// Data transfer direction, 1 - device to host
	UInt8  interruptBit:1;		// Interrupt bit
	UInt8  a:1;            // Auto-activate. Specifies if DMA Activate FIS is needed
 
    UInt8  reserved1[2];       // Reserved
 
	//DWORD 1&2
	UInt64 DMAbufferID;    // DMA Buffer Identifier. Used to Identify DMA buffer in host memory. SATA Spec says host specific and not in Spec. Trying AHCI spec might work.

	//DWORD 3
	UInt32 rsvd;           //More reserved

	//DWORD 4
	UInt32 DMAbufOffset;   //Byte offset into buffer. First 2 bits must be 0

	//DWORD 5
	UInt32 TransferCount;  //Number of bytes to transfer. Bit 0 must be 0

	//DWORD 6
	UInt32 reserved3;          //Reserved
} FisDmaSetup;

typedef struct FisDevBits
{
	// DWORD 0
	UInt8  fisType;	// FIS_TYPE_DMA_SETUP (A1h)
 
	UInt8 reserved0 : 5;		// Reserved
	UInt8 r0 : 1;
	UInt8 interruptBit : 1;
	UInt8 n : 1;

	UInt8 statusLow : 3;
	UInt8 r1 : 1;
	UInt8 statusHigh : 3;

	UInt8 r2 : 1;
	UInt8 error;

	// DWORD 1
	UInt32 act;
} FisDevBits;
