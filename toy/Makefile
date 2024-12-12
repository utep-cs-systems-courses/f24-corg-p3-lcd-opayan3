TARGET = toy2.out

# MSP430 toolchain settings
CC = msp430-elf-gcc
AS = msp430-elf-as
LD = msp430-elf-ld
CFLAGS = -mmcu=msp430g2553 -Wall -Os
LDFLAGS = -L../timerLib -L../lcdLib  # Linker flags to use libraries from these folders

# Paths to header files
INCLUDE_DIRS = -I../timerLib -I../lcdLib

# Source files
SRCS = toy2.c reaction_time.s

# Object files
OBJS = $(SRCS:.c=.o) $(SRCS:.s=.o)

# Default rule: build the target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

# Rule for compiling .c files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# Rule for assembling .s files
%.o: %.s
	$(AS) -mmcu=msp430g2553 -o $@ $<

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)
