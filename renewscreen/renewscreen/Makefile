CC = gcc
TARGET = renewscreen

LIB: OBJECT
	ar r lib${TARGET}.a ${TARGET}.o

OBJECT:
	${CC} ${TARGET}.c -c

clear:
	rm lib${TARGET}.a
	rm ${TARGET}.o