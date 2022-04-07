CC=gcc
CFLAGS=-I.

hellomake: CriaProcessos.o
	$(CC) $(CFLAGS) -o criaProcessos CriaProcessos.o
	./criaProcessos
