DIR_INC = ./include
DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

SRC = $(wildcard ${DIR_SRC}/*.c)  
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC})) 


#CFLAGS = -fstack-protector-all  -Wall -g -c  -I${DIR_INC}
CFLAGS = -Wall -g -c  -I${DIR_INC}
LDFLAGS =  -lpthread
TARGET = main

BIN_TARGET = ${DIR_BIN}/${TARGET}

CC =gcc

${BIN_TARGET}:${OBJ}
	$(CC) -o $@ $(OBJ) $(LDFLAGS)
    
${DIR_OBJ}/%.o:${DIR_SRC}/%.c
	$(CC) $(CFLAGS)  $< -o $@ 

.PHONY:clean

clean:
	rm  ${DIR_OBJ}/* -rf
	rm  ${BIN_TARGET}