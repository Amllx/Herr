#include <SATA/SATA.h>
#include <AHCI/AHCI.h>
#include <DT/Result.h>
#include <IOLib/PCP.h>
#include <AHCI/FisDma.h>

static PciDeviceHeader* gGlobalSataPci = NULL;
static PciBarHeader* gSataBar = NULL;
static Boolean gSataDetected = False;
static UInt32 gPortsImpl = 0;
static AhciBuffer gAhciBuffer;

#define HBA_PORT_IPM_ACTIVE  (1)
#define HBA_PORT_DET_PRESENT (3)
#define HBA_PxCMD_CR 	(0x8000)
#define HBA_PxCMD_FRE 	(0x0010)
#define HBA_PxCMD_ST 	(0x0001)
#define HBA_PxCMD_FR	(0x4000)
#define HBA_MAX_PORT 	(32U)
#define SATA_SIG_ATA	(0x00000101)	// SATA drive signature
#define SATA_SIG_ATAPI	(0xEB140101)	// SATAPI drive signature

Boolean ProbePorts(Void) {
	UIntPtr BAR = (gSataBar->BAR5);

    PciBecomeBusMaster(gGlobalSataPci->Bus, gGlobalSataPci->Device, gGlobalSataPci->Function, BAR);
    PciEnableMmio(gGlobalSataPci->Bus, gGlobalSataPci->Device, gGlobalSataPci->Function, BAR);

	HbaMem* mem = (HbaMem*)((UIntPtr)(BAR));
	VoidPtr dir = VirtualCreateMemoryEx((VoidPtr)BAR, FLAG_RW);
	VirtualFlushTLB(dir);

	gAhciBuffer.hbaMem = mem; // store the host here.
	gAhciBuffer.ahciVersionLow = gAhciBuffer.hbaMem->vs >> 0x0F;
	gAhciBuffer.ahciVersionHigh = gAhciBuffer.hbaMem->vs >> 31;

	gPortsImpl = mem->pi;

	for (UInt32 index = 0; index < HBA_MAX_PORT; index++) {
		if ((gPortsImpl & 1) == 1) {
			if (((gAhciBuffer.hbaMem->ports[index].ssts >> 8) & 0x0F) != HBA_PORT_DET_PRESENT) {
				ConsoleLog("Port is not implemented!\n");
				continue;
			} else if ((gAhciBuffer.hbaMem->ports[index].ssts & 0x0F) != HBA_PORT_IPM_ACTIVE) {
				ConsoleLog("Port is inactive....\n");
				continue;
			}

			if (gAhciBuffer.hbaMem->ports[index].sig == SATA_SIG_ATA) {
				ConsoleLog("SATA is detected\n");
				return True;
			} else if (gAhciBuffer.hbaMem->ports[index].sig == SATA_SIG_ATAPI) {
				ConsoleLog("SATAPI is detected\n");
				return True;
			}
		}

		gPortsImpl >>= 1;
	}

	gSataDetected = False;
	return False;
}

typedef struct __SataId {
	UInt16 DeviceId;
	UInt16 VendorId;
	Char name[1024];
} __SataId;

static __SataId gDriveId[] = {
		{ 0x1002, 0x4380, "ATI SB600" },
		{ 0x1002, 0x4390, "ATI SB700/800" },
		{ 0x1002, 0x4391, "ATI IXP700" },
		{ 0x1002, 0x4392, "ATI SB700/800" },
		{ 0x1002, 0x4393, "ATI SB700/800" },
		{ 0x1002, 0x4394, "ATI SB700/800" },
		{ 0x1002, 0x4395, "ATI SB700/800" },
		{ 0x1039, 0x1184, "SiS 966" },
		{ 0x1039, 0x1185, "SiS 966" },
		{ 0x1039, 0x0186, "SiS 968" },
		{ 0x10b9, 0x5288, "Acer Labs M5288" },
		{ 0x10de, 0x044c, "NVIDIA MCP65" },
		{ 0x10de, 0x044d, "NVIDIA MCP65" },
		{ 0x10de, 0x044e, "NVIDIA MCP65" },
		{ 0x10de, 0x044f, "NVIDIA MCP65" },
		{ 0x10de, 0x045c, "NVIDIA MCP65" },
		{ 0x10de, 0x045d, "NVIDIA MCP65" },
		{ 0x10de, 0x045e, "NVIDIA MCP65" },
		{ 0x10de, 0x045f, "NVIDIA MCP65" },
		{ 0x10de, 0x0550, "NVIDIA MCP67" },
		{ 0x10de, 0x0551, "NVIDIA MCP67" },
		{ 0x10de, 0x0552, "NVIDIA MCP67" },
		{ 0x10de, 0x0553, "NVIDIA MCP67" },
		{ 0x10de, 0x0554, "NVIDIA MCP67" },
		{ 0x10de, 0x0555, "NVIDIA MCP67" },
		{ 0x10de, 0x0556, "NVIDIA MCP67" },
		{ 0x10de, 0x0557, "NVIDIA MCP67" },
		{ 0x10de, 0x0558, "NVIDIA MCP67" },
		{ 0x10de, 0x0559, "NVIDIA MCP67" },
		{ 0x10de, 0x055a, "NVIDIA MCP67" },
		{ 0x10de, 0x055b, "NVIDIA MCP67" },
		{ 0x10de, 0x07f0, "NVIDIA MCP73" },
		{ 0x10de, 0x07f1, "NVIDIA MCP73" },
		{ 0x10de, 0x07f2, "NVIDIA MCP73" },
		{ 0x10de, 0x07f3, "NVIDIA MCP73" },
		{ 0x10de, 0x07f4, "NVIDIA MCP73" },
		{ 0x10de, 0x07f5, "NVIDIA MCP73" },
		{ 0x10de, 0x07f6, "NVIDIA MCP73" },
		{ 0x10de, 0x07f7, "NVIDIA MCP73" },
		{ 0x10de, 0x07f8, "NVIDIA MCP73" },
		{ 0x10de, 0x07f9, "NVIDIA MCP73" },
		{ 0x10de, 0x07fa, "NVIDIA MCP73" },
		{ 0x10de, 0x07fb, "NVIDIA MCP73" },
		{ 0x10de, 0x0ad0, "NVIDIA MCP77" },
		{ 0x10de, 0x0ad1, "NVIDIA MCP77" },
		{ 0x10de, 0x0ad2, "NVIDIA MCP77" },
		{ 0x10de, 0x0ad3, "NVIDIA MCP77" },
		{ 0x10de, 0x0ad4, "NVIDIA MCP77" },
		{ 0x10de, 0x0ad5, "NVIDIA MCP77" },
		{ 0x10de, 0x0ad6, "NVIDIA MCP77" },
		{ 0x10de, 0x0ad7, "NVIDIA MCP77" },
		{ 0x10de, 0x0ad8, "NVIDIA MCP77" },
		{ 0x10de, 0x0ad9, "NVIDIA MCP77" },
		{ 0x10de, 0x0ada, "NVIDIA MCP77" },
		{ 0x10de, 0x0adb, "NVIDIA MCP77" },
		{ 0x10de, 0x0ab4, "NVIDIA MCP79" },
		{ 0x10de, 0x0ab5, "NVIDIA MCP79" },
		{ 0x10de, 0x0ab6, "NVIDIA MCP79" },
		{ 0x10de, 0x0ab7, "NVIDIA MCP79" },
		{ 0x10de, 0x0ab8, "NVIDIA MCP79" },
		{ 0x10de, 0x0ab9, "NVIDIA MCP79" },
		{ 0x10de, 0x0aba, "NVIDIA MCP79" },
		{ 0x10de, 0x0abb, "NVIDIA MCP79" },
		{ 0x10de, 0x0abc, "NVIDIA MCP79" },
		{ 0x10de, 0x0abd, "NVIDIA MCP79" },
		{ 0x10de, 0x0abe, "NVIDIA MCP79" },
		{ 0x10de, 0x0abf, "NVIDIA MCP79" },
		{ 0x10de, 0x0d84, "NVIDIA MCP89" },
		{ 0x10de, 0x0d85, "NVIDIA MCP89" },
		{ 0x10de, 0x0d86, "NVIDIA MCP89" },
		{ 0x10de, 0x0d87, "NVIDIA MCP89" },
		{ 0x10de, 0x0d88, "NVIDIA MCP89" },
		{ 0x10de, 0x0d89, "NVIDIA MCP89" },
		{ 0x10de, 0x0d8a, "NVIDIA MCP89" },
		{ 0x10de, 0x0d8b, "NVIDIA MCP89" },
		{ 0x10de, 0x0d8c, "NVIDIA MCP89" },
		{ 0x10de, 0x0d8d, "NVIDIA MCP89" },
		{ 0x10de, 0x0d8e, "NVIDIA MCP89" },
		{ 0x10de, 0x0d8f, "NVIDIA MCP89" },
		{ 0x1106, 0x3349, "VIA VT8251" },
		{ 0x1106, 0x6287, "VIA VT8251" },
		{ 0x11ab, 0x6121, "Marvell 6121" },
		{ 0x11ab, 0x6145, "Marvell 6145" },
		{ 0x197b, 0x2360, "JMicron JMB360" },
		{ 0x197b, 0x2361, "JMicron JMB361" },
		{ 0x197b, 0x2362, "JMicron JMB362" },
		{ 0x197b, 0x2363, "JMicron JMB363" },
		{ 0x197b, 0x2366, "JMicron JMB366" },
		{ 0x8086, 0x2922, "Qemu 6 Ports SATA Controller [AHCI mode]" },
		{ 0x8086, 0x2652, "Intel ICH6R" },
		{ 0x8086, 0x2653, "Intel ICH6-M" },
		{ 0x8086, 0x2681, "Intel 63xxESB" },
		{ 0x8086, 0x2682, "Intel ESB2" },
		{ 0x8086, 0x2683, "Intel ESB2" },
		{ 0x8086, 0x27c1, "Intel ICH7R (AHCI mode)" },
		{ 0x8086, 0x27c3, "Intel ICH7R (RAID mode)" },
		{ 0x8086, 0x27c5, "Intel ICH7-M (AHCI mode)" },
		{ 0x8086, 0x27c6, "Intel ICH7-M DH (RAID mode)" },
		{ 0x8086, 0x2821, "Intel ICH8 (AHCI mode)" },
		{ 0x8086, 0x2822, "Intel ICH8R / ICH9 (RAID mode)" },
		{ 0x8086, 0x2824, "Intel ICH8 (AHCI mode)" },
		{ 0x8086, 0x2829, "Intel ICH8M (AHCI mode)" },
		{ 0x8086, 0x282a, "Intel ICH8M (RAID mode)" },
		{ 0x8086, 0x2922, "Intel ICH9 (AHCI mode)" },
		{ 0x8086, 0x2923, "Intel ICH9 (AHCI mode)" },
		{ 0x8086, 0x2924, "Intel ICH9" },
		{ 0x8086, 0x2925, "Intel ICH9" },
		{ 0x8086, 0x2927, "Intel ICH9" },
		{ 0x8086, 0x2929, "Intel ICH9M" },
		{ 0x8086, 0x292a, "Intel ICH9M" },
		{ 0x8086, 0x292b, "Intel ICH9M" },
		{ 0x8086, 0x292c, "Intel ICH9M" },
		{ 0x8086, 0x292f, "Intel ICH9M" },
		{ 0x8086, 0x294d, "Intel ICH9" },
		{ 0x8086, 0x294e, "Intel ICH9M" },
		{ 0x8086, 0x3a05, "Intel ICH10" },
		{ 0x8086, 0x3a22, "Intel ICH10" },
		{ 0x8086, 0x3a25, "Intel ICH10" },
		{ 0x15ad, 0x7e0,  "VMWare SATA AHCI Controller." },
		{ 0xFFFF, 0xFFFF, "Unknown Hard Drive." }
};

Boolean AhciInitDriver(Void) {
	if (AhciDetected()) return False;

	PciReq* get = MemAlloc(sizeof(PciReq));
	PciResponse* callback =  MemAlloc(sizeof(PciResponse));

	gSataBar = MemAlloc(sizeof(PciBarHeader));

	if (callback == NULL) return False;
	if (gSataBar == NULL) return False;
	if (get == NULL) return False;

#define PCI_CLASS_MASS_DISK_STORAGE (0x01)
#define PCI_SUBCLASS_SATA (0x06)
#define PCI_PROGIF_MDS (0x01)

	callback->Class = PCI_CLASS_MASS_DISK_STORAGE; // Mass Disk Storage
	callback->SubClass = PCI_SUBCLASS_SATA; // SATA
	callback->ProgIf = 0x01; // AHCI 1.0

	get->response = callback;

	gGlobalSataPci = PcpMakeRequest("GET", get);
	gSataDetected = get->response->CallbackSlot5 != PCP_FAIL;

	if (!gSataDetected) {
		goto Fail;
	} else {
		goto Finish;
	}

	Finish: {
		for (SizeT i = 0; i < 120; ++i)
		{
			if (gDriveId[i].DeviceId == gGlobalSataPci->DeviceId &&
				gDriveId[i].VendorId == gGlobalSataPci->VendorId) {
					ConsoleLog("%s %n", gDriveId->name);
					break;
				}
		}
		
		ProbePorts();
		ConsoleLog("%s %b %n", "Is SATA detected ? ", gSataDetected);
		
		if (!gSataDetected) goto Fail;
		return True;
	};

	Fail: {
		MemFree(get);
		MemFree(callback);
		MemFree(gSataBar);

		return False;
	};
}

Void StopCMD(HbaPort* hbaMem) {
	hbaMem->cmd &= ~HBA_PxCMD_ST;
	hbaMem->cmd &= ~HBA_PxCMD_FRE;

	while(True) {
		if (hbaMem->cmd & HBA_PxCMD_FR) continue;
		if (hbaMem->cmd & HBA_PxCMD_CR) continue;

		break;
	}
}

Void StartCMD(HbaPort* hbaMem) {
	while (hbaMem->cmd & HBA_PxCMD_CR);

	hbaMem->cmd |= HBA_PxCMD_FRE;
	hbaMem->cmd |= HBA_PxCMD_ST;
}

Void ConfigurePort(HbaPort* port, Int32 portno) {
	StopCMD(port);	// Stop command engine
 
	// Command list offset: 1K*portno
	// Command list entry size = 32
	// Command list entry maxim count = 32
	// Command list maxim size = 32*32 = 1K per port
	port->clb = AHCI_BASE_ADDRESS + (portno<<10);
	port->clbu = 0;
	SetMem((VoidPtr)((UIntPtr)port->clb), 0, 1024);
 
	// FIS offset: 32K+256*portno
	// FIS entry size = 256 bytes per port
	port->fb = AHCI_BASE_ADDRESS + (32<<10) + (portno<<8);
	port->fbu = 0;
	SetMem((VoidPtr)((UIntPtr)port->fb), 0, 256);
 
	// Command table offset: 40K + 8K*portno
	// Command table size = 256*32 = 8K per port
	HbaCmdHeader *cmdheader = (HbaCmdHeader*)((UInt64)port->clb);
	for (int i=0; i<32; i++)
	{
		cmdheader[i].prdtl = 8;	// 8 prdt entries per command table
					// 256 bytes per command table, 64+16+48+16*8
		// Command table offset: 40K + 8K*portno + cmdheader_index*256
		cmdheader[i].ctba = AHCI_BASE_ADDRESS + (40<<10) + (portno<<13) + (i<<8);
		cmdheader[i].ctbau = 0;
		SetMem((VoidPtr)(UIntPtr)cmdheader[i].ctba, 0, 256);
	}
 
	StartCMD(port);	// Start command engine
}

Boolean AhciDiskWrite(Char* bytes, HDISize lba) {
    if (!gSataDetected) return False;
	if (StringLength(bytes) > 4096) return False;

	for (SizeT index = 0; index < HBA_MAX_PORT; ++index) {
		HbaPort* port = &gAhciBuffer.hbaMem->ports[index];
		ConfigurePort(port, index);

		if (!SataWrite(port, lba, StringLength(bytes), bytes)) return False;
	}

	return True;
}

Char* AhciDiskRead(HDISize lba, HDISize length) {
    if (!gSataDetected) return NULL;
	if (length > 4096) return NULL;

	for (SizeT index = 0; index < HBA_MAX_PORT; ++index) {
		SetMem(gAhciBuffer.buffer, 1, length);

		HbaPort* port = &gAhciBuffer.hbaMem->ports[index];
		ConfigurePort(port, index);

		if (!SataRead(port, lba, length, gAhciBuffer.buffer)) return NULL;
	}

	ConsoleLog(gAhciBuffer.buffer);
	gAhciBuffer.buffer[length] = '\0';
	return gAhciBuffer.buffer;
}

Boolean AhciDetected(Void) { return gSataDetected; }
