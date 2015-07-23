all: run

run: main.c
	$(CC) -O3 $< -o $@

.PHONY: all
