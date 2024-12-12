CPU             = msp430g2553
CFLAGS          = -mmcu=$(CPU) -Os -I../lcdLib -I../timerLib -I
CC              = msp430-elf-gcc
AS              = msp430-elf-as
AR              = msp430-elf-ar

# Define your output file (executable)
OUTPUT          = main.out

# Define the source files (C and Assembly)
SRC_C           = toy2.c
SRC_ASM         = reaction_time.s

# Object files
OBJ_C           = $(SRC_C:.c=.o)
OBJ_ASM         = $(SRC_ASM:.s=.o)

# Default target
all: $(OUTPUT)

# Rule to build the final output (main.out)
$(OUTPUT): $(OBJ_C) $(OBJ_ASM)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(OBJ_C) $(OBJ_ASM)

# Rule to compile C files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile Assembly files into object files
%.o: %.s
	$(AS) -mmcu=$(CPU) -o $@ $<

# Rule to clean up object files and output
clean:
	rm -f $(OBJ_C) $(OBJ_ASM) $(OUTPUT)

# Optional install rule (if needed)
install: $(OUTPUT)
	mkdir -p ../h ../lib
	mv $(OUTPUT) ../lib
	cp *.h ../h
