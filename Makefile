TARGET = globalTest
CC = gcc


#PREF_SRC = ./src/
#PREF_OBJ = ./obj/

#SRC = $(wildcard $(PREF_SRC)*.c)
#OBJ = $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)*.o, $(SRC))

#${TARGET}.a: ${OBJ}
#	ar r ${TARGET}.a $<

#${PREF_OBJ}%.o: ${PREF_SRC}%.c
#	${CC} -c $< -o $@ -Isrc/Cell/ -I../enemy_system/src

#clear:
#	rm ${TARGET} ${OBJ}

${TARGET}: 
	${CC} ./Map/Map.a ./Map/tests/test_Map.a -o ${TARGET}