#pragma once

#include <AHCI/FisDma.h>
#include <Number.h>
#include <Bool.h>
#include <Defs.h>

#ifndef AHCI_GHC_AE
#define AHCI_GHC_AE (31)
#endif //!AHCI_GHC_AE

#ifndef AHCI_BASE_ADDRESS
#define AHCI_BASE_ADDRESS (0x4000000)
#endif //!AHCI_ADDRESS

typedef volatile struct HbaPort {
	UInt32 clb;		// 0x00, command list base address, 1K-byte aligned
	UInt32 clbu;		// 0x04, command list base address upper 32 bits
	UInt32 fb;		// 0x08, FIS base address, 256-byte aligned
	UInt32 fbu;		// 0x0C, FIS base address upper 32 bits
	UInt32 is;		// 0x10, interrupt status
	UInt32 ie;		// 0x14, interrupt enable
	UInt32 cmd;		// 0x18, command and status
	UInt32 reserved0;		// 0x1C, Reserved
	UInt32 tfd;		// 0x20, task file data
	UInt32 sig;		// 0x24, signature
	UInt32 ssts;		// 0x28, SATA status (SCR0:SStatus)
	UInt32 sctl;		// 0x2C, SATA control (SCR2:SControl)
	UInt32 serr;		// 0x30, SATA error (SCR1:SError)
	UInt32 sact;		// 0x34, SATA active (SCR3:SActive)
	UInt32 ci;		// 0x38, command issue
	UInt32 sntf;		// 0x3C, SATA notification (SCR4:SNotification)
	UInt32 fbs;		// 0x40, FIS-based switch control
	UInt32 reserved1[11];	// 0x44 ~ 0x6F, Reserved
	UInt32 vendor[4];	// 0x70 ~ 0x7F, vendor specific
}  HbaPort;

typedef volatile struct HbaMem {
	// 0x00 - 0x2B, Generic Host Control
	UInt32 cap;		// 0x00, Host capability
	UInt32 ghc;		// 0x04, Global host control
	UInt32 is;		// 0x08, Interrupt status
	UInt32 pi;		// 0x0C, Port implemented
	UInt32 vs;		// 0x10, Version
	UInt32 ccc_ctl;	// 0x14, Command completion coalescing control
	UInt32 ccc_pts;	// 0x18, Command completion coalescing ports
	UInt32 em_loc;		// 0x1C, Enclosure management location
	UInt32 em_ctl;		// 0x20, Enclosure management control
	UInt32 cap2;		// 0x24, Host capabilities extended
	UInt32 bohc;		// 0x28, BIOS/OS handoff control and status
 
	UInt16  rsv;
	UInt32  resv2;
 
	HbaPort	 ports[1];	// 1 ~ 32
}  HbaMem;

typedef struct HbaCmdHeader {
	// DW0
	UInt8  cfl:5;		// Command FIS length in DWORDS, 2 ~ 16
	UInt8  atapi:1;		// ATAPI
	UInt8  write:1;		// Write, 1: H2D, 0: D2H
	UInt8  prefetchable:1;		// Prefetchable
 
	UInt8  reset:1;		// Reset
	UInt8  BIST:1;		// BIST
	UInt8  clear:1;		// Clear busy upon R_OK
	UInt8  reserved0:1;		// Reserved
	UInt8  pmp:4;		// Port multiplier port

	UInt16 prdtl;		// Physical region descriptor table length in entries
	volatile UInt32 prdbc;		// Physical region descriptor byte count transferred
	
	UInt32 ctba;		// Command table descriptor base address
	UInt32 ctbau;		// Command table descriptor base address upper 32 bits
	
	UInt32 reserved1[4];	// Reserved
}  HbaCmdHeader;

typedef volatile struct HbaFis {
	// 0x00
	FisDmaSetup	dsfis;		// DMA Setup FIS
	UInt8 pad0[4];
	// 0x20
	FisPioSetup	psfis;		// PIO Setup FIS
	UInt8 pad1[12];
	// 0x40
	FisRegD2h	rfis;		// Register – Device to Host FIS
	UInt8 pad2[4];
	// 0x58
	FisDevBits	sdbfis;		// Set Device Bit FIS
	// 0x60
	UInt8 ufis[64];
	// 0xA0
	UInt8 rsv[0x100-0xA0];
}  HbaFis;

typedef struct HbaPrdtEntry {
	UInt32 dba;		// Data base address
	UInt32 dbau;		// Data base address upper 32 bits
	UInt32 reserved0;		// Reserved
	// DW3
	UInt32 dbc:22;		// Byte count, 4M max
	UInt32 reserved1:9;		// Reserved
	UInt32 interruptBit:1;		// Interrupt on completion
}  HbaPrdtEntry;

typedef struct HbaCmdTbl {
	UInt8  cfis[64];	// Command FIS
	UInt8  acmd[16];	// ATAPI command, 12 or 16 bytes
	UInt8  rsv[48];	// Reserved
	HbaPrdtEntry	prdtEntries[1];	// Physical region descriptor table entries, 0 ~ 65535
}  HbaCmdTbl;

/*
 *
 *
 *
 *
 *
 */

// AHCI Port abstraction
typedef struct AhciBuffer {
	HbaMem* hbaMem;

	UInt16 ahciVersionLow;
	UInt16 ahciVersionHigh;

	Char buffer[4096];
}  AhciBuffer; // Buffer
