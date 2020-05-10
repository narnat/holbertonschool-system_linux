#include "objdump.h"
#include "flags.h"
#include <stdint.h>
#include <stdlib.h>
#define ALIGNMENT 16

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

    phdr.addr = (uint8_t *)(elf->ehdr) + off;
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

    shdr.addr = (uint8_t *)(elf->ehdr) + off;
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
        return (2);
    elf->cls = *(char *)((uint8_t *)(elf->ehdr) + EI_CLASS);
    elf->data = *(char *)((uint8_t *)(elf->ehdr) + EI_DATA);
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
        type = GET_SHDR(elf->cls, ((uint8_t *)(elf->shdr.addr) + i * s_size), sh_type);
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
    char *flgs_names[] = {"BFD_NO_FLAGS", "HAS_RELOC", "EXEC_P", "HAS_LINENO",
        "HAS_DEBUG", "HAS_SYMS", "HAS_LOCALS", "DYNAMIC", "WP_TEXT", "D_PAGED"};
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

int skip_section(elf_t *elf, void *shdr)
{
    uint64_t size, flags;
    uint32_t type;

    size = GET_SHDR(elf->cls, shdr, sh_size);
    flags = GET_SHDR(elf->cls, shdr, sh_flags);
    type = GET_SHDR(elf->cls, shdr, sh_type);
    CONVERT(elf->data, size, elf->cls, 4, 8);
    CONVERT(elf->data, flags, elf->cls, 4, 8);
    CONVERT(elf->data, type, elf->cls, 4, 4);

    if ((type != SHT_NULL && size != 0 && type != SHT_NOBITS &&
         type != SHT_SYMTAB && type != SHT_STRTAB && type != SHT_RELA) ||
        (type == SHT_STRTAB && (flags & SHF_ALLOC)) ||
        (type == SHT_RELA &&
         (flags & SHF_ALLOC)))
        return (0);
    return (1);
}

void print_hex(uint8_t *bytes, uint64_t printed, uint64_t size)
{
    size_t i;

    for (i = printed; i < printed + ALIGNMENT; ++i)
    {
        if (i % 4 == 0)
            printf(" ");
        if (i >= size)
            printf("  ");
        else
            printf("%02x", *(bytes + i));
    }
}

void print_ascii(uint8_t *bytes, uint64_t printed, uint64_t size)
{
    size_t i;
    uint8_t c;

    printf("  ");
    for (i = printed; i < printed + ALIGNMENT; ++i)
    {
        if (i >= size)
            printf(" ");
        else
        {
            c = *(bytes + i);
            if (c < 32 || c > 126)
                printf(".");
            else
                printf("%c", c);
        }
    }
}

void print_bytes(elf_t *elf, void *shdr)
{
    uint64_t size, offset, addr, aligned_size, i;

    size = GET_SHDR(elf->cls, shdr, sh_size);
    offset = GET_SHDR(elf->cls, shdr, sh_offset);
    addr = GET_SHDR(elf->cls, shdr, sh_addr);
    CONVERT(elf->data, size, elf->cls, 4, 8);
    CONVERT(elf->data, offset, elf->cls, 4, 8);
    CONVERT(elf->data, addr, elf->cls, 4, 8);
    if (size % ALIGNMENT)
        aligned_size = size + ALIGNMENT - size % ALIGNMENT;
    else
        aligned_size = size;

    for (i = 0; i < aligned_size; i += ALIGNMENT)
    {
        printf(" %04lx", addr + i);
        print_hex((uint8_t *)(elf->ehdr) + offset, i, size);
        print_ascii((uint8_t *)(elf->ehdr) + offset, i, size);
        printf("\n");
    }
}

void print_sections(elf_t *elf, uint8_t *str_tab)
{
    uint16_t size = elf->shdr.entsize, num = elf->shdr.num;
    void *addr = elf->shdr.addr;
    size_t i;
    uint32_t name = 0;

    for (i = 0; i < num; ++i)
    {
        if (!skip_section(elf, (uint8_t *)(addr) + i * size))
        {
            name = GET_SHDR(elf->cls, (uint8_t *)(addr) + i * size, sh_name);
            CONVERT(elf->data, name, elf->cls, 4, 4);
            printf("Contents of section %s:\n", str_tab + name);
            print_bytes(elf, (uint8_t *)(addr) + i * size);
        }
    }
}

void objdump(elf_t *elf)
{
    uint8_t *sh_strtab;
    uint16_t indx, sh_size = elf->shdr.entsize;
    Elf64_Off str_off;

    indx = GET_EHDR(elf->cls, elf->ehdr, e_shstrndx);
    CONVERT(elf->data, indx, elf->cls, 2, 2);
    str_off = GET_SHDR(elf->cls, ((uint8_t *)(elf->shdr.addr) + sh_size * indx), sh_offset);
    CONVERT(elf->data, str_off, elf->cls, 4, 8);
    sh_strtab = (uint8_t *)(elf->ehdr) + str_off;
    print_file_headers(elf);
    print_sections(elf, sh_strtab);
}


int entry(char *cmd, char *filename)
{
    struct stat st;
    elf_t elf;
    int ret;

    if (access(filename, F_OK) == -1)
	{
		fprintf(stderr, "%s: '%s': No such file\n", cmd, filename);
		return (1);
	}

	if (access(filename, R_OK) == -1)
	{
		fprintf(stderr, "%s: %s: Permission denied\n", cmd, filename);
		return (EXIT_FAILURE);
	}

	if (stat(filename, &st) == -1)
	{
		return (EXIT_FAILURE);
	}
    elf.file_size = st.st_size;
    elf.file = filename;
    ret = init_elf(&elf);
    if (ret == 2)
    {
        fprintf(stderr, "%s: %s: File format not recognized\n", cmd, filename);
        return (EXIT_FAILURE);
    }
    if (ret == EXIT_FAILURE)
        return (EXIT_FAILURE);
    objdump(&elf);
    munmap(elf.ehdr, elf.file_size);
    return (EXIT_SUCCESS);
}

/**
 * main - Entry point, nm -p
 * @argc: argument count
 * @argv: argument vector
 * Return: 0 on success, 1 otherwise
 */
int main(int argc, char *argv[])
{
    char *def = "a.out";
	int n_files = argc - 1, i;
	int ret = 0;

	if (argc < 2)
		argv[1] = def, ++n_files;

	for (i = 1; i <= n_files; ++i)
	{
		if (entry(argv[0], argv[i]))
            ret = EXIT_FAILURE;
    }

    return (ret);
}
