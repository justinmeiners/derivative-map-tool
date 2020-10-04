SRC=derivative.c
CC=gcc
LFLAGS=-lm

.PHONY: test

derivative: ${SRC}
	${CC} -o $@ $^ ${LFLAGS}


test: sample/derivative.png

sample/derivative.png: sample/heightmap.png derivative
	./derivative sample/heightmap.png sample/derivative.png
