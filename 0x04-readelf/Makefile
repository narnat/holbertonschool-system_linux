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
FILES_0 = 0-hreadelf.o elf_header.o elf_header_2.o elf_header_3.o elf_header_4.o
FILES_1 = 1-hreadelf.o elf_section.o elf_section_2.o elf_section_3.o

0-hreadelf: utils.c $(FILES_0) $(HEADERS)

1-hreadelf:  utils_2.c utils.c $(FILES_1) $(HEADERS)

2-hreadelf: 2-hreadelf.c utils.c elf_section_2.c elf_section_3.c elf_section.c  utils_2.c $(HEADERS)

100-hreadelf: 100-hreadelf.o $(HEADERS)

.PHONY: clean, check, memcheck, memfull

clean:
	rm -f *.o 0-hreadelf 1-hreadelf 2-hreadelf 100-hreadelf
