#!/usr/bin/python3
import sys

''' This script changes ASCII string in heap of the process '''


def get_args():
    '''
    Parses arguments rand returns them
    argv[1]: <pid>
    argv[2]: <search_string>
    argv[3]: <replace_string>
    '''

    pid = int(sys.argv[1])
    search_string = sys.argv[2]
    replace_string = sys.argv[3]
    if len(replace_string) > len(search_string):
        raise IndexError

    return pid, search_string, replace_string


def read_write_file(pid, search_string, replace_string):
    '''
    Read mem file and swap the given string
    '''

    maps_file = "/proc/{:d}/maps".format(pid)
    mem_file = "/proc/{:d}/mem".format(pid)
    search_word = "[heap]"
    props_line = {"start": None, "end": None, "perms": None}

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

        if props_line["perms"][:2] != "rw":
            raise PermissionError

        mem.seek(props_line["start"])
        content = mem.read(props_line["end"] - props_line["start"])
        idx = content.index(bytes(search_string, "ASCII"))
        print("[*] Found string")
        print("[*] Replacing string")
        mem.seek(props_line["start"] + idx)
        mem.write(bytes(replace_string + "\0", "ASCII"))


def main():
    '''
    Entry point of the script
    '''
    try:
        exit_value = 1
        pid, search_string, replace_string = get_args()
        read_write_file(pid, search_string, replace_string)
        exit_value = 0
    except IndexError:
        print("Usage: <read_write_heap.py> pid search_string replace_string")
    except ValueError:
        print("PID should be a valid integer greater than 0")
    except FileNotFoundError as e:
        print(e)
    except ValueError:
        print("Could not find the string")
    except PermissionError:
        print("No permission")
    finally:
        exit(exit_value)

if __name__ == '__main__':
    main()
