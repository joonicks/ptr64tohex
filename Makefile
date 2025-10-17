

CFLAGS = -Os -Wall -fomit-frame-pointer -march=native -mtune=native

OBJ = simple.o io.o assembly.o

main:		main.c $(OBJ)
		gcc $(CFLAGS) -o main main.c $(OBJ)

assembly.o:	assembly.S
		gcc -c assembly.S

simple.o:	simple.c
		gcc $(CFLAGS) -c simple.c

io.o:		io.c
		gcc $(CFLAGS) -c io.c

clean:
		rm -f main $(OBJ)

dis:		main
		objdump -d main -M intel | less
