# Make file for shell v2 project at Holberton school
# List of variables
SHELL = /bin/bash
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu89 -g
# Find all c files in current directory and substitute
# extension with object files (.o)
# OBJ = $(patsubst %.c,%.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)
LINTER = betty

# Compile object files only when they are modified,
# It's implicitly compiled
# %.o: %.c
# 	$(CC) -c $< $(CFLAGS)

0-hreadelf: 0-hreadelf.o $(HEADERS)

1-hreadelf: 1-hreadelf.o $(HEADERS)

2-hreadelf: 2-hreadelf.o $(HEADERS)

100-hreadelf: 100-hreadelf.o $(HEADERS)

.PHONY: clean, check, memcheck, memfull

clean:
	rm -f *.o 0-hreadelf 1-hreadelf 2-hreadelf 100-hreadelf

# Compile and run  command, @ means do not print info
check:
	@$(LINTER) *.[ch]

memcheck: all
	@valgrind ./$(NAME)

memfull: all
	@valgrind --leak-check=full --track-origins=yes ./$(NAME)
