CC = gcc
FLAGS = -Wall -std=c11 -ggdb

all: test clean

compile: static shared dynamic

static:
	$(CC) $(FLAGS) -c library.c
	ar rcs library.a library.o
	$(CC) $(FLAGS) -static main.c -L. -lrary -o teststatic 

shared:
	$(CC) $(FLAGS) -c -fPIC library.c
	$(CC) $(FLAGS) -shared -fPIC -o library.so library.o
	$(CC) $(FLAGS) main.c -L. -Wl,-rpath=. -lrary -o testshared 

dynamic:
	$(CC) $(FLAGS) -c -fPIC library.c
	$(CC) $(FLAGS) -shared -fPIC -o library.so library.o
	$(CC) $(FLAGS) main.c -L. -Wl,-rpath=. -lrary -o testdynamic -ldl -D DLL=2

test: compile
	echo 'static library \n' >> results3a.txt
	echo '50000 blocks each 512 wide (dynamic), searching for 100, replacing 10000 blocks, adding 10000 blocks \n' >> results3a.txt
	./teststatic 50000 512 dynamic search 100 replace 10000 add 10000 | tee -a results3a.txt
	echo '50000 blocks each 512 wide (static), searching for 100, replacing 10000 blocks, adding 10000 blocks \n' >> results3a.txt
	./teststatic 50000 512 static search 100 replace 10000 add 10000 | tee -a results3a.txt
	echo '50000 blocks each 512 wide (dynamic), adding 10000 blocks, removing 10000 blocks, searching for 100 \n' >> results3a.txt
	./teststatic 50000 512 dynamic add 10000 remove 10000 search 100 | tee -a results3a.txt
	echo '50000 blocks each 512 wide (static), adding 10000 blocks, removing 10000 blocks, searching for 100 \n' >> results3a.txt
	./teststatic 50000 512 static add 10000 remove 10000 search 100 | tee -a results3a.txt
	echo 'shared library \n' >> results3a.txt
	echo '50000 blocks each 512 wide (dynamic), searching for 100, replacing 10000 blocks, adding 10000 blocks \n' >> results3a.txt
	./testshared 50000 512 dynamic search 100 replace 10000 add 10000 | tee -a results3a.txt
	echo '50000 blocks each 512 wide (static), searching for 100, replacing 10000 blocks, adding 10000 blocks \n' >> results3a.txt
	./testshared 50000 512 static search 100 replace 10000 add 10000 | tee -a results3a.txt
	echo '50000 blocks each 512 wide (dynamic), adding 10000 blocks, removing 10000 blocks, searching for 100 \n' >> results3a.txt
	./testshared 50000 512 dynamic add 10000 remove 10000 search 100 | tee -a results3a.txt
	echo '50000 blocks each 512 wide (static), adding 10000 blocks, removing 10000 blocks, searching for 100 \n' >> results3a.txt
	./testshared 50000 512 static add 10000 remove 10000 search 100 | tee -a results3a.txt
	echo 'dynamic library \n' >> results3a.txt
	echo '50000 blocks each 512 wide (dynamic), searching for 100, replacing 10000 blocks, adding 10000 blocks \n' >> results3a.txt
	./testdynamic 50000 512 dynamic search 100 replace 10000 add 10000 | tee -a results3a.txt
	echo '50000 blocks each 512 wide (static), searching for 100, replacing 10000 blocks, adding 10000 blocks \n' >> results3a.txt
	./testdynamic 50000 512 static search 100 replace 10000 add 10000 | tee -a results3a.txt
	echo '50000 blocks each 512 wide (dynamic), adding 10000 blocks, removing 10000 blocks, searching for 100 \n' >> results3a.txt
	./testdynamic 50000 512 dynamic add 10000 remove 10000 search 100 | tee -a results3a.txt
	echo '50000 blocks each 512 wide (static), adding 10000 blocks, removing 10000 blocks, searching for 100 \n' >> results3a.txt
	./testdynamic 50000 512 static add 10000 remove 10000 search 100 | tee -a results3a.txt



clean:
	rm -f *.o
	rm -f *.so
	rm -f *.a
	rm -f *test*

	
