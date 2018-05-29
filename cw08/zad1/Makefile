CC=gcc
CFLAGS=-Wall -std=gnu99 -lpthread -lm

.PHONY: clean

all: main

main: main.c
	$(CC) $< -o $@ $(CFLAGS)


clean:
	rm *.o
	rm main