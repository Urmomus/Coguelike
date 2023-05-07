CC = gcc
TARGET = test_Map

PREF_SRC = ./test_Map/
PREF_OBJ = ./obj/

PREF_HEADERS = ../src/

SRC = $(wildcard $(PREF_SRC)*.c)
OBJ = $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(SRC))

${TARGET}.a: ${OBJ}
	ar r lib${TARGET}.a $<

${PREF_OBJ}%.o: ${PREF_SRC}%.c
	${CC} -c -I ${PREF_HEADERS} -I${PREF_HEADERS}/Cell/ -I../../enemy_system/src/ $< -o $@

clear:
	rm ${TARGET} ${OBJ}
