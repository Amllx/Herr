# QEMU Makefile.
# Written by Amlal El Mahrouss.

export BIN = Deployment/bin/ Deployment/LimineDeploy.elf Deployment/boot.iso disk/
export CACHE_DIR = DEPLOY/bin/boot/cache
export BOOT = DEPLOY/bin/efi/boot/
export QEMU = qemu-system-x86_64
export QEMU_FLAGS = -machine q35 -smp cores=4 -m 4G -device pvpanic

$(BOOT)/BOOTX64.EFI:
	mkdir -p $(BOOT)
	wget https://github.com/limine-bootloader/limine/raw/v3.0-branch-binary/BOOTX64.EFI -O $@

$(CACHE_DIR)/OVMF.fd:
	mkdir -p $(CACHE_DIR)
	wget https://retrage.github.io/edk2-nightly/bin/DEBUGX64_OVMF.fd -O $@

.PHONY: run-debug
run-debug: $(BOOT)/BOOTX64.EFI $(CACHE_DIR)/OVMF.fd
	$(QEMU) $(QEMU_FLAGS) -hda disk.img -cdrom Deployment/boot.iso -bios $(CACHE_DIR)/OVMF.fd -d int

.PHONY: run
run: $(BOOT)/BOOTX64.EFI $(CACHE_DIR)/OVMF.fd
	$(QEMU) $(QEMU_FLAGS) -hda disk.img -cdrom Deployment/boot.iso -bios $(CACHE_DIR)/OVMF.fd

.PHONY: disk
disk:
	qemu-img create -f qcow2 disk.img 512M

.PHONY: clean
clean:
	sudo rm -rf $(BIN)
