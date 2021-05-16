CC=gcc
LIBS=-Wall -Wextra -Wpedantic -pedantic-errors
CFLAGS=-c -g -O0
VFLAGS=--leak-check=full --track-origins=yes --show-leak-kinds=all --main-stacksize=8388608
BINARY=./database
DEP=veiculo.o linha.o main.o

all: $(DEP)
	$(CC) $(DEP) -o $(BINARY) 

veiculo.o: source/veiculo.c include/veiculo.h
	$(CC) $(CFLAGS) source/veiculo.c $(LIBS)

linha.o: source/linha. include/linha.h
	$(CC) $(CFLAGS) source/linha.c $(LIBS)

main.o: source/main.c include/veiculo.h include/linha.h
	$(CC) $(CFLAGS) source/main.c $(LIBS)

run:
	$(BINARY)

valgrind:
	valgrind $(VFLAGS) $(BINARY)