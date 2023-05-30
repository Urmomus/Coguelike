TARGET = test_renewscreen
CC = gcc

LIB = renewscreen

EXEC :
	${CC} -c ${TARGET}.c
	ar r lib${TARGET}.a ${TARGET}.o
	${CC} main.c -L./ -l${TARGET} -l${LIB} -lncurses
 
 clear:
	rm lib${TARGET}.a
	rm ${TARGET}.o