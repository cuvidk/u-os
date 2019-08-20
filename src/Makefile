BIN_DIR=../bin
OBJ_DIR=$(BIN_DIR)/obj

ISO_DIR=$(BIN_DIR)/iso
BOOT_DIR=$(ISO_DIR)/boot
OS_BIN=$(BOOT_DIR)/u-os.bin
OS_ISO=$(BOOT_DIR)/u-os.iso

AS_FLAGS=-felf64 -I../include
AS_SRC=boot.asm vga_print.asm
AS=../build-tools/nasm-2.14.02-linux/nasm

LD_FLAGS=-T ./linker.ld -nostdlib -lgcc -n
CC_FLAGS=-mno-red-zone -ffreestanding -O2 -Wall -Wextra -std=gnu99
CC=../build-tools/x86_64-elf-cross-compiler/install/bin/x86_64-elf-gcc

all: os-bin
	grub-mkrescue -o $(OS_ISO) $(ISO_DIR)

os-bin: boot.o vga_print.o segmentation.o paging.o
	$(CC) $(LD_FLAGS) $(CC_FLAGS) -o $(OS_BIN) $(OBJ_DIR)/*

boot.o: boot.asm
	$(AS) $(AS_FLAGS) -o $(OBJ_DIR)/$@ $<

vga_print.o: vga_print.asm
	$(AS) $(AS_FLAGS) -o $(OBJ_DIR)/$@ $<

segmentation.o: segmentation.asm
	$(AS) $(AS_FLAGS) -o $(OBJ_DIR)/$@ $<

paging.o: paging.asm
	$(AS) $(AS_FLAGS) -o $(OBJ_DIR)/$@ $<

clean: clean-obj clean-os

clean-os:
	rm -rf $(OS_BIN)
	rm -rf $(OS_ISO)

clean-obj:
	rm -rf $(OBJ_DIR)/*.o
