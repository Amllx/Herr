#include <StringUtils.h>
#include <AHCI/FisDma.h>
#include <AHCI/Utils.h>
#include <AHCI/AHCI.h>
#include <ATA/ATA.h>


Int32 SataWrite(HbaPort* Port,  UInt64 iBaseLba, UInt64 iLen, Char* Buffer) {
	Port->is = (UInt32)-1;
 	Int32 slot = FindCmdSlot(Port);

	UInt32 startLow = (UInt32) iBaseLba;
	UInt32 startHigh = (UInt32) (iBaseLba >> 32);

 	if (slot == -1) return 0;

 	HbaCmdHeader *cmdHeader = (HbaCmdHeader*)(UInt64)Port->clb;
	cmdHeader += slot;
	cmdHeader->cfl = (sizeof(FisRegH2d) / sizeof(UInt32));	// Command FIS size
	cmdHeader->write = 1;
    cmdHeader->clear = 0;
    cmdHeader->prefetchable = 0;
	cmdHeader->prdtl = 1;

    HbaCmdTbl *cmdTbl = (HbaCmdTbl*)((UInt64)cmdHeader->ctba);
	SetMem(cmdTbl, 0, sizeof(HbaCmdTbl) + (cmdHeader->prdtl - 1) * sizeof(HbaPrdtEntry));

	for (SizeT i = 0; i < cmdHeader->prdtl; i++)
	{
#pragma GCC diagnostic ignored "-Wint-conversion"

		cmdTbl->prdtEntries[i].dba = Buffer;
		cmdTbl->prdtEntries[i].dbau = &Buffer;

#pragma GCC diagnostic pop
		
		cmdTbl->prdtEntries[i].dbc = (iLen * 512) - 1;
		cmdTbl->prdtEntries[i].interruptBit = True;
	}

	FisRegH2d *cmdFis = (FisRegH2d*)(&cmdTbl->cfis);

	cmdFis->fisType = FIS_TYPE_REG_H2D;
	cmdFis->cmdOrCtrl = 1;
	cmdFis->command = ATA_CMD_WRITE_DMA_EX;

	cmdFis->lba0 = (UInt8)startLow;
	cmdFis->lba1 = (UInt8)(startLow >> 8);
	cmdFis->lba2 = (UInt8)(startLow >> 16);

	cmdFis->lba3 = (UInt8)startHigh;
	cmdFis->lba4 = (UInt8)(startHigh >> 8);
	cmdFis->lba5 = (UInt8)(startHigh >> 16);

	cmdFis->device = (1 << 6);	// Set this to LBA mode

	cmdFis->countLow = iLen & 0xFF;
	cmdFis->countHigh = (iLen >> 8) & 0xFF;

	Int32 spin = 0;
	
	while ((Port->tfd & (ATA_SR_BSY | ATA_SR_DRQ)) && spin < 1000000) spin++;
	
	if (spin == 1000000) { return False; }

	Port->ci = (1 << slot);
	
	while (True) {
		if ((Port->ci & (1 << slot)) == 0) break;
	}

    return True;
}
