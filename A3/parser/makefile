CC = gcc
CFLAGS = -Wall -std=c11 -g -Iinclude
LDFLAGS = -llist -lparser

SRC = ./src/
BIN = ./bin/
INC = ./include/
LIB = -L./bin

TEST = $(BIN)test

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin)
	LIB := sharedLib.dylib
else
	LIB := sharedLib.so
endif

$(LIB): parser

#test : test.o
#	$(CC) $(BIN)testMain.o $(LIB) $(LDFLAGS) -o $(TEST)

#test.o : $(SRC)testMain.c
#	$(CC) $(CFLAGS) -c $(SRC)testMain.c -o $(BIN)testMain.o

#list: LinkedList.o
#	ar cr $(BIN)liblist.a $(BIN)LinkedListAPI.o

parser: GEDCOM.o GEDCOMutil LinkedList.o
	$(CC) -shared -o ../$(LIB) $(BIN)GEDCOMparser.o $(BIN)GEDCOMutil.o $(BIN)LinkedListAPI.o

GEDCOM.o : $(SRC)GEDCOMparser.c
	$(CC) $(CFLAGS) -c -fpic $(SRC)GEDCOMparser.c -o $(BIN)GEDCOMparser.o

GEDCOMutil : $(SRC)GEDCOMutilities.c $(INC)GEDCOMutilities.h
	$(CC) $(CFLAGS) -c -fpic $(SRC)GEDCOMutilities.c -o $(BIN)GEDCOMutil.o

LinkedList.o : $(SRC)LinkedListAPI.c $(INC)LinkedListAPI.h
	$(CC) $(CFLAGS) -c $(SRC)LinkedListAPI.c -o $(BIN)LinkedListAPI.o

#TestUtil.o : $(SRC)GEDCOMTestUtilities.c $(INC)GEDCOMTestUtilities.h
#	$(CC) $(CFLAGS) -c $(SRC)GEDCOMTestUtilities.c -o $(BIN)GEDCOMTestUtilities.o

#run :
#	$(TEST)

#memCheck :
#	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(TEST)

clean :
	rm $(BIN)*
