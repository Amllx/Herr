#include <HILib/Intel/InterruptService.h>
#include <GraphicsLib/Graphics.h>
#include <HILib/Intel/Intel.h>
#include <DTLib/Dump.h>

static IDT* Idt = NULL;
static IDTReg* IdtReg = NULL;

extern UIntPtr pInterruptVectorTable[];
extern void LdIdt(IDTReg* reg);

void AddInt(UIntPtr isr, UInt8 attr, UInt8 index) {
    Idt[index].isrLow = isr & 0xFFFF;
    Idt[index].kernelCs = 0x28; /* Hard-coded code segment, BAD! */
    Idt[index].ist = 0;
    Idt[index].attributes = attr;
    Idt[index].isrMid = isr >> 16 & 0xFFFF;
    Idt[index].isrHigh = isr >> 32 & 0xFFFFFFFF;
    Idt[index].reserved = 0;
}

Boolean IdtInit(void) {
    Idt = MemAlloc(sizeof(IDT) * IDT_MAX_INTERRUPTS);
	if (Idt == NULL) return False;
	
    for (UInt16 index = 0; index < 255; index++)
        AddInt(pInterruptVectorTable[index], IDT_GATE, index);

    AddInt(pInterruptVectorTable[66], IDT_TRAP_GATE, 66);

    IdtReg = MemAlloc(sizeof(IDTReg));
	if (IdtReg == NULL) return False;

    IdtReg->base = (UIntPtr)&Idt[0];
    IdtReg->limit = (UIntPtr)sizeof(IDT) * 256 - 1;

    LdIdt(IdtReg);
	return True;
}

static Char* ExceptionMessage[32] = {
	"Division by zero",
	"Debug Breakpoint",
	"Non-maskable interrupt",
	"Breakpoint",
	"Detected overflow",
	"Out-of-bounds",
	"Invalid opcode",
	"No coprocessor",
	"Double fault",
	"Coprocessor segment overrun",
	"Bad TSS",
	"Segment not found",
	"Stack error.",
	"General Protection Fault",
	"Page Fault",
	"Invalid interrupt",
	"Coprocessor fault",
	"Alignment check",
	"Machine check",
	"Reserved",
	"Reserved",
	"System Process Switch Issued",
	"System was interrupted by kernel",
	"System hang by kernel",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
};

static StackFrame* gCurrentStack = NULL;

StackFrame* CurrentStack(Void) { return gCurrentStack; }

extern UIntPtr HandleInterrupts(UIntPtr rsp) {
	Cli();

	gCurrentStack = (StackFrame*)rsp;

	if (gCurrentStack->intNum == 0x25) {
		ConsoleLog("ASSERTION FAILED!");
		MemDump(gCurrentStack->intNum, rsp);

		Halt();
	}

	if (gCurrentStack->intNum < 32)
		ConsoleLog(ExceptionMessage[gCurrentStack->intNum]);

	ISSendEoi(gCurrentStack->intNum);
	Sti();

	return rsp;
}