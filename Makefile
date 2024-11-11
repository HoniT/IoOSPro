# Directories
BUILD = $(CURDIR)/build
BIN = $(CURDIR)/bin
SRC = $(CURDIR)/src

BOOTLOADER_DIR = $(SRC)/bootloader
KERNEL_DIR = $(SRC)/kernel

# HPP file includes
INCLUDE = -I src/kernel/include

# Flags and tools 
ASM = nasm

CXX = i686-elf-g++
CXX_FLAGS = $(INCLUDE) -g -Wall -O2 -ffreestanding -mgeneral-regs-only \
           -fno-exceptions -fno-rtti -fno-pic -fno-asynchronous-unwind-tables

GCC = i686-elf-gcc
GCC_LINK_FLAGS = -nostdlib -nostartfiles -nodefaultlibs -no-pie -Wl,--build-id=none

OBJCOPY = i686-elf-objcopy

# Output files
OS_ELF = $(BIN)/io_os.elf
OS_BIN = $(BIN)/io_os.bin

# Find all .cpp and .asm files in src directory and its subdirectories
CPP_SOURCES = $(shell find $(SRC) -name "*.cpp")
ASM_SOURCES = $(shell find $(SRC) -name "*.asm")

# Convert sources to objects
CPP_OBJECTS = $(patsubst $(SRC)/%.cpp, $(BUILD)/%.o, $(CPP_SOURCES))
ASM_OBJECTS = $(patsubst $(SRC)/%.asm, $(BUILD)/%.o, $(ASM_SOURCES))

# Target
all: $(OS_BIN)

# Compile and link each file separately
$(OS_ELF): $(ASM_OBJECTS) $(CPP_OBJECTS)
	@mkdir -p $(BIN)
	$(GCC) $(GCC_LINK_FLAGS) -T src/linker.ld -o $@ $(ASM_OBJECTS) $(CPP_OBJECTS)

$(OS_BIN): $(OS_ELF)
	$(OBJCOPY) -O binary $< $@

# Rule to compile each .cpp file individually
$(BUILD)/%.o: $(SRC)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) -std=c++23 -c $< -o $@

# Rule to assemble each .asm file individually
$(BUILD)/%.o: $(SRC)/%.asm
	@mkdir -p $(dir $@)
	$(ASM) -f elf -g $< -o $@

# Cleaning the build
clean:
	rm -rf $(BUILD) $(BIN)
	mkdir -p $(BUILD) $(BIN)

.PHONY: all clean
