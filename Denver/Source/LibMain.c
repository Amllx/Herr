#include <HILib/Intel/InterruptService.h>
#include <HILib/Intel/Intel.h>
#include <GraphicsLib/Graphics.h>
#include <HILib/Intel/HPET.h>
#include <HILib/Intel/SMP.h>
#include <HILib/Intel/Fpu.h>
#include <DevLib/Device.h>
#include <DevLib/Helpers.h>
#include <StringUtils.h>
#include <HDILib/VFS.h>
#include <HDILib/HDI.h>
#include <IOLib/PCI.h>
#include <SATA/SATA.h>
#include <ATA/ATA.h>
#include <Time.h>

static UInt16 gControlWord = 0;

Boolean DoSSEAndX87Tests(void) {
	if (CpuFeature(CPU_FEATURE_EDX_APIC, True)) { ConsoleLog("This device is not compliant with the APIC.\n"); return False; }
	if (CpuFeature(CPU_FEATURE_EDX_SSE, True)) { ConsoleLog("This device does not support SSE, halted.\n"); return False; }
	
	if (!X87Test()) { ConsoleLog("x87 test failed!"); return False; }

	FpuLoadControlWord(gControlWord);

	return True;
}

struct BootStruct* gBootHdr = NULL;

void LibMain(BootloaderHeader* pBootHdr) {
	gBootHdr = pBootHdr;

	Console(gBootHdr);
	ConsoleEnableLogging(False);

	if (!MemInit(pBootHdr)) { ConsoleLog("Could not initialize the Mem API!\n"); Halt(); }
	if (!GLInit(pBootHdr)) { ConsoleLog("Could not initialize the GLDeviceContext!\n"); Halt(); }
	if (!IdtInit()) { ConsoleLog("Could not map interrupts!\n"); Halt(); }
	if (!DoSSEAndX87Tests()) { ConsoleLog("Processor specific features not found!\n"); Halt(); }
	if (!OpenHDI(pBootHdr)) { ConsoleLog("Could not open the HDI context!\n"); Halt(); }
	if (!HPETClockCreate(pBootHdr)) { ConsoleLog("Denver had some issues when initializing the MT!\n"); }
	if (SmpProbeApic(pBootHdr) < 1) { ConsoleLog("Could not detect more CPUs!\n"); }
	if (!PciFindMcfg(pBootHdr)) { ConsoleLog("Could not detect the MCFG!\n"); }

	while (True) { Cli(); Halt(); }
}
