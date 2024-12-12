CC = msp430-gcc
CFLAGS = -Wall -mmcu=msp430g2553 -I./lcdlib

SRC = toy.c lcdlib/lcdutils.c main.s
OUTPUT = game.elf

$(OUTPUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)
