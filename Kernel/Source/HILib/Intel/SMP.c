#include <GraphicsLib/Terminal.h>
#include <HILib/Intel/APIC.h>
#include <HILib/Intel/SMP.h>

extern void DisablePIC(void);

static UInt8 g_NumCores = 0;          // number of cores detected
static struct ProcessorEntry* g_LocalApic[256];

const UInt8 SmpNumCores(Void) { return g_NumCores; }

struct ProcessorEntry* SmpProcessorEntryGet(SizeT Index) {
    Check(Index < 256, "Bad Processor entry...\n");
    return g_LocalApic[Index];
}

#ifndef SMP_EXTENDED_BIT
#define SMP_EXTENDED_BIT 'X'
#endif // !SMP_EXTENDED_BIT

Int32 SmpProbeApic(BootloaderHeader* pBootHdr) {
    if (!pBootHdr) return False;
    APICHeader* hdr = ApicGetMadt(pBootHdr);
    if (hdr) {
        DisablePIC();
        ConsoleLog("Disabled PIC, finding MP entries..\n");

        UInt8 *ptr = NULL;
        UInt8 *ptr2 = NULL;
        UInt32 len = 0;
        
        // A simple Iterator on each descriptor.
        // i was very lazy, so instead of casting a struct, we use offsets.

        for(len = *((UInt32*)(hdr + 4)), ptr2 = (UInt8*)hdr + 36; ptr2 < (UInt8*)hdr + len; ptr2 += ((UInt8*)hdr)[0] == SMP_EXTENDED_BIT ? 8 : 4) {
            // ^ iterate every entries, and jump according to the SMP_EXTENDED_BIT.

            // get the structure.
            ptr = (UInt8*)(UIntPtr)(((UInt8*)hdr)[0] == SMP_EXTENDED_BIT ? *((UInt64*)ptr2) : *((UInt32*)ptr2));
            if(!StringCompare((Char*)ptr, APIC_IDENT, 4)) { // is this the APIC?
                ConsoleLog("Found APIC controller....\n");
                g_LocalApic[g_NumCores] = MemAlloc(sizeof(struct ProcessorEntry));
                g_LocalApic[g_NumCores]->LocalApic = (UInt64)((ptr+0x24));

                ptr2 = ptr + *((UInt32*)(ptr + 4));
                for(ptr += 44; ptr < ptr2; ptr += ptr[1]) {
                    switch(ptr[0]) {
                    case 0: {
                        if(ptr[4] & 1)
                            g_LocalApic[g_NumCores]->iApicType |= LOCAL_APIC_TYPE;

                        ConsoleLog("Found Local APIC....\n");
                        break; // found Processor Local APIC
                    }
                    case 1: {
                        g_LocalApic[g_NumCores]->IoApic = (UInt64)*((UInt32*)(ptr+4));
                        g_LocalApic[g_NumCores]->iApicType |= IO_APIC_TYPE;

                        ConsoleLog("Found Local IO-APIC....\n");
                        break; // io apic
                    }
                    case 5: {
                        g_LocalApic[g_NumCores]->LocalApic = *((UInt64*)(ptr+4)); 
                        g_LocalApic[g_NumCores]->iApicType |= TYPE_64BIT_APIC;

                        ConsoleLog("Found 64 APIC....\n");
                        break; // 64 bit APIC
                    }         // found 64 bit LAPIC
                    }

                    ++g_NumCores;
                }
                break;
            }
        }

        return g_NumCores;
    }

    return 0;
}