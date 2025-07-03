all: clean symlinks build run

clean:
	rm -f main
	rm -f ./dynamic_array.c
	rm -f ./dynamic_array.h

symlinks:
	ln -sf $(PWD)/../../include/tools/dynamic_array.h ./dynamic_array.h
	ln -sf $(PWD)/../../src/tools/dynamic_array.c ./dynamic_array.c

run:
	./main


build:
	gcc -g -Wall -Wextra -std=gnu99 -I../../include/ -o main main.c dynamic_array.c -DLOGGER_USE_PRINTF

.PHONY: clean run build symlinks
