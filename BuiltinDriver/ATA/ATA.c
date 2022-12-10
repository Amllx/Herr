#include <ATA/ATA.h>
#include <HILib/Intel/Intel.h>

static Boolean gIsUsingMasterDrive = False;
static Boolean gATADetected = False;

void IDESelect(UInt8 Bus, Boolean isMaster) {
	if (Bus == ATA_PRIMARY)
		Out8(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, isMaster ? ATA_PRIMARY_SEL : ATA_SECONDARY_SEL);
	else
		Out8(ATA_SECONDARY_IO + ATA_REG_HDDEVSEL, isMaster ? ATA_PRIMARY_SEL : ATA_SECONDARY_SEL);

	gIsUsingMasterDrive = isMaster;
}

void IDEPrimaryIrq(Void) { ISSendEoi(ATA_PRIMARY_IRQ); }
void IDESecondIrq(Void) { ISSendEoi(ATA_SECONDARY_IRQ); }

extern void AddInt(UIntPtr isr, UInt8 attr, UInt8 index);

Boolean IDEInitDriver(UInt8 Bus, UInt8 Drive) {
	AddInt((UIntPtr)IDEPrimaryIrq, 0, ATA_PRIMARY_IRQ);
	AddInt((UIntPtr)IDESecondIrq, 0, ATA_SECONDARY_IRQ);

	UInt16 IO = (Bus == ATA_PRIMARY) ? ATA_PRIMARY_IO : ATA_SECONDARY_IO;

	IDESelect(Bus, Drive);

	Out8(IO + ATA_REG_SEC_COUNT0, 0);
	Out8(IO + ATA_REG_LBA0, 0);
	Out8(IO + ATA_REG_LBA1, 0);
	Out8(IO + ATA_REG_LBA2, 0);

	Out8(IO + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);

	UInt8 status = In8(IO + ATA_REG_STATUS);

	if (status) {
		Int32 spin = 0;
		while ((In8(IO + ATA_REG_STATUS) & ATA_SR_BSY) != 0) {
			++spin;
			if (spin == 1000) break;
		}
		
		if (status & ATA_REG_ERROR) {
			ConsoleLog("IDE_ERR[{ATA_REG_ERROR}]\n");
			return False;
		}

		while (!(status & ATA_SR_DRQ))
			status = In8(IO + ATA_REG_STATUS);

		ConsoleLog("ATA Drive is now On-Line.\n");
		ConsoleLog(IO == ATA_PRIMARY ? "ATA PRIMARY\n" : "ATA SECONDARY\n");

		gATADetected = True;
		return status;
	}

	ConsoleLog("IDE_ERR[{NOT_IDE}]\n");
	return False;
}

void IDEDelay(UInt16 IO) {
	for(int i = 0;i < 4; i++)
		In8(IO + ATA_REG_ALT_STATUS);
}

void IDEPoll(UInt16 IO) {
	IDEDelay(IO);

	retry:
		UInt8 stat = In8(IO + ATA_REG_STATUS);
		if(stat & ATA_SR_BSY) goto retry;

	retry2:
		stat = In8(IO + ATA_REG_STATUS);

		if (stat & ATA_SR_ERR) return;

	if(!(stat & ATA_SR_DRQ)) goto retry2;

	return;
}

UInt8 IDERead(UInt32 Lba, UInt8 Drive) {
	Lba &= 0x00FFFFFF;
	UInt16 IO = 0;

	switch (Drive) {
		case (ATA_PRIMARY << 1 | ATA_MASTER): {
			IO = ATA_PRIMARY_IO;
			Drive = ATA_MASTER;
			break;
		}

		case (ATA_SECONDARY << 1 | ATA_MASTER): {
			IO = ATA_SECONDARY_IO;
			Drive = ATA_MASTER;

			break;
		}

		case (ATA_PRIMARY << 1 | ATA_SLAVE): {
			IO = ATA_PRIMARY_IO;
			Drive = ATA_SLAVE;
			break;
		}

		case (ATA_SECONDARY << 1 | ATA_SLAVE): {
			IO = ATA_SECONDARY_IO;
			Drive = ATA_SLAVE;

			break;
		}
	}

	UInt8 Command = (Drive == ATA_MASTER ? 0xE0 : 0xF0);

	Out8(IO + ATA_REG_HDDEVSEL, Command | ((Lba >> 24) & 0x0f));
	Out8(IO + 1, 0x00);

	Out8(IO + ATA_REG_SEC_COUNT0, 0x1);

	Out8(IO + ATA_REG_LBA0, (UInt8)Lba);
	Out8(IO + ATA_REG_LBA1, (UInt8)Lba >> 8);
	Out8(IO + ATA_REG_LBA2, (UInt8)Lba >> 16);


	Out8(IO + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

	while ((In8 (ATA_COMMAND (IO))) & 0x88)
		IDEDelay(IO);

	IDEPoll(IO);

	UInt16 data = In16(IO + ATA_REG_DATA);

	while ((In8 (ATA_COMMAND (IO))) & 0x88)
		IDEDelay(IO);

	return (UInt8)data;
}

Void IDEWrite(UInt8 Byte, UInt32 Lba, UInt8 Drive) {
	Lba &= 0x00FFFFFF;
	UInt16 IO = 0;

	switch (Drive) {
		case (ATA_PRIMARY << 1 | ATA_MASTER): {
			IO = ATA_PRIMARY_IO;
			Drive = ATA_MASTER;
			break;
		}

		case (ATA_SECONDARY << 1 | ATA_MASTER): {
			IO = ATA_SECONDARY_IO;
			Drive = ATA_MASTER;

			break;
		}

		case (ATA_PRIMARY << 1 | ATA_SLAVE): {
			IO = ATA_PRIMARY_IO;
			Drive = ATA_SLAVE;
			break;
		}

		case (ATA_SECONDARY << 1 | ATA_SLAVE): {
			IO = ATA_SECONDARY_IO;
			Drive = ATA_SLAVE;

			break;
		}
	}

	UInt8 Command = (Drive == ATA_MASTER ? 0xE0 : 0xF0);

	Out8(IO + ATA_REG_HDDEVSEL, Command | ((Lba >> 24) & 0x0f));
	Out8(IO + 1, 0x00);

	Out8(IO + ATA_REG_SEC_COUNT0, 0x1);

	Out8(IO + ATA_REG_LBA0, (UInt8)Lba);
	Out8(IO + ATA_REG_LBA1, (UInt8)Lba >> 8);
	Out8(IO + ATA_REG_LBA2, (UInt8)Lba >> 16);

	Out8(IO + ATA_REG_COMMAND, ATA_CMD_WRITE_PIO); // TODO: support DMA

	IDEPoll(IO);

	Out16(IO + ATA_REG_DATA, Byte);

	while ((In8 (ATA_COMMAND (IO))) & 0x88)
		IDEDelay(IO);
}

Boolean AtaDetected(Void) { return gATADetected;  }