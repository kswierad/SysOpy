CC = gcc
FLAGS = -Wall -std=c11 -ggdb

all: compile clean


compile:
	$(CC) $(FLAGS) -c libfilelib.c
	ar rcs libfilelib.a libfilelib.o
	$(CC) $(FLAGS) main.c -L. -lfilelib -o test 
	
clean:
	rm -f *.o
	rm -f *.a
	#rm -f *test*