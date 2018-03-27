CC = gcc
CFLAGS = -std=c11 -Wall


compile:
	$(CC) $(CFLAGS) -o test main.c
	$(CC) $(CFLAGS) -o prog prog.c

test: compile
	./test input.txt