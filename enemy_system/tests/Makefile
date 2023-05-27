TARGET = libTestEnemySystem
CC = gcc

PREF_SRC = ./test_units/
HEADER_PREF = ../src/
PREF_OBJ = ./obj/

SRC = $(wildcard $(PREF_SRC)*.c)
OBJ = $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(SRC))

${TARGET}: ${OBJ}
	ar r ${TARGET}.a ${OBJ}

${PREF_OBJ}%.o: ${PREF_SRC}%.c
	${CC} -c -I ${HEADER_PREF} -I ./test_units $< -o $@
