DIR_INCLUDE = ./src/include
DIR_SRC = ./src
DIR_OBJ = ./build/obj
DIR_BIN = ./build/bin
DIR_LIB = ./lib

SRC = $(wildcard ${DIR_SRC}/*.c)
OBJ = $(patsubst %.c, ${DIR_OBJ}/%.o, $(notdir ${SRC}))

TARGET = lisping
BIN_TARGET = ${DIR_BIN}/${TARGET}

CC = gcc
CFLAGS = -g -Wall -I${DIR_INCLUDE}

LINKER_FLAGS = -lreadline -lm
LIBS = ${DIR_LIB}/mpc.o

${BIN_TARGET}:${OBJ}
	$(CC) $(OBJ) ${LIBS} ${LINKER_FLAGS} -o $@

${DIR_OBJ}/%.o:${DIR_SRC}/%.c
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY:clean
clean:
	rm ${DIR_OBJ}/*.o

.PHONY:test
test:
	make ./test/Makefile