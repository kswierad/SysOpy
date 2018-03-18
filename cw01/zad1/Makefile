CC = gcc
CFLAGS = -Wall -std=c11 -ggdb

all: static shared clean


static:
	$(CC) $(CFLAGS) -c library.c
	ar rcs library.a library.o

shared:
	$(CC) $(CFLAGS) -c -fPIC library.c
	$(CC) -shared -fPIC -o library.so library.o

clean:
	rm -f *.o
	rm -f *.so
	rm -f *.a
