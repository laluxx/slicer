PROG=dungeon-generator
CC=gcc
FLAGS=-lm -lraylib 

.DEFAULT_GOAL := build

install: ${PROG}
	cp ${PROG} ~/.local/bin/

build: dungeon.c
	${CC} dungeon.c -o ${PROG} ${FLAGS}

build-osx: ${PROG}
	clang -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL libraylib.a dungeon.c -o ${PROG}

clean: ${PROG}
	rm ${PROG}
