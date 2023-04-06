#include <GraphicsLib/Terminal.h>
#include <HILib/Intel/HPET.h>
#include <MemLib/IO.h>
#include <DTLib/Assert.h>
#include <DTLib/Result.h>

#define CLOCK_HPET_GENERAL_CAPS_AND_ID 0x00
#define CLOCK_HPET_GENERAL_CONFIG 0x10
#define CLOCK_HPET_MAIN_COUNTER 0xf0

static UIntPtr gBaseAddressAddr = 0;
static Boolean gEnabled = False;
static ClockPeriod gPeriod = 0;

static 
void 
ClockWrite(UIntPtr reg, UIntPtr val);

static 
UInt64
ClockRead(UIntPtr reg);

static 
UInt64 
ClockRead(UIntPtr reg) {
    if (reg == 0) return gBaseAddressAddr;
    return gBaseAddressAddr + reg; 
}

static 
void 
ClockWrite(UIntPtr reg, UIntPtr val) {
    volatile UIntPtr* ptr = (volatile UIntPtr*)gBaseAddressAddr + reg;
    *ptr = val;
}

// need to master hpet address!
Boolean
InternalHPETClockEnable(HPETClockHeader* hpet) {
    Result = CLOCK_HPET_NOT_PRESENT;

    gBaseAddressAddr = (UIntPtr)MemAllocIO(hpet->baseAddress, MMIO_PAGE_PROTECT);
    
    ClockWrite(CLOCK_HPET_MAIN_COUNTER, 1);
    UInt8 config = ClockRead(CLOCK_HPET_LEGACY_REPLACEMENT);
    ClockWrite(CLOCK_HPET_GENERAL_CONFIG, config);

    gPeriod = ClockRead(CLOCK_HPET_GENERAL_CAPS_AND_ID) >> 32;
    ConsoleLog("%i %n", gPeriod);

    Result = CLOCK_HPET_ENABLED;

    gEnabled = True;
    return gEnabled;
}

Boolean HPETClockCreate(BootloaderHeader *bs) {
    RSDP* rsdp = AcpiGetRsdp(bs);
    if (rsdp == NULL) return False;
    
    HPETClockHeader* hpet = AcpiFind(rsdp, CLOCK_HPET_SIGNATURE);
    if (hpet == NULL) return False;

    return InternalHPETClockEnable(hpet);
}

Boolean HPETClockEnabled(void) { return gEnabled; }

void HPETSleep(ClockPeriod delay) {
    if (!HPETClockEnabled()) return;
    if (gPeriod == 0) return;
    
    ClockPeriod target = ClockRead(CLOCK_HPET_MAIN_COUNTER) + (delay) / gPeriod;
    while (ClockRead(CLOCK_HPET_MAIN_COUNTER) < target) { /* Do nothing, spin */ }
}
