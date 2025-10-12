

main:	main.c io.o ptr64tohex.o
	gcc -Os -Wall -o main main.c io.o ptr64tohex.o

ptr64tohex.o: ptr64tohex.S
	gcc -c ptr64tohex.S -o ptr64tohex.o

io.o:	io.c
	gcc -Os -Wall -c io.c
