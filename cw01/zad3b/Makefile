CC = gcc
FLAGS = -Wall -std=c11 -ggdb

optim: 
	make test OPT=0
	make test OPT=1
	make test OPT=2
	make test OPT=3

all: optim clean

compile: static shared dynamic

static:
	$(CC) $(FLAGS) -c library.c
	ar rcs library.a library.o
	$(CC) $(FLAGS) -static main.c -L. -lrary -o test_static_O$(OPT) -O$(OPT)

shared:
	$(CC) $(FLAGS) -c -fPIC library.c
	$(CC) $(FLAGS) -shared -fPIC -o library.so library.o
	$(CC) $(FLAGS) main.c -L. -Wl,-rpath=. -lrary -o test_shared_O$(OPT) -O$(OPT)

dynamic:
	$(CC) $(FLAGS) -c -fPIC library.c
	$(CC) $(FLAGS) -shared -fPIC -o library.so library.o
	$(CC) $(FLAGS) main.c -L. -Wl,-rpath=. -lrary -o test_dynamic_O$(OPT) -ldl -D DLL=2 -O$(OPT)

test: compile
	echo 'static library \n' >> results3b.txt
	echo '50000 blocks each 512 wide (dynamic), searching for 100, replacing 10000 blocks, adding 10000 blocks with -O$(OPT) \n' >> results3b.txt
	./test_static_O$(OPT) 50000 512 dynamic search 100 replace 10000 add 10000 | tee -a results3b.txt
	echo '50000 blocks each 512 wide (static), searching for 100, replacing 10000 blocks, adding 10000 blocks with -O$(OPT) \n' >> results3b.txt
	./test_static_O$(OPT) 50000 512 static search 100 replace 10000 add 10000 | tee -a results3b.txt
	echo '50000 blocks each 512 wide (dynamic), adding 10000 blocks, removing 10000 blocks, searching for 100 with -O$(OPT) \n' >> results3b.txt	
	./test_static_O$(OPT) 50000 512 dynamic add 10000 remove 10000 search 100 | tee -a results3b.txt
	echo '50000 blocks each 512 wide (static), adding 10000 blocks, removing 10000 blocks, searching for 100 with -O$(OPT) \n' >> results3b.txt
	./test_static_O$(OPT) 50000 512 static add 10000 remove 10000 search 100 | tee -a results3b.txt
	echo 'shared library \n' >> results3b.txt
	echo '50000 blocks each 512 wide (dynamic), searching for 100, replacing 10000 blocks, adding 10000 blocks with -O$(OPT) \n' >> results3b.txt
	./test_shared_O$(OPT) 50000 512 dynamic search 100 replace 10000 add 10000 | tee -a results3b.txt
	echo '50000 blocks each 512 wide (static), searching for 100, replacing 10000 blocks, adding 10000 blocks with -O$(OPT) \n' >> results3b.txt
	./test_shared_O$(OPT) 50000 512 static search 100 replace 10000 add 10000 | tee -a results3b.txt
	echo '50000 blocks each 512 wide (dynamic), adding 10000 blocks, removing 10000 blocks, searching for 100 with -O$(OPT) \n' >> results3b.txt
	./test_shared_O$(OPT) 50000 512 dynamic add 10000 remove 10000 search 100 | tee -a results3b.txt
	echo '50000 blocks each 512 wide (static), adding 10000 blocks, removing 10000 blocks, searching for 100 with -O$(OPT) \n' >> results3b.txt
	./test_shared_O$(OPT) 50000 512 static add 10000 remove 10000 search 100 | tee -a results3b.txt
	echo 'dynamic library \n' >> results3b.txt
	echo '50000 blocks each 512 wide (dynamic), searching for 100, replacing 10000 blocks, adding 10000 blocks with -O$(OPT) \n' >> results3b.txt
	./test_dynamic_O$(OPT) 50000 512 dynamic search 100 replace 10000 add 10000 | tee -a results3b.txt
	echo '50000 blocks each 512 wide (static), searching for 100, replacing 10000 blocks, adding 10000 blocks with -O$(OPT) \n' >> results3b.txt
	./test_dynamic_O$(OPT) 50000 512 static search 100 replace 10000 add 10000 | tee -a results3b.txt
	echo '50000 blocks each 512 wide (dynamic), adding 10000 blocks, removing 10000 blocks, searching for 100 with -O$(OPT) \n' >> results3b.txt
	./test_dynamic_O$(OPT) 50000 512 dynamic add 10000 remove 10000 search 100 | tee -a results3b.txt
	echo '50000 blocks each 512 wide (static), adding 10000 blocks, removing 10000 blocks, searching for 100 with -O$(OPT) \n' >> results3b.txt
	./test_dynamic_O$(OPT) 50000 512 static add 10000 remove 10000 search 100 | tee -a results3b.txt



clean:
	rm -f *.o
	rm -f *.so
	rm -f *.a
	rm -f *test*

	
