#include <AHCI/AHCI.h>
#include <AHCI/Utils.h>
#include <AHCI/FisDma.h>
#include <StringUtils.h>

#define ATA_DEV_BUSY (0x80)
#define ATA_DEV_DRQ  (0x08)

Int32 SataRead(HbaPort* Port, UInt64 iBaseLba, UInt32 iLength, Char* Buffer) {
	UInt32 sectorL = (UInt32)iBaseLba;
	UInt32 sectorH = (UInt32)(iBaseLba >> 32);

	Port->is = (UInt32)-1;
	Int32 slot = FindCmdSlot(Port);

	if (slot == -1) return False;

	HbaCmdHeader *cmdHeader = (HbaCmdHeader*)(UInt64)Port->clb;

	cmdHeader += slot;
	cmdHeader->cfl = (sizeof(FisRegH2d) / sizeof(UInt32));
	cmdHeader->write = 0; // Read from device
	cmdHeader->atapi = 0;
	cmdHeader->prdtl = 1;

	HbaCmdTbl *cmdTbl = (HbaCmdTbl*)((UInt64)cmdHeader->ctba);
	SetMem(cmdTbl, 0, (sizeof(HbaCmdTbl) + (cmdHeader->prdtl -1)) * sizeof(HbaPrdtEntry));

	for (SizeT i = 0; i < cmdHeader->prdtl; i++)
	{
#pragma GCC diagnostic ignored "-Wint-conversion"

		cmdTbl->prdtEntries[i].dba = Buffer;
		cmdTbl->prdtEntries[i].dbau = &Buffer;

#pragma GCC diagnostic pop
		
		cmdTbl->prdtEntries[i].dbc = (iLength * 512) - 1;
		cmdTbl->prdtEntries[i].interruptBit = True;
	}

	// Setup command
	FisRegH2d *cmdFis = (FisRegH2d*)((UInt64)cmdTbl->cfis);

	cmdFis->fisType = FIS_TYPE_REG_D2H;
	cmdFis->cmdOrCtrl = 1;
	cmdFis->command = ATA_CMD_WRITE_DMA_EX;

	cmdFis->lba0 = (UInt8)sectorL;
	cmdFis->lba1 = (UInt8)((sectorL >> 8) & 0xFF);
	cmdFis->lba2 = (UInt8)((sectorL >> 16) & 0xFF);

	cmdFis->lba3 = (UInt8)(sectorH>>24);
	cmdFis->lba4 = (UInt8)((sectorH >> 8) & 0xFF);
	cmdFis->lba5 = (UInt8)((sectorH >> 16) & 0xFF);
	cmdFis->device = 1 << 6;	// Set this to LBA mode

	cmdFis->fisType = FIS_TYPE_REG_H2D;

	cmdFis->countLow = iLength & 0xFF;
	cmdFis->countHigh = (iLength >> 8) & 0xFF;

	Int32 spin = 0; // Spin lock timeout counter

	while ((Port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 100000000) {
		spin++;
	}

	if (spin == 100000000) { return False; }

	Port->ci = (1 << slot);	// Issue command

	while (True) {
		if ((Port->ci & (1 << slot)) == 0) break;
		if ((Port->is & (1 << 30))) return False;
	}

	return True;
}
 