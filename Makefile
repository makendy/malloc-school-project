CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -fPIC\
         -fvisibility=hidden -fno-builtin -g -I ./include/
VPATH = src/ tests/
LDFLAGS = -shared
TARGET_LIB = libmalloc.so
LDLIBS = -lcheck -lpthread -lm -lrt -lsubunit

OBJS = malloc.o my_program.o
TEST_OBJS = unit_tests.o my_program.o
TBIN= unit_tests

${TARGET_LIB}: ${OBJS}
	${CC} ${LDFLAGS} -o $@ $^

${TBIN}: ${TEST_OBJS}
	${CC} ${CFLAGS} $^ ${LDLIBS} -o $@

all: ${TARGET_LIB}

test: ${TBIN}

check: all test
	./unit_tests
	@ python3 ./tests/app_test.py

clean:
	${RM} ${TARGET_LIB} ${OBJS} ${TEST_OBJS} ${TBIN}

.PHONY: all ${TARGET_LIB} clean test check ${TBIN}
