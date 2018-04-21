SRC=main.c wav/wav.c
CPPFLAGS=-Wall -Wpedantic -Werror -O3 -g -std=c11 -I./wav/
LDFLAGS=-lm
OFNAME=resize

build:
	@gcc ${CPPFLAGS} ${SRC} -o ${OFNAME} ${LDFLAGS}

run: build
	@./${OFNAME}

clean:
	@rm -f ${OFNAME} *.o
