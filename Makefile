CC = gcc
CFLAGS = -Wall

all: part1 part2 part3

part1: part1.c
	$(CC) $(CFLAGS) part1.c -o part1

part2: part2.c phypages.c pagetable.c
	$(CC) $(CFLAGS) part2.c phypages.c pagetable.c -o part2

part3: part3.c
	$(CC) $(CFLAGS) part3.c -o part3

clean:
	rm -f part1 part2 part3
