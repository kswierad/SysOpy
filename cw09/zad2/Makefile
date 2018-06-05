CC=gcc
CFLAGS=-Wall -std=gnu99 -lpthread 

.PHONY: clean

all: main

main: main.c
	$(CC) $< -o $@ $(CFLAGS)

debug: main.c
	$(CC) $< -o $@ $(CFLAGS) -fsanitize=address -lasan  -g 

clean:
	rm *.o
	rm main