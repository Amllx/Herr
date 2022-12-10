#include <HILib/Intel/APIC.h>

Boolean ApicIsDetected(void) {
    return CpuFeature(CPU_FEATURE_EDX_APIC, True);
}

APICHeader* ApicGetMadt(BootloaderHeader* pBootHdr) {
   if (pBootHdr != NULL) {
       RSDP* apicDesc = AcpiGetRsdp(pBootHdr);
        APICHeader* header = AcpiFind(apicDesc, APIC_IDENT);
        return header;
   }

   return NULL;
}
