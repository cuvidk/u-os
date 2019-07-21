#!/bin/bash

# change the versions bellow to build other gcc compiler / binutils versions
# check https://wiki.osdev.org/Cross-Compiler_Successful_Builds for a list
# of compatible gcc - binutils pairs
BUILD_BINUTILS_VERSION='2.30'
BUILD_GCC_VERSION='7.4.0'

DEPENDENCIES='build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libcloog-isl-dev libisl-dev'
CURRENT_DIR="$(pwd)"
CROSS_COMPILER_DIR="${CURRENT_DIR}/cross-compiler"
CROSS_COMPILER_INSTALLATION_DIR="${CROSS_COMPILER_DIR}/install"

BUILD_BINUTILS_TAR="${CROSS_COMPILER_DIR}/binutils-${BUILD_BINUTILS_VERSION}.tar.gz"
BUILD_BINUTILS_SRC="${CROSS_COMPILER_DIR}/binutils-${BUILD_BINUTILS_VERSION}"
BUILD_BINUTILS_BUILD="${CROSS_COMPILER_DIR}/build-binutils-${BUILD_BINUTILS_VERSION}"
BUILD_BINUTILS_URL="https://ftp.gnu.org/gnu/binutils/binutils-${BUILD_BINUTILS_VERSION}.tar.gz"

BUILD_GCC_TAR="${CROSS_COMPILER_DIR}/gcc-${BUILD_GCC_VERSION}.tar.gz"
BUILD_GCC_SRC="${CROSS_COMPILER_DIR}/gcc-${BUILD_GCC_VERSION}"
BUILD_GCC_BUILD="${CROSS_COMPILER_DIR}/build-gcc-${BUILD_GCC_VERSION}"
BUILD_GCC_URL="https://ftp.gnu.org/gnu/gcc/gcc-${BUILD_GCC_VERSION}/gcc-${BUILD_GCC_VERSION}.tar.gz"

export PREFIX="${CROSS_COMPILER_INSTALLATION_DIR}"
export TARGET=x86_64-elf
export PATH="${PREFIX}/bin:$PATH"

function announce() {
    local sep='---------------------------'
    echo -e "${sep}\n$1\n${sep}"
}

if [ "root" != "$(whoami)" ]; then
    echo "You need to be root to execute this script"
    exit 1
fi

echo "This script will build a ${TARGET} gcc compiler."
echo "gcc ${BUILD_GCC_VERSION} and binutils ${BUILD_BINUTILS_VERSION} will be built and installed."
echo "libgcc will be built without the redzone feature."
echo "The following packages have to be installed to help with the compilation of the ${TARGET} compiler:"
for dependency in ${DEPENDENCIES}; do
    echo -e "\t$dependency"
done
echo -n "The required packages will automatically be installed only if you agree with it (y / n[default]):"
read consent
if [ 'y' != "${consent}" -a 'Y' != "${consent}" ]; then
    exit 1
fi

mkdir -p ${CROSS_COMPILER_DIR} && cd ${CROSS_COMPILER_DIR}

announce "Downloading binutils ${BUILD_BINUTILS_VERSION}"
wget "${BUILD_BINUTILS_URL}" || exit 1
tar -xzf "${BUILD_BINUTILS_TAR}" || exit 1
rm -rf "${BUILD_BINUTILS_TAR}"

announce "Downloading gcc ${BUILD_GCC_VERSION}"
wget "${BUILD_GCC_URL}" || exit 1
tar -xzf "${BUILD_GCC_TAR}" || exit 1
rm -rf "${BUILD_GCC_TAR}"

echo "Installing packages necessary for cross-compiler build"
for dependency in ${DEPENDENCIES}; do
    announce "Installing ${dependency}"
    apt -y install ${dependency} || exit 3
done


mkdir -p ${BUILD_BINUTILS_BUILD} && cd ${BUILD_BINUTILS_BUILD}
announce "Compiling binutils ${BUILD_BINUTILS_VERSION}" && sleep 2
"${BUILD_BINUTILS_SRC}/configure" --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
announce "Installing binutils ${BUILD_BINUTILS_VERSION}" && sleep 2
make install

rm -rf ${BUILD_BINUTILS_SRC}
rm -rf ${BUILD_BINUTILS_BUILD}

which $TARGET-as >/dev/null
if [ $? -ne 0 ]; then
    echo "Missing $TARGET-as. Something went wrong during installation / compilation of binutils ${BUILD_BINUTILS_VERSION}"
    exit 1
fi

# disabling red-zone feature in libgcc
cat<<EOF >"${BUILD_GCC_SRC}/gcc/config/i386/t-x86_64-elf"
MULTILIB_OPTIONS += mno-red-zone
MULTILIB_DIRNAMES += no-red-zone
EOF

sed -r -i 's/x86_64-\*-elf\*\)/&\n\ttmake_file="\$\{tmake_file\} i386\/t-x86_64-elf"/' "${BUILD_GCC_SRC}/gcc/config.gcc"

mkdir -p ${BUILD_GCC_BUILD} && cd ${BUILD_GCC_BUILD}
"${BUILD_GCC_SRC}/configure" --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
announce "Compiling gcc ${BUILD_GCC_VERSION}" && sleep 2
make all-gcc
make all-target-libgcc
announce "Installing gcc ${BUILD_GCC_VERSION}" && sleep 2
make install-gcc
make install-target-libgcc

announce "Cleaning stuff"
rm -rf ${BUILD_GCC_SRC}
rm -rf ${BUILD_GCC_BUILD}

echo -e "Done. Cross compiler related files were dumped in ${CROSS_COMPILER_INSTALLATION_DIR}\nBye"
