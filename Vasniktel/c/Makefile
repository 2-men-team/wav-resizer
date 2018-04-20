SRC=main.c wav/wav.c
CPPFLAGS=-Wall -Wpedantic -Werror  -g -std=c11 -I./wav/
LDFLAGS=
OFNAME=resize

build:
	@gcc ${CPPFLAGS} ${SRC} -o ${OFNAME} ${LDFLAGS}

run: build
	@./${OFNAME}

clean:
	@rm -f ${OFNAME} *.o
