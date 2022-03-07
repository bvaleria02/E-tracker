all:
	gcc -c lib/updateChannel.c
	gcc -c lib/audiorender.c
	gcc -c lib/filter1.c
	gcc -c lib/filter2.c
	gcc -c lib/filter3.c
	gcc -c lib/filter4.c
	gcc -c main.c
	gcc -o main main.o updateChannel.o audiorender.o filter1.o filter2.o filter3.o filter4.o
	mv *.o ./bin
	./main data.hex
