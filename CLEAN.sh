# !/bin/zsh

make -f CleanKern.mk clean
rm -f disk.img
rm -f *.o
rm -f Kernel/Source/HILib/Intel/*.o