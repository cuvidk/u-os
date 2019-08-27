BIN_DIR=../bin
OBJ_DIR=$(BIN_DIR)/obj

ISO_DIR=$(BIN_DIR)/iso
BOOT_DIR=$(ISO_DIR)/boot
OS_BIN=$(BOOT_DIR)/u-os.bin
OS_ISO=$(BOOT_DIR)/u-os.iso

HEADERS=../include

AS_FLAGS=-felf64 -I$(HEADERS)
AS_SRC=boot.asm vga_print.asm
AS=../build-tools/nasm-2.14.02-linux/nasm

LD_FLAGS=-T ./linker.ld -nostdlib -lgcc -n
CC_FLAGS=-mno-red-zone -ffreestanding -O2 -Wall -Wextra -std=gnu99 -I$(HEADERS)
CC=../build-tools/x86_64-elf-cross-compiler/install/bin/x86_64-elf-gcc

all: os-bin
	grub-mkrescue -o $(OS_ISO) $(ISO_DIR)

os-bin: gen_as_objs gen_cc_objs
	$(CC) $(LD_FLAGS) $(CC_FLAGS) -o $(OS_BIN) $(OBJ_DIR)/*

gen_as_objs:
	$(AS) $(AS_FLAGS) -o $(OBJ_DIR)/boot.o boot.asm
	$(AS) $(AS_FLAGS) -o $(OBJ_DIR)/vga_print.o vga_print.asm
	$(AS) $(AS_FLAGS) -o $(OBJ_DIR)/segmentation.o segmentation.asm
	$(AS) $(AS_FLAGS) -o $(OBJ_DIR)/paging.o paging.asm
	$(AS) $(AS_FLAGS) -o $(OBJ_DIR)/isr0.o isr.asm
	$(AS) $(AS_FLAGS) -o $(OBJ_DIR)/trap_frame0.o trap_frame.asm
	$(AS) $(AS_FLAGS) -o $(OBJ_DIR)/keyboard0.o keyboard.asm

gen_cc_objs: $(CSRC_FILES) 
	$(CC) $(CC_FLAGS) -o $(OBJ_DIR)/kernel.o -c kernel.c
	$(CC) $(CC_FLAGS) -o $(OBJ_DIR)/vga.o -c vga.c
	$(CC) $(CC_FLAGS) -o $(OBJ_DIR)/string.o -c string.c
	$(CC) $(CC_FLAGS) -o $(OBJ_DIR)/kprint.o -c kprint.c
	$(CC) $(CC_FLAGS) -o $(OBJ_DIR)/xform.o -c xform.c
	$(CC) $(CC_FLAGS) -o $(OBJ_DIR)/io.o -c io.c
	$(CC) $(CC_FLAGS) -o $(OBJ_DIR)/pic.o -c pic.c
	$(CC) $(CC_FLAGS) -o $(OBJ_DIR)/idt.o -c idt.c
	$(CC) $(CC_FLAGS) -o $(OBJ_DIR)/isr.o -c isr.c
	$(CC) $(CC_FLAGS) -o $(OBJ_DIR)/trap_frame.o -c trap_frame.c
	$(CC) $(CC_FLAGS) -o $(OBJ_DIR)/keyboard.o -c keyboard.c

clean: clean-obj clean-os

clean-os:
	rm -rf $(OS_BIN)
	rm -rf $(OS_ISO)

clean-obj:
	rm -rf $(OBJ_DIR)/*.o
