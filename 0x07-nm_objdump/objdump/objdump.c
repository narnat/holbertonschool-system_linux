#include "objdump.h"
#include "flags.h"
#include <elf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

hdr_t init_phdr(elf_t *elf)
{
    hdr_t phdr;
    Elf64_Off off;
    uint16_t size, num;

    off = GET_EHDR(elf->cls, elf->ehdr, e_phoff);
    size = GET_EHDR(elf->cls, elf->ehdr, e_phentsize);
    num = GET_EHDR(elf->cls, elf->ehdr, e_phnum);

    if (elf->data == ELFDATA2MSB)
    {
        CONVERT(elf->data, off, elf->cls, 4, 8);
        CONVERT(elf->data, size, elf->cls, 2, 2);
        CONVERT(elf->data, num, elf->cls, 2, 2);
    }

    phdr.addr = elf->ehdr + off;
    phdr.entsize = size;
    phdr.num = num;

    return (phdr);
}

hdr_t init_shdr(elf_t *elf)
{
    hdr_t shdr;
    Elf64_Off off;
    uint16_t size, num;

    off = GET_EHDR(elf->cls, elf->ehdr, e_shoff);
    size = GET_EHDR(elf->cls, elf->ehdr, e_shentsize);
    num = GET_EHDR(elf->cls, elf->ehdr, e_shnum);

    if (elf->data == ELFDATA2MSB)
    {
        CONVERT(elf->data, off, elf->cls, 4, 8);
        CONVERT(elf->data, size, elf->cls, 2, 2);
        CONVERT(elf->data, num, elf->cls, 2, 2);
    }

    shdr.addr = elf->ehdr + off;
    shdr.entsize = size;
    shdr.num = num;
    return (shdr);
}

int init_elf(elf_t *elf)
{
    int fd;

    fd = open(elf->file, O_RDONLY);
    if (fd == -1)
        return (EXIT_FAILURE);
    elf->ehdr = mmap(NULL, elf->file_size, PROT_READ, MAP_SHARED, fd, 0);
    if (elf->ehdr == MAP_FAILED)
        return (EXIT_FAILURE);
    if (check_elf((uint8_t *)elf->ehdr))
        return (EXIT_FAILURE);
    elf->cls = *(char *)(elf->ehdr + EI_CLASS);
    elf->data = *(char *)(elf->ehdr + EI_DATA);
    elf->phdr = init_phdr(elf);
    elf->shdr = init_shdr(elf);
    return (EXIT_SUCCESS);
}

static char *get_file_frmt(int cls, int data)
{
    if (cls == ELFCLASS32 && data == ELFDATA2MSB)
        return ("elf32-big");
    if (cls == ELFCLASS32)
        return ("elf32-i386");
    return ("elf64-x86-64");
}

static char *get_arch(int cls, int data)
{
    if (cls == ELFCLASS32 && data == ELFDATA2MSB)
        return ("UNKNOWN!");
    if (cls == ELFCLASS32)
        return ("i386");
    return ("i386:x86-64");
}

int get_syms_flag(elf_t *elf)
{
    int flag = 0, i;
    uint32_t type;
    size_t s_size = elf->shdr.entsize;

    for (i = 0; i < elf->shdr.num; ++i)
    {
        type = GET_SHDR(elf->cls, (elf->shdr.addr + i * s_size), sh_type);
        CONVERT(elf->data, type, elf->cls, 4, 4);
        if (type == SHT_SYMTAB || type == SHT_DYNSYM)
        {
            flag |= HAS_SYMS;
            break;
        }
    }
    return (flag);
}

int get_flags(elf_t *elf)
{
    int flags = 0;
    uint16_t type = GET_EHDR(elf->cls, elf->ehdr, e_type);

    CONVERT(elf->data, type, elf->cls, 2, 2);

    switch (type) {
    case (ET_DYN):
        flags |= DYNAMIC;
        break;
    case (ET_EXEC):
        flags |= EXEC_P;
        break;
    case (ET_REL):
        flags |= HAS_RELOC;
        break;
    case (WP_TEXT):
        flags |= WP_TEXT;
        break;
    case (HAS_LOCALS):
        flags |= HAS_LOCALS;
        break;
    case (HAS_LINENO):
        flags |= HAS_LINENO;
        break;
    case (HAS_DEBUG):
        flags |= HAS_DEBUG;
        break;
    case (BFD_NO_FLAGS):
        flags |= BFD_NO_FLAGS;
        break;
    default:
        break;
    }

    if (elf->phdr.num > 0)
        flags |= D_PAGED;
    flags |= get_syms_flag(elf);

    return (flags);
}
void print_flags(int flags)
{
    int printed = 0;
    int flgs_arr[] = {BFD_NO_FLAGS, HAS_RELOC, EXEC_P, HAS_LINENO, HAS_DEBUG,
        HAS_SYMS, HAS_LOCALS, DYNAMIC, WP_TEXT, D_PAGED};
    char *flgs_names[] = {"BFD_NO_FLAGS", "HAS_RELOC", "EXEC_P", "HAS_LINENO", "HAS_DEBUG",
        "HAS_SYMS", "HAS_LOCALS", "DYNAMIC", "WP_TEXT", "D_PAGED"};
    size_t size = sizeof(flgs_arr) / sizeof(flgs_arr[0]), i;

    for (i = 0; i < size; ++i)
    {
        if (flags & flgs_arr[i])
        {
            printf("%s%s", printed ? ", " : "", flgs_names[i]);
            printed = 1;
        }
    }
    putchar('\n');
}

void print_file_headers(elf_t *elf)
{
    Elf64_Off addr;
    int flags = get_flags(elf);

    addr = GET_EHDR(elf->cls, elf->ehdr, e_entry);
    CONVERT(elf->data, addr, elf->cls, 4, 8);
    printf("\n%s:     file format %s\n", elf->file, get_file_frmt(elf->cls, elf->data));
    printf("architecture: %s, flags %#010x:\n", get_arch(elf->cls, elf->data), flags);
    print_flags(flags);
    printf("start address 0x%0*lx\n\n", elf->cls == ELFCLASS32 ? 8 : 16, addr);
}


void objdump(elf_t *elf)
{
    print_file_headers(elf);
}

/**
 * main - Entry point, nm -p
 * @argc: argument count
 * @argv: argument vector
 * Return: 0 on success, 1 otherwise
 */
int main(int argc, char *argv[])
{
    struct stat st;
    elf_t elf;

	if (argc != 2)
	{
		fprintf(stderr, "hobjdump [objfile ...]\n");
		return (EXIT_SUCCESS);
	}
	if (access(argv[1], F_OK) == -1)
    {
        fprintf(stderr, "objdump: '%s': No such file\n", argv[1]);
		return (EXIT_FAILURE);
	}
	if (access(argv[1], R_OK) == -1 || stat(argv[1], &st) == -1)
	{
		return (EXIT_FAILURE);
	}
    elf.file_size = st.st_size;
    elf.file = argv[1];
    if (init_elf(&elf))
        return (EXIT_FAILURE);
    objdump(&elf);

    return (0);
}
