CC = msp430-gcc
CFLAGS = -Wall -mmcu=msp430g2553

# List of source files
SRC = toy.c lcdlib/lcdutils.c

# The output file
OUTPUT = game.elf

# How to build the output file from source files
$(OUTPUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)
