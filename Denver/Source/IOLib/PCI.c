#include <GraphicsLib/Terminal.h>
#include <IOLib/PCI.h>

Void PciInternalSetCfgTarget(UInt16 bus, UInt16 device, UInt8 function, UInt16 bar) {
	UInt32 target = 0x80000000 | ((UInt32)bus << 16) | ((UInt32)device << 11) | ((UInt32)function << 8) | (bar & ~3);
	Out32(PCI_CONFIG_ADDRESS, target);
}

UInt32 PciReadCfg(UInt16 bus, UInt16 device, UInt8 function, UInt16 bar, SizeT size) {
	PciInternalSetCfgTarget(bus, device, function, bar);

	if (size == 4) return In32(PCI_CONFIG_DATA + (bar & 3));
	if (size == 2) return In16(PCI_CONFIG_DATA + (bar & 3));
	if (size == 1) return In8(PCI_CONFIG_DATA + (bar & 3));

	return PCI_INVALID;
}

Void PciWriteCfg(UInt16 bus, UInt16 device, UInt8 function, UInt16 bar, SizeT data, SizeT size) {
	PciInternalSetCfgTarget(bus, device, function, bar);

	if (size == 4) Out32(PCI_CONFIG_DATA + (bar & 3), (UInt32)data);
	if (size == 2) Out16(PCI_CONFIG_DATA + (bar & 3), (UInt16)data);
	if (size == 1) Out8(PCI_CONFIG_DATA + (bar & 3), (UInt8)data);
}

UInt32 PciInternalReadRaw(UInt16 bus, UInt16 device, UInt8 function, UInt16 bar) {
	UInt32 target = 0x80000000 | ((UInt32)bus << 16) | ((UInt32)device << 11) | ((UInt32)function << 8) | (bar & 0xFC);

	Out32(PCI_CONFIG_ADDRESS, target);
	return In32(PCI_CONFIG_DATA);
}

UInt16 PciVendorId(UInt8 bus, UInt8 device, UInt8 function) { return (UInt16)PciInternalReadRaw(bus, device, function, 0x0); }
UInt16 PciInterfaceId(UInt8 bus, UInt8 device, UInt8 function) { return (UInt16)(PciInternalReadRaw(bus, device, function, 0x0) >> 16); }
UInt8 PciClass(UInt8 bus, UInt8 device, UInt8 function) { return (UInt8)(PciInternalReadRaw(bus, device, function, 0x08) >> 24); }
UInt8 PciSubclass(UInt8 bus, UInt8 device, UInt8 function) { return (UInt8)(PciInternalReadRaw(bus, device, function, 0x08) >> 16); }
UInt8 PciProgIf(UInt8 bus, UInt8 device, UInt8 function) { return (UInt8)(PciInternalReadRaw(bus, device, function, 0x08) >> 8); }
UInt8 PciHeaderType(UInt8 bus, UInt8 device, UInt8 function) { return (UInt8)(PciInternalReadRaw(bus, device, function, 0xC) >> 16); }

Void PciEnableMmio(UInt8 bus, UInt8 device, UInt8 function, UInt32 bar) {
	UInt8 enableBit = PciReadCfg(bus, device, function, bar, sizeof(UInt8)) | (1 << 1);
	PciWriteCfg(bus, device, function, bar, enableBit, sizeof(UInt16));
}

Void PciBecomeBusMaster(UInt16 bus, UInt16 device, UInt8 function, UInt32 bar) {
	UInt16 enableBit = PciReadCfg(bus, device, function, bar, sizeof(UInt16)) | (1 << 2);
	PciWriteCfg(bus, device, function, bar, enableBit, sizeof(UInt16));
}

UInt16 PciVendor(UInt16 bus, UInt16 device, UInt8 function) {
	UInt16 vendor = PciVendorId(bus, device, function);

	if (vendor != PCI_INVALID)
		device = (UInt16)PciReadCfg(bus, device, function, 0x0, sizeof(UInt16));

	return device;
}

MCFGHeader* gMcfg = NULL;

Boolean 
PciFindMcfg(BootloaderHeader* pBootHdr) 
{
	if (pBootHdr) {
		MCFGHeader* mcfg = AcpiFind(AcpiGetRsdp(pBootHdr), "MCFG");
		if (mcfg == NULL) return False;
		gMcfg = mcfg;
		
		return True;
	}

	return False;
}

MCFGHeader* PciGetMcfg(Void) {
	Check(NotNull(gMcfg), "MCFG is NULL!");
	return gMcfg;
}