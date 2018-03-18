CC = gcc
FLAGS = -Wall -std=c11 -ggdb

all: test clean


static:
	$(CC) $(FLAGS) -c library.c
	ar rcs library.a library.o

clean:
	rm -f *.o
	rm -f *.so
	rm -f *.a
	rm -f *test*

compile: static
	$(CC) $(FLAGS) main.c -L. -lrary -o test 

test: compile
	echo 'static library \n' >> results2.txt
	echo '50000 blocks each 512 wide (dynamic), searching for 100, replacing 10000 blocks, adding 10000 blocks \n' >> results2.txt
	./test 50000 512 dynamic search 100 replace 10000 add 10000 | tee -a results2.txt
	echo '50000 blocks each 512 wide (static), searching for 100, replacing 10000 blocks, adding 10000 blocks \n' >> results2.txt
	./test 50000 512 static search 100 replace 10000 add 10000 | tee -a results2.txt
	echo '50000 blocks each 512 wide (dynamic), adding 10000 blocks, removing 10000 blocks, searching for 100 \n' >> results2.txt
	./test 50000 512 dynamic add 10000 remove 10000 search 100 | tee -a results2.txt
	echo '50000 blocks each 512 wide (static), adding 10000 blocks, removing 10000 blocks, searching for 100 \n' >> results2.txt
	./test 50000 512 static add 10000 remove 10000 search 100 | tee -a results2.txt

