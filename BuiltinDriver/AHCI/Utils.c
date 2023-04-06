#include <AHCI/Utils.h>

Int32 FindCmdSlot(HbaPort *port) {
	UInt32 slots = (port->sact | port->ci);

	for (Int32 index = 0; index < 32; index++) {
		if ((slots & 1) == 0) return index;
		slots >>= 1;
	}

	return -1;
}
