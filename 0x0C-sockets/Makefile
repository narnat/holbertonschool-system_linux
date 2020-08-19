##
# Todo API using sockets
#
# @version 0.1
# Malloc implementation

CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -ggdb3

TASK0 = todo_api_0
TASK1 = todo_api_1
TASK2 = todo_api_2
TASK3 = todo_api_3
TASK4 = realloc

$(TASK3): $(TASK3).c rest_api.h rest_api.c
	$(CC) $(CFLAGS) -o $@  $^

$(TASK2): $(TASK2).c rest_api.h rest_api.c
	$(CC) $(CFLAGS) -o $@  $^

$(TASK1): $(TASK1).c rest_api.h rest_api.c
	$(CC) $(CFLAGS) -o $@  $^

$(TASK0): $(TASK0).c rest_api.h rest_api.c
	$(CC) $(CFLAGS) -o $@  $^

.PHONY: clean, check, run

clean:
	rm $(TASK0) $(TASK1) $(TASK2) $(TASK3) $(TASK4)

check:
	betty *.c *.h

# end
