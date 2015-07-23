SRC=$(wildcard *.c)
OBJ=$(patsubst %.c,%.o,$(SRC))

all: run

run: $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c -O3 -o $@ $<

clean:
	rm *.o run

.PHONY: all clean
