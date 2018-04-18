CC = gcc
CFLAGS = -std=c11 -Wall


compile:
	$(CC) $(CFLAGS) -o master master.c
	$(CC) $(CFLAGS) -o slave slave.c

test: compile
	./test input.txt