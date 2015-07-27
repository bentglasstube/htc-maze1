SRC=$(wildcard *.c)
OBJ=$(patsubst %.c,%.o,$(SRC))

all: run

run: $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -Wall -c -O3 -o $@ $<

clean:
	rm -f *.o run

test: run
	t/runtests

.PHONY: all clean test
