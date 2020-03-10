# Proc filesystem
Write a script that finds a string in the heap of a running process, and replaces it.

    Usage: read_write_heap.py pid search_string replace_string
        where pid is the pid of the running process
        and strings are ASCII
    The script should look only in the heap of the process
    Output: you can print whatever you think is interesting
    On usage error, print an error message on stdout and exit with status code 1

