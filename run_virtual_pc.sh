#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: ./run_virtual_pc.sh <os_image>"
    exit 1
fi

OS_IMAGE="$1"

#QEMU emulator version 2.11.1(Debian 1:2.11+dfsg-1ubuntu7.12)
QEMU_SYSTEM=qemu-system-x86_64

CONFIG_OUT_FILE="${QEMU_SYSTEM}.conf"
MEMORY="512M"
BOOT_ORDER="abd" #floppy1, floppy2, first CD-ROM
VGA="std"
TIMESTAMP="timestamp=on"

CPU_TYPE="qemu64"
CPUID_FLAGS="+mmx,"
CPUID_FLAGS="${CPUID_FLAGS},+sse2"
CPUID_FLAGS="${CPUID_FLAGS},-sse4a"
CPUID_FLAGS="${CPUID_FLAGS},-misalignsse"
CPUID_FLAGS="${CPUID_FLAGS},+sep"
CPUID_FLAGS="${CPUID_FLAGS},-movbe"
CPUID_FLAGS="${CPUID_FLAGS},-adx"
CPUID_FLAGS="${CPUID_FLAGS},-aes"
CPUID_FLAGS="${CPUID_FLAGS},-xsave"
CPUID_FLAGS="${CPUID_FLAGS},-xsaveopt"
CPUID_FLAGS="${CPUID_FLAGS},-pcid"
CPUID_FLAGS="${CPUID_FLAGS},-fsgsbase"
CPUID_FLAGS="${CPUID_FLAGS},-smep"
CPUID_FLAGS="${CPUID_FLAGS},-smap"
#CPUID_FLAGS="${CPUID_FLAGS},+vmx"

NUM_CPUS="1"
SMP="cpus=${NUM_CPUS}"

${QEMU_SYSTEM} \
    -m ${MEMORY} \
    -boot order=${BOOT_ORDER} \
    -cpu ${CPU_TYPE},${CPUID_FLAGS} \
    -smp ${SMP} \
    -vga ${VGA} \
    -msg ${TIMESTAMP} \
    -fda "${OS_IMAGE}" \
    -writeconfig "${CONFIG_OUT_FILE}"
