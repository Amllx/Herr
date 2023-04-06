#include <DT/Result.h>
#include <IOLib/ACPI.h>
#include <GraphicsLib/Terminal.h>

RSDP* AcpiGetRsdp(BootloaderHeader* pBootHdr)
{
	if (pBootHdr == NULL) return NULL;
    struct TagRsdp* rsdp = BootloaderTag(pBootHdr, EKBOOT_STRUCT_TAG_RSDP_ID);
    Check(rsdp != NULL, "RSDP is NULL!");

    Result = ERR_SUCCESS;
    if (rsdp != NULL) return (RSDP*)rsdp->rsdp;
	
	Result = ERR_BAD_ACCESS; // Bad access here, means that the rsdp does not exist here.
    return NULL;
}

Boolean AcpiChecksum(SDT* tableHeader)
{
    UInt8 sum = 0;
    for (Int32 i = 0; i < tableHeader->length; i++)
    {
        sum += ((Char *) tableHeader)[i];
    }
 
    return sum == 0;
}

VoidPtr AcpiFind(RSDP* rsdp, Char* signature) {
#ifdef __x86_64__
	UInt64 entries = 5;
	SDT* sdt = NULL;

	if (rsdp->Revision >= 2) {
		ConsoleLog("ACPI 2.0+ has been detected!\n");

		sdt = (SDT*)(rsdp->XsdtAddress >> PAGE_FRAME_INDEX(rsdp->XsdtAddress));
		entries = ((sdt->length + sizeof(SDT)) / 8); // Since we are in 64-Bit divide by 8
	} else {
		ConsoleLog("ACPI 1.0 has been detected!\n");

		sdt = (SDT*)(UIntPtr)(rsdp->RsdtAddress >> PAGE_FRAME_INDEX(rsdp->RsdtAddress));
	}

	if (!AcpiChecksum(sdt))
		return NULL;

	ConsoleLog("%s %i %n", "This RSD PTR has this number of entries: ", entries);
	
	for (UInt64 index = 0; index < entries; index++) {
		sdt = (SDT*)*((UInt64*)(UInt64)sdt + sizeof(SDT) + (index * 8));

		for (int signature_index = 0; signature_index < 4; signature_index++){
			if (sdt->signature[signature_index] != signature[signature_index])
				break;

			if (signature_index == 3) return sdt;
		}
	}
#else
#	error Not implemented!
#endif // !__x86_64__

	ConsoleLog("%s %s %n", signature, " was not found!");
	return NULL;
}
