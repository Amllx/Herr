OUTPUT = $(KERNEL_ID).lib
LD_SCRIPT = Linkerscript.ld
CROSS_LD = x86_64-elf-ld
CROSS_CC = x86_64-elf-gcc	
ROOTLIB = Denver
LD_FLAGS = --script=$(LD_SCRIPT) -nostdlib -static 	-zmax-page-size=0x1000

SOURCES = $(wildcard $(ROOTDIR)/Source/*.c) $(wildcard $(ROOTDIR)/Source/Prekernel/*.c) $(wildcard $(ROOTDIR)/Source/DT/*.c) $(wildcard $(ROOTDIR)/Source/GraphicsLib/*.c) $(wildcard $(ROOTDIR)/Source/ELFLib/*.c)\
					$(wildcard $(ROOTDIR)/Source/HDILib/*.c) $(wildcard $(ROOTDIR)/Source/HILib/Intel/*.c) $(wildcard $(ROOTDIR)/Source/HILib/*.c) $(wildcard $(ROOTDIR)/Source/IOLib/*.c) $(wildcard $(ROOTDIR)/Source/MemLib/*.c)\
					$(wildcard $(ROOTDIR)/AHCI/*.c) $(wildcard $(ROOTDIR)/ATA/*.c) $(wildcard $(ROOTDIR)/UHCI/*.c) $(wildcard $(ROOTDIR)/ATAPI/*.c) $(wildcard $(ROOTDIR)/SATA/*.c) $(wildcard $(ROOTDIR)/Ethernet/*.c)\
					$(wildcard $(ROOTDIR)/Source/DevLib/*.c) $(wildcard $(ROOTDIR)/Source/SchedLib/*.c)

OBJECTS = $(wildcard *.o) $(wildcard $(ROOTLIB)/Source/HILib/Intel/*.o)
CC_INC = -I$(ROOTDIR)/Library -IBuiltinDriverLib
CC_FLAGS = -m64 -c -nostdlib -mcmodel=kernel -ffreestanding -Wall -Werror -fno-stack-protector -fno-stack-check -mgeneral-regs-only -mno-red-zone
ASM = nasm
ASM_FLAGS = -f elf64

.PHONY: kernbuild
kernbuild:
	$(CROSS_CC) $(CC_INC) $(BUILDOPT) $(CC_FLAGS) $(SOURCES)

.PHONY: kernlink
kernlink:
	$(CROSS_LD) $(LD_FLAGS) $(OBJECTS) -o $(OUTPUT)

.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(wildcard *.APP)

ifeq ($(ROOTDIR), $(ROOTLIB))

include $(ROOTLIB)/Optional.mk buildopt

endif