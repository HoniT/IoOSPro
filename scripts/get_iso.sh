
# GCC Cross-Compiler Preparation
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

# Making Project
make clean
make all

# Copying BIN to iso/boot
cp bin/io_os.bin iso/boot/io_os.bin

# Creating ISO
mkisofs -o iso/io_os.iso \
  -b boot/io_os.bin \
  -no-emul-boot \
  -boot-load-size 4 \
  -boot-info-table \
  iso/

# Running QEMU
qemu-system-x86_64 -cdrom iso/io_os.iso
