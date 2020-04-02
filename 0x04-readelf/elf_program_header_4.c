#include "_elf.h"

/**
 * get_pheader_memsz - Get pheader memsize
 * @pheader: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 * Return: memsz
 */
uint64_t get_pheader_memsz(unsigned char *pheader, int class, int endianess)
{
	uint64_t memsz = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_memsz :
		((Elf64_Phdr *) pheader)->p_memsz;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &memsz, class == ELFCLASS32 ? 4 : 8);

	return (memsz);
}

/**
 * print_pheader_msize - Print pheader msize
 * @pheader: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_pheader_msize(unsigned char *pheader, int class, int endianess)
{
	uint64_t memsz = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_memsz :
		((Elf64_Phdr *) pheader)->p_memsz;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &memsz, class == ELFCLASS32 ? 4 : 8);

	printf(" 0x%0*lx", class == ELFCLASS32 ? 5 : 6, memsz);
}

/**
 * print_pheader_flg - Print pheader flg
 * @pheader: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_pheader_flg(unsigned char *pheader, int class, int endianess)
{
	uint32_t flags = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_flags :
		((Elf64_Phdr *) pheader)->p_flags;
	char str[4] = {0};

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &flags, class == ELFCLASS32 ? 4 : 8);

	str[0] = flags & PF_R ? 'R' : ' ';
	str[1] = flags & PF_W ? 'W' : ' ';
	str[2] = flags & PF_X ? 'E' : ' ';

	printf(" %s", str);
}

/**
 * print_pheader_align - Print pheader align
 * @pheader: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_pheader_align(unsigned char *pheader, int class, int endianess)
{
	uint64_t align = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_align :
		((Elf64_Phdr *) pheader)->p_align;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &align, class == ELFCLASS32 ? 4 : 8);

	printf(" %#lx\n", align);
}

/**
 * get_program_offset - Get pheadr offset
 * @pheader: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 * Return: offset
 */
Elf64_Off get_program_offset(unsigned char *pheader, int class, int endianess)
{
	Elf64_Off offset = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_offset :
		((Elf64_Phdr *) pheader)->p_offset;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &offset, class == ELFCLASS32 ? 4 : 8);

	return (offset);
}
