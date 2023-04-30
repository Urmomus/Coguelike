TARGET = enemySystem
CC = gcc

PREF_SRC = ./src/
PREF_OBJ = ./obj/

SRC = $(wildcard $(PREF_SRC)*.c)
OBJ = $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.out, $(SRC))

${TARGET}: ${OBJ}
	${CC} ${OBJ} -o ${TARGET}

${PREF_OBJ}%.out: ${PREF_SRC}%.c
	${CC} -c $< -o $@

clean:
	rm ${TARGET} ${OBJ}
