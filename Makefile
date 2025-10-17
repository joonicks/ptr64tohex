

CFLAGS = -Os -Wall -fomit-frame-pointer -march=native -mtune=native

OBJ = simple.o io.o ptr64tohex.o

main:		main.c $(OBJ)
		gcc $(CFLAGS) -o main main.c $(OBJ)

ptr64tohex.o:	ptr64tohex.S
		gcc -c ptr64tohex.S -o ptr64tohex.o

simple.o:	simple.c
		gcc $(CFLAGS) -c simple.c

io.o:		io.c
		gcc $(CFLAGS) -c io.c

clean:
		rm -f main $(OBJ)

dis:		main
		objdump -d main -M intel | less
