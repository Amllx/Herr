#!//bin/zsh

wget "https://github.com/limine-bootloader/limine/raw/v3.4.6-binary/limine-cd-efi.bin" -O limine-cd-efi.bin
wget "https://github.com/limine-bootloader/limine/raw/v3.4.6-binary/limine-cd.bin" -O limine-cd.bin
wget "https://github.com/limine-bootloader/limine/raw/v3.4.6-binary/limine.sys" -O limine.sys

mv limine.sys bin/
mv limine-cd.bin bin/
mv limine-cd-efi.bin bin/