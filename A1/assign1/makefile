CC = gcc
CFLAGS = -Wall -std=c11 -g -Iinclude
LDFLAGS = -llist -lparser

SRC = ./src/
BIN = ./bin/
LIB = -L./bin

all: list parser

list: LinkedList.o
	ar cr $(BIN)liblist.a $(BIN)LinkedListAPI.o

parser: GEDCOM.o GEDCOMutil LinkedList.o
	ar cr $(BIN)libparser.a $(BIN)LinkedListAPI.o $(BIN)GEDCOMutil.o $(BIN)GEDCOMparser.o

GEDCOM.o :
	$(CC) $(CFLAGS) -c $(SRC)GEDCOMparser.c -o $(BIN)GEDCOMparser.o

GEDCOMutil :
	$(CC) $(CFLAGS) -c $(SRC)GEDCOMutilities.c -o $(BIN)GEDCOMutil.o

LinkedList.o :
	$(CC) $(CFLAGS) -c $(SRC)LinkedListAPI.c -o $(BIN)LinkedListAPI.o

clean :
	rm $(BIN)*
