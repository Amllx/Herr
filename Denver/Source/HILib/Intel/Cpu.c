#include <HILib/Intel/Cpu.h>
#include <StringUtils.h>
#include <System.h>

#ifdef __x86_64__

void Out8(UInt16 port, UInt8 value) { asm volatile ("outb %%al, %1"  : : "a" (value), "Nd" (port) : "memory"); }
void Out16(UInt16 port, UInt8 value) { asm volatile ("outw %%ax, %1"  : : "a" (value), "Nd" (port) : "memory"); }
void Out32(UInt16 port, UInt32 value) { asm volatile ("outl %%eax, %1" : : "a" (value), "Nd" (port) : "memory"); }

UInt8 In8(UInt16 port) {
    UInt8 value;
    asm volatile ("inb %1, %%al"  : "=a" (value) : "Nd" (port) : "memory");
    return value;
}

UInt16 In16(UInt16 port) {
    UInt16 value;
    asm volatile ("inw %1, %%ax"  : "=a" (value) : "Nd" (port) : "memory");
    return value;
}

UInt32 In32(UInt16 port) {
    UInt32 value;
    asm volatile ("inl %1, %%eax" : "=a" (value) : "Nd" (port) : "memory");
    return value;
}

#define EBX_INTEL 0x756E6547
#define EBX_AMD 0x68747541
#define EBX_NULL 0x00000000

#define QEMU_ID  "QEMU Project Cpu."
#define INTEL_ID "Intel Corpoation."
#define AMD_ID "AMD Corporation."

CPU CpuGetVendor(void) {
    UInt32 ebx = 0;
    UInt32 unused = 0;

    CPU ident = { .iVendorId = EBX_NULL, .strVendor = QEMU_ID }; // Surely the most possible case
    CpuId(unused, 0, &ebx, 0, 0);

    switch (ebx) {
        case EBX_INTEL: {
            CopyMem(INTEL_ID, ident.strVendor, StringLength(INTEL_ID));
            ident.iVendorId = EBX_INTEL;
			break;
        }
        case EBX_AMD: {
            CopyMem(AMD_ID, ident.strVendor, StringLength(AMD_ID));
            ident.iVendorId = EBX_AMD;
			break;
        }
    }

    return ident;
}

Boolean CpuFeature(UInt32 offset, Boolean isEdx) {
	UInt32 leaf = 0, eax = 0, ebx = 0, ecx = 0, edx = 0;
	CpuId(leaf, &eax, &ebx, &ecx, &edx);

	return ((isEdx ? edx : ecx) & offset);
}

#else
#error This target is not supported by Denver!
#endif