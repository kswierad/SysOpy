CC = gcc
FLAGS = -Wall -std=c11 -ggdb

all: compile tests clean


libs:
	$(CC) $(FLAGS) -c libfilelib.c
	$(CC) $(FLAGS) -c libfilesys.c
	ar rcs libfile.a libfilesys.o libfilelib.o

compile: libs
	$(CC) $(FLAGS) main.c -L. -lfile -o a.out 
	
tests:
	echo 4 bajty 4000 rekordów: >> wyniki.txt
	echo "" >> wyniki.txt
	./a.out generate dane.txt 4000 4 sys >> wyniki.txt
	./a.out generate dane.txt 4000 4 lib >> wyniki.txt
	./a.out copy dane.txt dane_copy.txt 4000 4 sys >> wyniki.txt
	./a.out copy dane.txt dane_copy2.txt 4000 4 lib >> wyniki.txt	
	./a.out sort dane_copy.txt 4000 4 sys >> wyniki.txt
	./a.out sort dane_copy2.txt 4000 4 lib >> wyniki.txt
	echo "" >> wyniki.txt	
	echo 4 bajty 3000 rekordów >> wyniki.txt 
	echo "" >> wyniki.txt
	./a.out generate dane2.txt 3000 4 sys >> wyniki.txt
	./a.out generate dane2.txt 3000 4 lib >> wyniki.txt
	./a.out copy dane2.txt dane2_copy.txt 3000 4 sys >> wyniki.txt
	./a.out copy dane2.txt dane2_copy2.txt 3000 4 lib >> wyniki.txt
	./a.out sort dane2_copy.txt 3000 4 sys >> wyniki.txt
	./a.out sort dane2_copy2.txt 3000 4 lib >> wyniki.txt
	echo "" >> wyniki.txt
	echo 512 bajtów 4800 rekordów: >> wyniki.txt
	echo "" >> wyniki.txt
	./a.out generate dane3.txt 4800 512 sys >> wyniki.txt
	./a.out generate dane3.txt 4800 512 lib >> wyniki.txt
	./a.out copy dane3.txt dane3_copy.txt 4800 512 sys >> wyniki.txt
	./a.out copy dane3.txt dane3_copy2.txt 4800 512 lib >> wyniki.txt
	./a.out sort dane3_copy.txt 4800 512 sys >> wyniki.txt
	./a.out sort dane3_copy2.txt 4800 512 lib >> wyniki.txt
	echo "" >> wyniki.txt
	echo 512 bajtów 3000 rekordów: >> wyniki.txt
	echo "" >> wyniki.txt
	./a.out generate dane4.txt 3000 512 sys >> wyniki.txt
	./a.out generate dane4.txt 3000 512 lib >> wyniki.txt
	./a.out copy dane4.txt dane4_copy.txt 3000 512 sys >> wyniki.txt
	./a.out copy dane4.txt dane4_copy2.txt 3000 512 lib >> wyniki.txt
	./a.out sort dane4_copy.txt 3000 512 sys >> wyniki.txt
	./a.out sort dane4_copy2.txt 3000 512 lib >> wyniki.txt
	echo "" >> wyniki.txt
	echo 4096 bajtów 2400 rekordów: >> wyniki.txt
	echo "" >> wyniki.txt
	./a.out generate dane5.txt 2400 4096 sys >> wyniki.txt
	./a.out generate dane5.txt 2400 4096 lib >> wyniki.txt
	./a.out copy dane5.txt dane5_copy.txt 2400 4096 sys >> wyniki.txt
	./a.out copy dane5.txt dane5_copy2.txt 2400 4096 lib >> wyniki.txt
	./a.out sort dane5_copy.txt 2400 4096 sys >> wyniki.txt
	./a.out sort dane5_copy2.txt 2400 4096 lib >> wyniki.txt
	echo "" >> wyniki.txt
	echo 4096 bajtów 3000 rekordów: >> wyniki.txt
	echo "" >> wyniki.txt
	./a.out generate dane6.txt 3000 4096 sys >> wyniki.txt
	./a.out generate dane6.txt 3000 4096 lib >> wyniki.txt 
	./a.out copy dane6.txt dane6_copy.txt 3000 4096 sys >> wyniki.txt
	./a.out copy dane6.txt dane6_copy2.txt 3000 4096 lib >> wyniki.txt
	./a.out sort dane6_copy.txt 3000 4096 sys >> wyniki.txt
	./a.out sort dane6_copy2.txt 3000 4096 lib >> wyniki.txt
	echo "" >> wyniki.txt
	echo 8192 bajty 2500 rekordów: >> wyniki.txt
	echo "" >> wyniki.txt
	./a.out generate dane7.txt 2500 8192 sys >> wyniki.txt
	./a.out generate dane7.txt 2500 8192 lib >> wyniki.txt
	./a.out copy dane7.txt dane7_copy.txt 2500 8192 sys >> wyniki.txt 
	./a.out copy dane7.txt dane7_copy2.txt 2500 8192 lib >> wyniki.txt
	./a.out sort dane7_copy.txt 2500 8192 sys >> wyniki.txt
	./a.out sort dane7_copy2.txt 2500 8192 lib >> wyniki.txt
	echo "" >> wyniki.txt
	echo 8192 bajty 3500 rekordów: >> wyniki.txt
	echo "" >> wyniki.txt
	./a.out generate dane8.txt 3500 8192 sys >> wyniki.txt
	./a.out generate dane8.txt 3500 8192 lib >> wyniki.txt
	./a.out copy dane8.txt dane8_copy.txt 3500 8192 sys >> wyniki.txt 
	./a.out copy dane8.txt dane8_copy2.txt 3500 8192 lib >> wyniki.txt
	./a.out sort dane8_copy.txt 3500 8192 sys >> wyniki.txt
	./a.out sort dane8_copy2.txt 3500 8192 lib >> wyniki.txt
	
clean:
	rm -f *.o
	rm -f *.a
	rm -f *test*
	rm -f *dane*
	rm -f kule.txt
	rm -f a.out
