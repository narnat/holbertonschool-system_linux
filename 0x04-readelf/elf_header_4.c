#include "_elf.h"

/**
 * print_header_size - Print header size
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_header_size(unsigned char *bytes, int class, int endianess)
{
	uint16_t ehsize = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_ehsize :
		((Elf64_Ehdr *) bytes)->e_ehsize;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &ehsize, 2);
	printf("  Size of this header:               %d (bytes)\n", ehsize);
}

/**
 * print_program_header_size - Print program_header_size
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_program_header_size(unsigned char *bytes, int class, int endianess)
{
	uint16_t phentsize = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_phentsize :
		((Elf64_Ehdr *) bytes)->e_phentsize;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &phentsize, 2);
	printf("  Size of program headers:           %d (bytes)\n", phentsize);
}

/**
 * print_num_program_headers - Print num_program_headers
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_num_program_headers(unsigned char *bytes, int class, int endianess)
{
	uint16_t phnum = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_phnum :
		((Elf64_Ehdr *) bytes)->e_phnum;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &phnum, 2);
	printf("  Number of program headers:         %d\n", phnum);
}

/**
 * print_section_hsize - Print section size
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_section_hsize(unsigned char *bytes, int class, int endianess)
{
	uint16_t shentsize = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_shentsize :
		((Elf64_Ehdr *) bytes)->e_shentsize;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &shentsize, 2);
	printf("  Size of section headers:           %d (bytes)\n", shentsize);
}

/**
 * print_num_section_headers - Print number of section headers
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_num_section_headers(unsigned char *bytes, int class, int endianess)
{
	uint16_t shnum = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_shnum :
		((Elf64_Ehdr *) bytes)->e_shnum;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &shnum, 2);
	printf("  Number of section headers:         %d\n", shnum);
}
