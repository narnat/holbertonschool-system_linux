#include "_elf.h"

/**
 * print_section_name - print section name
 * @data: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 * @str_table: string table
 */
void print_section_name(unsigned char *data, int class, int endianess,
			unsigned char *str_table)
{
	uint32_t name = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_name :
		((Elf64_Shdr *) data)->sh_name;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &name, class == ELFCLASS32 ? 4 : 4);

	printf("%-17s", str_table + name);
}

/**
 * print_section_addr - print section addr
 * @data: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_section_addr(unsigned char *data, int class, int endianess)
{
	Elf64_Addr addr = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_addr :
		((Elf64_Shdr *) data)->sh_addr;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &addr, class == ELFCLASS32 ? 4 : 8);
	printf(" %0*lx", class == ELFCLASS32 ? 8 : 16, addr);
}

/**
 * print_section_off - print section offset
 * @data: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_section_off(unsigned char *data, int class, int endianess)
{
	Elf64_Off offset = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_offset :
		((Elf64_Shdr *) data)->sh_offset;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &offset, class == ELFCLASS32 ? 4 : 8);
	printf(" %06lx", offset);
}

/**
 * print_section_size - print section size
 * @data: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_section_size(unsigned char *data, int class, int endianess)
{
	uint64_t size = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_size :
		((Elf64_Shdr *) data)->sh_size;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &size, class == ELFCLASS32 ? 4 : 8);
	printf(" %06lx", size);
}

/**
 * print_section_es - print section entry size
 * @data: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_section_es(unsigned char *data, int class, int endianess)
{
	uint64_t entsize = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_entsize :
		((Elf64_Shdr *) data)->sh_entsize;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &entsize, class == ELFCLASS32 ? 4 : 8);
	printf(" %02lx", entsize);
}
