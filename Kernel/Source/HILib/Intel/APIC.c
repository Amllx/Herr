#include <HILib/Intel/APIC.h>

Boolean ApicIsDetected(void) {
    return CpuFeature(CPU_FEATURE_EDX_APIC, True);
}

MADTHeader* ApicGetMadt(BootloaderHeader* pBootHdr) {
   if (pBootHdr != NULL) {
       RSDP* apicDesc = AcpiGetRsdp(pBootHdr);
        MADTHeader* header = AcpiFind(apicDesc, APIC_IDENT);
        return header;
   }

   return NULL;
}
