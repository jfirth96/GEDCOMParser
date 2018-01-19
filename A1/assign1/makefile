CC = gcc
CFLAGS = -Wall -std=c99 -g -Iinclude -c

SRC = src/
BIN = bin/

EXEC = $(BIN)program
TEST = $(BIN)testMain

test : GEDCOM.o test.o LinkedList.o GEDCOMutil
	$(CC) $(BIN)testMain.o $(BIN)GEDCOMutil.o $(BIN)GEDCOMparser.o $(BIN)LinkedListAPI.o -o $(TEST)

GEDCOM.o :
	$(CC) $(CFLAGS) $(SRC)GEDCOMparser.c -o $(BIN)GEDCOMparser.o

GEDCOMutil :
	$(CC) $(CFLAGS) $(SRC)GEDCOMutilities.c -o $(BIN)GEDCOMutil.o

LinkedList.o :
	$(CC) $(CFLAGS) $(SRC)LinkedListAPI.c -o $(BIN)LinkedListAPI.o

test.o :
	$(CC) $(CFLAGS) $(SRC)testMain.c -o $(BIN)testMain.o

clean :
	rm $(BIN)*
