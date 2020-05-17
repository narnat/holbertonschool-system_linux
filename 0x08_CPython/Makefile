CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -ggdb3 -shared -Wl,-soname,PyList -fPIC -I/usr/include/python3.8

.PHONY: clean, check

libPyList.so: 1-python.c
	$(CC) $(CFLAGS) -o $@  $^

clean:
	rm libPyList.so

check:
	betty *.c
