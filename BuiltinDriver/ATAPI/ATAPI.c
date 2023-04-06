#include <ATAPI/ATAPI.h>
#include <MemLib/Alloc.h>

UInt8* ATAPIRead(UInt16 Bus, UInt8 Drive, UInt8 Lba) {
	// The packet to base ourself of.
	UInt8 ReadCmd[12] = { 0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	UInt8 status = 0;
	Int32 size = 0;

	Out8(Drive & (1 << 4), Bus + 6);

	IDEDelay(Bus);
	Out8(0x0, Bus + 1);
	Out8(ATAPI_SECTOR_SIZE & 0xFF, ATA_ADDRESS2(Bus));
	Out8(ATAPI_SECTOR_SIZE >> 0x8, ATA_ADDRESS3(Bus));

	Out8(0xA0, ATA_COMMAND(Bus)); // Send a packet, it starts here

	while ((status = In8(ATA_COMMAND(Bus))) & 0x80)
		IDEDelay(Bus);


	while ((status = In8(ATA_COMMAND(Bus))) & 0x80 && (status & 0x01) == 0)
		IDEDelay(Bus);

	if (status & 0x01)
		return NULL;

	ReadCmd[9] = 1;              /* 1 sector */
	ReadCmd[2] = (Lba >> 0x18) & 0xFF;   /* most sig. byte of LBA */
	ReadCmd[3] = (Lba >> 0x10) & 0xFF;
	ReadCmd[4] = (Lba >> 0x08) & 0xFF;
	ReadCmd[5] = (Lba >> 0x00) & 0xFF;

	for (SizeT index = 0; index < 12; ++index)
		Out16(Bus + ATA_REG_DATA, ReadCmd[index]);

	size = (Int32) In8(ATA_ADDRESS3(Bus)) << 8 | (Int32) In8(ATA_ADDRESS2(Bus));
	Check(size == ATAPI_SECTOR_SIZE, "ATAPI: Bad Sector Size!");

	UInt8* bytes = MemAlloc(sizeof(UInt8) * (size / 2));

	for (SizeT index = 0; index < size / 2; ++index)
		bytes[index] = In16(Bus + ATA_REG_DATA);

	while ((status = In8 (ATA_COMMAND (Bus))) & 0x88)
		IDEDelay(Bus);

	return 0;
}
