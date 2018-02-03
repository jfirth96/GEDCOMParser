CC = gcc
CFLAGS = -Wall -g -Iinclude

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	CCFLAGS += -std=gnu11
endif
ifeq ($(UNAME), Darwin)
	CCFLAGS += -std=c11
endif

listTests: src/listTest.c
	$(CC) $(CFLAGS) src/listTest.c src/testCases.c src/testharness.c studentCode/*.c -o bin/listTests
