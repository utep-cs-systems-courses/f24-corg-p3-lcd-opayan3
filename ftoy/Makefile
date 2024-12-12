# MSP430 Makefile - Flat Directory Structure

# Set the MSP430 device you're using
MCU = msp430g2553

# Compiler and tools
CC = msp430-gcc
AS = msp430-as
LD = msp430-ld
OBJCOPY = msp430-objcopy
OBJDUMP = msp430-objdump
GDB = msp430-gdb

# Source files
C_SOURCES = ftoy.c
ASM_SOURCES = toggleLEDs.s

# Object files
C_OBJECTS = $(C_SOURCES:.c=.o)
ASM_OBJECTS = $(ASM_SOURCES:.s=.o)

# Output files
OUTPUT = main.out
ELF = main.elf
HEX = main.hex

# Compiler flags
CFLAGS = -mmcu=$(MCU) -g -Wall
ASFLAGS = -mmcu=$(MCU)

# Default target: build everything
all: $(OUTPUT) $(HEX)

# Compile C source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile Assembly source files
%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

# Link the object files into an ELF executable
$(OUTPUT): $(C_OBJECTS) $(ASM_OBJECTS)
	$(LD) -mmcu=$(MCU) $(C_OBJECTS) $(ASM_OBJECTS) -o $(ELF)

# Convert ELF to HEX format (for flashing)
$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $(ELF) $(HEX)

# Flash the MSP430
flash: $(HEX)
	msp430loader -v -a 0x4400 -e $(HEX)

# Clean up build files
clean:
	rm -rf *.o $(OUTPUT) $(ELF) $(HEX)

# Disassemble the ELF file
disassemble: $(ELF)
	$(OBJDUMP) -D $(ELF) > disassembly.txt

# Debug with GDB
debug: $(ELF)
	$(GDB) $(ELF)
