NASM_LINUX="./nasm-2.14.02-linux/nasm"
NASM_MACOS="./nasm-2.14.02-macos/nasm"
NASM=$(shell uname | grep "Linux" >/dev/null && echo $(NASM_LINUX) || echo $(NASM_MACOS))

BOOTLOADER_IN = "bootloader.asm"
BOOTLOADER_OUT = "bootloader.img"

bootloader:
	$(NASM) $(BOOTLOADER_IN) -o $(BOOTLOADER_OUT) -fbin

bootloader-clean:
	rm -rf $(BOOTLOADER_OUT)
