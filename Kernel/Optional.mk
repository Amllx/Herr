.PHONY: buildopt
buildopt:
	$(ASM) $(ASM_FLAGS) $(ROOTDIR)/Source/HILib/Intel/GDT.x86_64.S
	$(ASM) $(ASM_FLAGS) $(ROOTDIR)/Source/HILib/Intel/IDT.x86_64.S
	$(ASM) $(ASM_FLAGS) $(ROOTDIR)/Source/HILib/Intel/PIC.x86_64.S
	$(ASM) $(ASM_FLAGS) $(ROOTDIR)/Source/HILib/Intel/INTEL.x86_64.S
	$(ASM) $(ASM_FLAGS) $(ROOTDIR)/Source/HILib/Intel/PAGING.x86_64.S