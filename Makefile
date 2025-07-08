CC = gcc
CFLAGS = -Wall -Wextra -g

all: bitflip

bitflip: bitflip.o
	$(CC) $(CFLAGS) bitflip.o -o bitflip

bitflip.o: bitflip.c
	$(CC) $(CFLAGS) -c bitflip.c

clean:
	rm -f *.o bitflip
