# Makefile configuration for ftoy.c and toggleLEDs.s

# Set the target MCU
CPU = msp430g2553

# Compiler and linker flags
CFLAGS = -mmcu=${CPU} -Os -I../h  # Include headers from ../h
LDFLAGS = -L../lib -L/opt/ti/msp430_gcc/include/  # Linker library path
LIBS = -lTimer -lLcd  # Libraries to link against

# Toolchain
CC = msp430-elf-gcc
AS = msp430-elf-as

# Source files
C_SOURCES = ftoy2.c
ASM_SOURCES = toggleLEDS.s

# Object files
C_OBJECTS = $(C_SOURCES:.c=.o)
ASM_OBJECTS = $(ASM_SOURCES:.s=.o)

# Output files
OUTPUT = toyf.elf

# Default target: build everything
all: $(OUTPUT)

# Link the object files into an ELF executable
$(OUTPUT): $(C_OBJECTS) $(ASM_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

# Compile C source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile Assembly source files
%.o: %.s
	$(AS) -mmcu=${CPU} -c $< -o $@

# Clean up build files
clean:
	rm -f *.o $(OUTPUT) $(HEX)

