# MSP430 Makefile for ftoy.c and toggleLEDs.s

# Set the target MCU
CPU = msp430g2553

# Compiler and linker flags
CFLAGS = -mmcu=${CPU} -Os -I../h   # Include headers from ../h
LDFLAGS = -L../lib                  # Linker library path
LIBS = -lTimer -lLcd                # Libraries to link against

# Toolchain
CC = msp430-elf-gcc
AS = msp430-elf-as

# Source files
C_SOURCES = ftoy.c
ASM_SOURCES = toggleLEDS.s

# Object files
C_OBJECTS = $(C_SOURCES:.c=.o)
ASM_OBJECTS = $(ASM_SOURCES:.s=.o)

# Output files
OUTPUT = toyf.elf
HEX = toyf.hex

# Default target: build everything
all: $(OUTPUT)

# Link the object files into an ELF executable
$(OUTPUT): $(C_OBJECTS) $(ASM_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

# Convert ELF to HEX format (for flashing)
$(HEX): $(OUTPUT)
	msp430-objcopy -O ihex $(OUTPUT) $(HEX)

# Flash the MSP430
flash: $(HEX)
	msp430loader.sh $(HEX)

# Compile C source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile Assembly source files
%.o: %.s
	$(AS) -mmcu=${CPU} -$< -o $@
# Clean up build files
clean:
	rm -f *.o $(OUTPUT) $(HEX)

# Debugging or disassembling (optional)
disassemble: $(OUTPUT)
	$(OBJDUMP) -D $(OUTPUT) > disassembly.txt
