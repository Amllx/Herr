#pragma once

#include <Lib.h>
#include <System.h>

typedef UIntPtr EFFileOff;

typedef struct EFContainer {
    UInt32 Magic;
    UInt32 Version;
    UInt32 Type;
    UInt32 Cpu;
    UInt32 SubCpu;
    EFFileOff StartSections;
    SizeT SectionsSz;
    SizeT SectionsCount;
} EFContainer;

#define EF_MAGIC_PPC (0xBAF0)