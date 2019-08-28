# u-os

"Toy" os project developed for masters class. Was compiled and tested with nasm 2.14.20-linux and gcc-7.4.0-elf x86-64 compiler. There's a script in build-tools that helps with the generation of the gcc cross-compiler (should be compatible with systems employing apt as a package manager).

## The project contains / handles:
* multiboot with grub
* 32 bit segmentation (gdt32)
* enabling 4 level paging and identity mapping the first 2MB of memory
* transitioning to long mode and setting up gdt64
* PIC programming
* setting up IDT
* installing interrupt gates for architectural defined exceptions / interrupts
* dumping trap frame upon exception encounter
* a minimalistic keyboard driver
