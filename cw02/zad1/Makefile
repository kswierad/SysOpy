CC = gcc
FLAGS = -Wall -std=c11 -ggdb

all: libs clean


libs:
	$(CC) $(FLAGS) -c libfilelib.c
	ar rcs libfilelib.a libfilelib.o
	$(CC) $(FLAGS) -c libfilesys.c
	ar rcs libfilesys.a libfilesys.o

lib: libs
	$(CC) $(FLAGS) main.c -L. -lfilelib -o test 
	
sys: libs
	$(CC) $(FLAGS) main.c -L. -lfilelib -o test
	
clean:
	rm -f *.o
	rm -f *.a
	#rm -f *test*
