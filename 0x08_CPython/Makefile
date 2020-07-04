CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -ggdb3 -shared -Wl,-soname,PyList -fPIC -I/usr/include/python3.8

.PHONY: clean, check

# libPython.so.2: 100-python.c
# 	$(CC) $(CFLAGS) -o $@  $^

libPython.so: 3-python.c
	$(CC) $(CFLAGS) -o $@  $^

# libPython.so: 2-python.c
# 	$(CC) $(CFLAGS) -o $@  $^

libPyList.so: 1-python.c
	$(CC) $(CFLAGS) -o $@  $^

clean:
	rm libPyList.so

check:
	betty *.c
