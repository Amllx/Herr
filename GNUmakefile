# QEMU Makefile.
# Written by Amlal El Mahrouss.

export BIN = DEPLOY/bin/ DEPLOY/LimineDeploy.elf DEPLOY/boot.iso disk/
export CACHE_DIR = DEPLOY/bin/boot/cache
export BOOT = DEPLOY/bin/efi/boot/
export QEMU = qemu-system-x86_64
export QEMU_FLAGS = -machine q35 -smp cores=4 -m 4G -device pvpanic

$(BOOT)/BOOTX64.EFI:
	mkdir -p $(BOOT)
	wget https://github.com/Amllx/tmp/raw/main/BOOTX64.EFI -O $@

.PHONY: run-debug
run-debug: $(BOOT)/BOOTX64.EFI
	$(QEMU) $(QEMU_FLAGS) -hda disk.img -cdrom Deployment/boot.iso -d int

.PHONY: run
run: $(BOOT)/BOOTX64.EFI
	$(QEMU) $(QEMU_FLAGS) -hda disk.img -cdrom Deployment/boot.iso

.PHONY: disk
disk:
	qemu-img create -f qcow2 disk.img 512M

.PHONY: clean
clean:
	sudo rm -rf $(BIN)
