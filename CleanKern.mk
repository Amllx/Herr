.PHONY: clean
clean:
	rm -rf $(wildcard *.LIB) $(wildcard *.o) $(wildcard KernelLib/Source/MemLib/*.o)\
	$(wildcard KernelLib/Source/HILib/Intel/*.o)