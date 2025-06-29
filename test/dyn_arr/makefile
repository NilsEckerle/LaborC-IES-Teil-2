all: clean build run

clean:
	rm -f main

run:
	./main

build:
	gcc -g -Wall -Wextra -std=gnu99 -I../../include/ -o main main.c dynamic_array.c

.PHONY: clean run build
