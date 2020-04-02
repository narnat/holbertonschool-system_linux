#include "_elf.h"

/**
 * print_interpreter - Print interpreter
 * @pheader: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 * @filename: elf file
 */
void print_interpreter(unsigned char *pheader, int class, int endianess,
		       char *filename)
{
	Elf64_Off offset = get_program_offset(pheader, class, endianess);
	Elf64_Off memsz = get_pheader_memsz(pheader, class, endianess);
	unsigned char *interp;

	read_bytes(&interp, filename, offset, memsz);
	printf("      [Requesting program interpreter: %s]\n", interp);
	free(interp);
}

/**
 * print_pheader_loop- Print all pheadr info
 * @pheader: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 * @n_pheader: number of Phdr
 * @header_size: size of Phdr
 * @filename: elf file
 */
void print_pheader_loop(unsigned char *pheader, int class, int endianess,
			uint16_t n_pheader, uint16_t header_size, char *filename)
{
	size_t i;
	int is_interp;

	for (i = 0; i < n_pheader; ++i, pheader += header_size)
	{
		is_interp = print_pheader_type(pheader, class, endianess);
		print_pheader_offset(pheader, class, endianess);
		print_pheader_vaddr(pheader, class, endianess);
		print_pheader_paddr(pheader, class, endianess);
		print_pheader_fsize(pheader, class, endianess);
		print_pheader_msize(pheader, class, endianess);
		print_pheader_flg(pheader, class, endianess);
		print_pheader_align(pheader, class, endianess);
		if (is_interp)
			print_interpreter(pheader, class, endianess, filename);
	}

}

/**
 * print_section_name_2 - print section name
 * @data: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 * @str_table: string table
 */
void print_section_name_2(unsigned char *data, int class, int endianess,
			  unsigned char *str_table)
{
	uint32_t name = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_name :
		((Elf64_Shdr *) data)->sh_name;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &name, class == ELFCLASS32 ? 4 : 4);

	if (*(str_table + name))
		printf("%s ", str_table + name);
}

/**
 * get_section_addr - get section addr
 * @data: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 * Return: section addr
 */
Elf64_Addr get_section_addr(unsigned char *data, int class, int endianess)
{
	Elf64_Addr addr = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_addr :
		((Elf64_Shdr *) data)->sh_addr;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &addr, class == ELFCLASS32 ? 4 : 8);
	return (addr);
}

/**
 * get_sh_size - print section size
 * @data: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 * Return: section header size
 */
uint64_t get_sh_size(unsigned char *data, int class, int endianess)
{
	uint64_t size = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_size :
		((Elf64_Shdr *) data)->sh_size;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &size, class == ELFCLASS32 ? 4 : 8);
	return (size);
}
