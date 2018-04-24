SRC=main.c wav/wav.c
CFLAGS=-Wall -Wpedantic -Werror -O3 -std=c11 -I./wav/
LDFLAGS=-lm
OFNAME=resize

build:
	@gcc ${CFLAGS} ${SRC} -o ${OFNAME} ${LDFLAGS}

run: build
	@./${OFNAME}

clean:
	@rm -f ${OFNAME} *.o
