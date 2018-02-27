# Source Files
SRC_DIR := src/
STUDENT_SRC_DIR := studentCode/

SOURCE_FILES := $(wildcard $(SRC_DIR)*.c)
STUDENT_SOURCE_FILES := $(wildcard $(STUDENT_SRC_DIR)*.c)

# Source Objects
OBJ_DIR := bin/
OBJ_FILES := $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SOURCE_FILES))
OBJ_FILES += $(patsubst $(STUDENT_SRC_DIR)%.c,$(OBJ_DIR)%.o,$(STUDENT_SOURCE_FILES))

# Because we have a lot of "mains" and don't have time to "pretty-ify" all of their compilation, just removing them for GEDCOMtests
OBJ_FILES := $(filter-out $(OBJ_DIR)ListTest.o, $(OBJ_FILES))
OBJ_FILES := $(filter-out $(OBJ_DIR)MemTest.o, $(OBJ_FILES))
OBJ_FILES := $(filter-out $(OBJ_DIR)MemTestPrint.o, $(OBJ_FILES))

# Binary File
OBJ := $(OBJ_DIR)GEDCOMtests

# Header Files
HEADER_DIR := include/
STUDENT_HEADER_DIR := studentInclude/

# Depend Files (If a .h file is updated, recompile any file depending on it, not default by Make)
DEPEND_DIR := .depend/
DEPEND_FILES := $(patsubst $(SRC_DIR)%.c,$(DEPEND_DIR)%.d,$(SOURCE_FILES))
DEPEND_FILES += $(patsubst $(STUDENT_SRC_DIR)%.c,$(DEPEND_DIR)%.d,$(STUDENT_SOURCE_FILES))

# Dependency Flags
DFLAGS := -MMD -MF

# Include directories
INCLUDE := -I$(HEADER_DIR)
STUDENT_INCLUDE := -I$(STUDENT_HEADER_DIR) -I$(HEADER_DIR)

CC = gcc
CFLAGS = -Wall -g -std=c11

UNAME := $(shell uname)

.PHONY: all listTests memTest valgrind clean

all: $(OBJ)

# bin/GEDCOMtests
$(OBJ): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $@

# Compile the Students code
$(OBJ_DIR)%.o: $(STUDENT_SRC_DIR)%.c
	$(CC) -c $(CFLAGS) $(STUDENT_INCLUDE) $< $(DFLAGS) $(patsubst $(OBJ_DIR)%.o,$(DEPEND_DIR)%.d,$@) -o $@

# Compile our test code
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< $(DFLAGS) $(patsubst $(OBJ_DIR)%.o,$(DEPEND_DIR)%.d,$@) -o $@

# Include header targets
-include $(DEPEND_FILES)

# bin/listTests
listTests:  
	$(CC) $(INCLUDE) $(STUDENT_INCLUDE) $(CFLAGS) src/ListTest.c src/ListTestCases.c src/TestHarness.c studentCode/*.c  -o bin/listTests

valgrind:
	valgrind --show-leak-kinds=all --leak-check=yes --track-origins=yes ./$(OBJ_DIR)memTest1 1; \
	valgrind --show-leak-kinds=all --leak-check=yes --track-origins=yes ./$(OBJ_DIR)memTest1 2; \
	valgrind --show-leak-kinds=all --leak-check=yes --track-origins=yes ./$(OBJ_DIR)memTest1 3; \
	valgrind --show-leak-kinds=all --leak-check=yes --track-origins=yes ./$(OBJ_DIR)memTest1 4

# bin/memTest1
memTest:
	$(CC) $(INCLUDE) $(STUDENT_INCLUDE) $(CFLAGS) src/MemTest.c studentCode/*.c  -o bin/memTest1

clean:
	-rm .depend/*
	-rm bin/*
