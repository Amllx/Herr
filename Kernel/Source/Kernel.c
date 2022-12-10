#include <HILib/Intel/InterruptService.h>
#include <HILib/Intel/Intel.h>
#include <GraphicsLib/Graphics.h>
#include <HILib/Intel/HPET.h>
#include <HILib/Intel/SMP.h>
#include <HILib/Intel/Fpu.h>
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
	if (!CpuFeature(CPU_FEATURE_EDX_APIC, True)) { ConsoleLog("APIC test failed!"); return False; }
	if (!CpuFeature(CPU_FEATURE_EDX_SSE, True)) { ConsoleLog("SSE cpuid failed!\n"); }
	
	if (!X87Test()) { ConsoleLog("x87 test failed!"); return False; }

	Int32 result = SSETest();
	if (result == 0) { 
		ConsoleLog("SSE test failed!"); 
		return False; 
	}  else { 
		SSEEnable(); 
	}

	if (CpuFeature(CPU_FEATURE_ECX_AVX, False)) {
		AVXEnable();
	} else {
		ConsoleLog("AVX was not found on this CPU...\n");
	}

	FpuLoadControlWord(gControlWord);

	return True;
}

static struct DeviceContext* gDevCtx = NULL;
struct BootStruct* gBootHdr = NULL;

Boolean SetLoadingBackground(Void) {
	gDevCtx->fGraphicsWriteRect(gDevCtx, 1920, 1080,  0x50, 0x05, 0x11);

	return True;
}

void Main(BootloaderHeader* pBootHdr) {
	gBootHdr = pBootHdr;

	Console(gBootHdr);
	ConsoleEnableLogging(True);

	Boolean mcfg = False;

	if (!VirtualEnablePaging()) { ConsoleLog("Could not initialize the Virtual API!\n"); Halt(); }
	if (!MemInit(pBootHdr)) { ConsoleLog("Could not initialize the Mem API!\n"); Halt(); }
	if (!GraphicsInit(pBootHdr)) { ConsoleLog("Could not initialize the DeviceContext!\n"); Halt(); }
	if (!IdtInit()) { ConsoleLog("Could not map interrupts!\n"); Halt(); }
	if (!DoSSEAndX87Tests()) { ConsoleLog("DoSSEAndX87Tests failed!\n"); Halt(); }
	if (!OpenHDI(pBootHdr)) { ConsoleLog("Could not open the HDI context!\n"); Halt(); }
	if (!HPETClockCreate(pBootHdr)) { ConsoleLog("Denver had some issues when initializing the MT!\n"); }
	if (SmpProbeApic(pBootHdr) < 1) { ConsoleLog("Could not detect more CPUs!\n"); }
	if (!(mcfg = PciFindMcfg(pBootHdr))) { ConsoleLog("Could not detect the MCFG!\n"); }

	HDIContext* context = HDICreateContext(T("A:"), 0, True);
	HDIGetPIODiskIO(context);

	UInt16* MICROSOFT = AllocWideString(T("MICROSOFT CORPORATION CORPORATION CORPORATION CORPORATION CORPORATION CORPORATION CORPORATION CORPORATION CORPORATION CORPORATION CORPORATION CORPORATION CORPORATION CORPORATION CORPORATION CORPORATION CORPORATION CORPORATION CORPORATION CORPORATION CORP"));
	context->fWrite(context, 512, 10, MICROSOFT);
	UInt16* bytes = context->fRead(context, 512, 10);
	
	for (SizeT i = 0; i < 256 * 10; i++)
	{
		ConsoleLogHex(bytes[i]);
	}
	
	if (mcfg) 
		AhciInitDriver();

	while (True) { Cli(); Halt(); }
}
