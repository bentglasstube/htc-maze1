SRC=$(wildcard *.c)
OBJ=$(patsubst %.c,%.o,$(SRC))

all: test

run: $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c -O3 -o $@ $<

clean:
	rm *.o run

test: run
	t/runtests

.PHONY: all clean test
