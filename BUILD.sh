# !/bin/zsh

export ROOTDIR=Kernel
export BUILDOPT="-IKernel/Library -IBuiltinDriver -IBuiltinDriver/AHCI -IBuiltinDriver/ATA -IBuiltinDriver/SATA -IBuiltinDriver/UHCI -IBuiltinDriver/Ethernet -IBuiltinDriver/ATAPI"
make -f KernelBuilder.mk kernbuild

export ROOTDIR=BuiltinDriver
export BUILDOPT="-IKernel/Library -IBuiltinDriver -IBuiltinDriver/AHCI -IBuiltinDriver/ATA -IBuiltinDriver/SATA -IBuiltinDriver/UHCI -IBuiltinDriver/Ethernet -IBuiltinDriver/ATAPI -IKernel/Library"
make -f KernelBuilder.mk kernbuild

export KERNEL_ID="DENVER64"
make -f KernelBuilder.mk kernlink