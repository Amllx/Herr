#include <GraphicsLib/Terminal.h>
#include <MemLib/IO.h>
#include <IOLib/PCP.h>
#include <IOLib/PCI.h>

// looks for the matching pci class.
// returns null if it fails.
static VoidPtr PcpGet(UInt8 Class, UInt8 SubClass, UInt8 ProgIf) {
	MCFGHeaderPtr header = PciGetMcfg();

	Int64 entries = 0;
	Int64 i = header->Length - sizeof(struct MCFGHeader);
	
	while (i >= sizeof(struct MCFGHeader)) {
		entries++;
		i -= sizeof(struct MCFGHeader);
	}

	for (Int64 t = 0; t < entries; t++) {
		AcpiConfigHeaderPtr newDeviceConfig = (AcpiConfigHeaderPtr)((UIntPtr)header + sizeof(MCFGHeader) + (sizeof(ACPIConfigHeader) * t));
		
        for (UInt64 bus = newDeviceConfig->StartBus; bus < newDeviceConfig->EndBus; bus++) {
			UIntPtr sumBus = (newDeviceConfig->BaseAddress) + (bus << 20);

			VirtualGetPhysicalAddress(sumBus);
			VirtualMemoryMap((VoidPtr)sumBus, (VoidPtr)sumBus);

			PciDeviceHeader* headerBus = (PciDeviceHeader*)sumBus;

			if (headerBus->DeviceId == 0) continue;
			if (headerBus->DeviceId == PCI_INVALID) continue;

			for (SizeT device = 0; device < PCI_DEVICE_COUNT; device++) {
				UIntPtr sumDevice = (newDeviceConfig->BaseAddress) + (device << 15);

				VirtualGetPhysicalAddress(sumDevice);
				VirtualMemoryMap((VoidPtr)sumDevice, (VoidPtr)sumDevice);

				PciDeviceHeader* headerDevice = (PciDeviceHeader*)sumDevice;

				if (headerDevice->DeviceId == 0) continue;
				if (headerDevice->DeviceId == PCI_INVALID) continue;
			
				for (SizeT func = 0; func < PCI_FUNC_COUNT; func++) {
					UIntPtr sumFunc = (newDeviceConfig->BaseAddress) + (device << 12);
					VirtualGetPhysicalAddress(sumFunc);
					VirtualMemoryMap((VoidPtr)sumFunc, (VoidPtr)sumFunc);

					PciDeviceHeader* headerFunc = (PciDeviceHeader*)sumFunc;

					if (headerFunc->DeviceId == 0) continue;
					if (headerFunc->DeviceId == PCI_INVALID) continue;

					if (headerFunc->Class == Class) {
						if (headerFunc->SubClass == SubClass) {
							if (headerFunc->ProgIf == ProgIf) {
								ConsoleLog("Found the header!\n");
								return headerFunc;
							}
						}
					}
				}		
			}
        }
	}

	return NULL;
}

VoidPtr PcpMakeRequest(const Char* argCmd, VoidPtr structPtr) {
	if (argCmd == NULL) return NULL;

	// "GET" request, doesn't need the deviceid and vendorid
	if (StringCompare(argCmd, "GET", StringLength("GET")) == 0) {
		PciReq* get = (PciReq*)structPtr;

		if (get == NULL) return NULL;
		if (get->response == NULL) return NULL;
		
		VoidPtr result = PcpGet(get->response->Class, 
								get->response->SubClass, 
								get->response->ProgIf);

		get->response->CallbackSlot1 = (UIntPtr)result;
		get->response->CallbackSlot5 = (result == NULL) ? PCP_FAIL : PCP_OK; // 200 : OK

		return result;
	}

	return NULL;
}