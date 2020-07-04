CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -ggdb3 -std=gnu90
HEADERS = strace.h syscalls.h
TASK_0 = strace_0
TASK_1 = strace_1
TASK_2 = strace_2
TASK_3 = strace_3
TASK_4 = strace_4
TASK_5 = strace_5
TASK_6 = strace_6
TASK_7 = strace_7
TASK_8 = strace_8
UTILS = utils

.PHONY: clean, check

# $(TASK_8): $(TASK_8).o $(UTILS).o $(HEADERS)
# 	$(CC) $(CFLAGS) -o $@  $^

# $(TASK_7): $(TASK_7).o $(UTILS).o $(HEADERS)
# 	$(CC) $(CFLAGS) -o $@  $^

$(TASK_6): $(TASK_6).o $(UTILS).o $(HEADERS)
	$(CC) $(CFLAGS) -o $@  $^

$(TASK_5): $(TASK_5).o $(UTILS).o $(HEADERS)
	$(CC) $(CFLAGS) -o $@  $^

$(TASK_4): $(TASK_4).o $(UTILS).o $(HEADERS)
	$(CC) $(CFLAGS) -o $@  $^

$(TASK_3): $(TASK_3).o $(HEADERS)
	$(CC) $(CFLAGS) -o $@  $^

$(TASK_2): $(TASK_2).o $(HEADERS)
	$(CC) $(CFLAGS) -o $@  $^

$(TASK_1): $(TASK_1).o $(HEADERS)
	$(CC) $(CFLAGS) -o $@  $^

$(TASK_0): $(TASK_0).o $(HEADERS)
	$(CC) $(CFLAGS) -o $@  $^

clean:
	rm *.o $(TASK_0) $(TASK_1) $(TASK_2) $(TASK_3) $(TASK_4) $(TASK_5) $(TASK_6) $(TASK_7) $(TASK_8)

check:
	betty *.c *.h
