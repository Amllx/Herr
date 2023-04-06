#pragma once

#include <Lib.h>
#include <System.h>

#define IDT_GATE (0x8E)
#define IDT_USER (0b01100000)

#define IDT_TRAP_GATE       (0xEF)
#define IDT_MAX_INTERRUPTS (0x100)

#define LDT_ADDRESS     (0x2)

#define TSS_AVAILABLE   (0x9)
#define TSS_BUSY        (0xB)

#define GDT_DESCRIPTOR_ACCESS       (0x01)
#define GDT_DESCRIPTOR_READWRITE    (0x02)
#define GDT_DESCRIPTOR_DC           0x04
#define GDT_DESCRIPTOR_EXECUTABLE   0x08
#define GDT_DESCRIPTOR_CODE_DATA    0x10
#define GDT_DESCRIPTOR_DPL          0x60
#define GDT_DESCRIPTOR_PRESENT      0x80
#define GDT_GRANULARITY_OS          0x10
#define GDT_GRANULARITY_X64         0x20
#define GDT_GRANULARITY_X32         0x40
#define GDT_GRANULARITY_4K          0x80

#define GDT_OFFSET(OFFSET, LEVEL)   (OFFSET * LEVEL) /* Assuming offset is a valid GDT address. */

#define GDT_OFFSET_KERNEL_CODE      GDT_OFFSET(GDT_DESCRIPTOR_EXECUTABLE, 1)
#define GDT_OFFSET_KERNEL_DATA      GDT_OFFSET(GDT_DESCRIPTOR_CODE_DATA, 2)
#define GDT_OFFSET_USER_DATA        GDT_OFFSET(GDT_DESCRIPTOR_CODE_DATA, 3)
#define GDT_OFFSET_USER_CODE        GDT_OFFSET(GDT_DESCRIPTOR_EXECUTABLE, 4)

typedef struct IDT {
    UInt16 isrLow;
    UInt16 kernelCs;
    UInt8 ist;
    UInt8 attributes;
    UInt16 isrMid;
    UInt32 isrHigh;
    UInt32 reserved;
} Attribute((packed)) IDT;

typedef struct IDTReg {
    UInt16 limit;
    UInt64 base;
} Attribute((packed)) IDTReg;

typedef struct GDT {
    UInt16 limit;
    UInt16 baseLow;
    UInt8  baseMid;
    UInt8  access;
    UInt8  granularity;
    UInt8  baseHigh;
} Attribute((packed)) GDT;

extern void LdGdt(GDT* dt);

Boolean IdtInit(void);
Boolean GdtInit(void);

void AddInt(UIntPtr isr, UInt8 attr, UInt8 index);
StackFrame* CurrentStack(Void);