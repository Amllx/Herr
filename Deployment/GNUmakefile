.PHONY: isofs
isofs:
	xorriso -as mkisofs -b limine-cd.bin \
	-no-emul-boot -boot-load-size 4 -boot-info-table \
	--efi-boot limine-cd-efi.bin \
	-efi-boot-part --efi-boot-image --protective-msdos-label \
	bin/ -o boot.iso

LimineDeploy.elf: Deploy.c
	$(CC) $(CFLAGS) $(LDFLAGS) -std=c99 Deploy.c -o $@