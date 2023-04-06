#include <GraphicsLib/Terminal.h>
#include <HILib/Intel/APIC.h>
#include <HILib/Intel/SMP.h>

extern void DisablePIC(void);

static struct ProcessorEntry* g_LocalApic[256]; // number of APIC detected by the OS.
static UInt8 g_NumCores = 0;  // number of cores detected

const UInt8 SmpNumCores(Void) { return g_NumCores; }

struct ProcessorEntry* SmpProcessorEntryGet(SizeT Index) {
    if (Index > 255)
        return NULL;

    return g_LocalApic[Index];
}

#ifndef SMP_EXTENDED_BIT
#define SMP_EXTENDED_BIT 'X'
#endif // !SMP_EXTENDED_BIT

Int32 SmpProbeApic(BootloaderHeader* bootHdr) {
    if (!bootHdr)
        return False;

    MADTHeader* madtHeader = ApicGetMadt(bootHdr);

    if (madtHeader) {
        DisablePIC();

        UInt8 *ptr = NULL;
        UInt8 *ptr2 = NULL;
        UInt32 len = 0;
        
        // A simple Iterator on each descriptor.
        // I was very lazy, so instead of casting a struct, we use offsets.

        for(len = *((UInt32*)(madtHeader + 4)), ptr2 = (UInt8*)madtHeader + 36; ptr2 < (UInt8*)madtHeader + len; ptr2 += ((UInt8*)madtHeader)[0] == SMP_EXTENDED_BIT ? 8 : 4) {
            // ^ iterate every entry, and jump according to the SMP_EXTENDED_BIT.

            // get the structure.
            ptr = (UInt8*)(UIntPtr)(((UInt8*)madtHeader)[0] == SMP_EXTENDED_BIT ? *((UInt64*)ptr2) : *((UInt32*)ptr2));

            if(StringCompare((Char*)ptr, APIC_IDENT, 4) == 0) { // is this the APIC?
                ConsoleLog("Found APIC controller....\n");
                g_LocalApic[g_NumCores] = MemAlloc(sizeof(struct ProcessorEntry));
                g_LocalApic[g_NumCores]->LocalApic = (UInt64)((ptr+0x24));

                ptr2 = ptr + *((UInt32*)(ptr + 4));
                for(ptr += 44; ptr < ptr2; ptr += ptr[1]) {
                    switch(ptr[0]) {
                    case 0: {
                        if(ptr[4] & 1)
                            g_LocalApic[g_NumCores]->iApicType |= LOCAL_APIC_TYPE;

                        break; // found Processor Local APIC
                    }
                    case 1: {
                        g_LocalApic[g_NumCores]->IoApic = (UInt64)*((UInt32*)(ptr+4));
                        g_LocalApic[g_NumCores]->iApicType |= IO_APIC_TYPE;

                        break; // io apic
                    }
                    case 5: {
                        g_LocalApic[g_NumCores]->LocalApic = *((UInt64*)(ptr+4));
                        g_LocalApic[g_NumCores]->iApicType |= TYPE_64BIT_APIC;

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