
# GCC Cross-Compiler Preparation
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

# Making Project
make clean
make all

# Running QEMU
qemu-system-i386 -drive file=bin/io_os.bin,format=raw