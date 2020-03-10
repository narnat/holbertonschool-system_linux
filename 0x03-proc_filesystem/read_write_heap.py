#!/usr/bin/python3
import sys

''' This script changes ASCII string in heap of the process '''

try:
    pid = int(sys.argv[1])
    search_string = sys.argv[2]
    replace_string = sys.argv[3]
    if len(replace_string) > len(search_string):
        raise IndexError
except IndexError:
    print("Usage: <read_write_heap.py> pid search_string replace_string")
    exit(1)
except ValueError:
    print("PID should be a valid integer greater than 0")
    exit(1)


maps_file = "/proc/{:d}/maps".format(pid)
mem_file = "/proc/{:d}/mem".format(pid)
search_word = "[heap]"
props_line = {"start": None, "end": None, "perms": None}

try:
    with open(maps_file, "r") as maps, open(mem_file, "rb+") as mem:
        for line in maps:
            if search_word in line:
                print("[*] Found heap")
                tmp = line.split(" ")
                addr = tmp[0].split("-")
                props_line["start"] = int(addr[0], 16)
                props_line["end"] = int(addr[1], 16)
                props_line["perms"] = tmp[1]
                break

        if props_line["perms"] != "rw-p":
            raise PermissionError

        mem.seek(props_line["start"])
        content = mem.read(props_line["end"] - props_line["start"])
        idx = content.index(bytes(search_string + "\0", "ASCII"))
        print("[*] Found string")
        print("[*] Replacing string")
        mem.seek(props_line["start"] + idx)
        mem.write(bytes(replace_string + "\0", "ASCII"))

except FileNotFoundError as e:
    print(e, file=sys.stderr)
    exit(1)

except ValueError:
    print("Could not find the string")
    exit(1)

except PermissionError:
    print("No permission")
    exit(1)
