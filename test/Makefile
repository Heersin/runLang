DIR_INCLUDE = ../src/include
DIR_SRC = ../src
DIR_OBJ = ../build/obj
DIR_BIN = ../build/bin
DIR_LIB = ../lib


CC = gcc
CFLAGS = -g -Wall -I${DIR_INCLUDE}

LINKER_FLAGS = -lreadline -lm
LIBS = ${DIR_LIB}/mpc.o


DIR_TEST_SRC = ./src
DIR_TEST_BIN = ./example
DIR_TEST_OBJ = ./obj
TEST_SRC = $(wildcard $(DIR_TEST_SRC)/*.c)
TEST_TARGET = $(patsubst %.c, $(DIR_TEST_BIN)/%.bin, $(notdir ${TEST_SRC}))
TEST_OBJ = $(patsubst %.c, $(DIR_TEST_OBJ)/%.o, $(notdir ${TEST_SRC}))

all: $(TEST_TARGET)

$(TEST_TARGET):$(TEST_OBJ)
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $@ $(LIBS) $<

$(DIR_TEST_OBJ)/%.o: $(DIR_TEST_SRC)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<