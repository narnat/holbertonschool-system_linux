#include "_elf.h"

/**
 * print_pheader_vaddr - Print pheader vaddr
 * @pheader: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_pheader_vaddr(unsigned char *pheader, int class, int endianess)
{
	Elf64_Off vaddr = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_vaddr :
		((Elf64_Phdr *) pheader)->p_vaddr;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &vaddr, class == ELFCLASS32 ? 4 : 8);

	printf(" 0x%0*lx", class == ELFCLASS32 ? 8 : 16, vaddr);
}

/**
 * get_pheader_vaddr - Get pheader vaddr
 * @pheader: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 * Return: vaddr
 */
Elf64_Off get_pheader_vaddr(unsigned char *pheader, int class, int endianess)
{
	Elf64_Off vaddr = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_vaddr :
		((Elf64_Phdr *) pheader)->p_vaddr;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &vaddr, class == ELFCLASS32 ? 4 : 8);

	return (vaddr);
}

/**
 * print_pheader_paddr - Print pheader paddr
 * @pheader: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_pheader_paddr(unsigned char *pheader, int class, int endianess)
{
	Elf64_Off paddr = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_paddr :
		((Elf64_Phdr *) pheader)->p_paddr;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &paddr, class == ELFCLASS32 ? 4 : 8);

	printf(" 0x%0*lx", class == ELFCLASS32 ? 8 : 16, paddr);
}

/**
 * print_pheader_fsize - Print pheader fsize
 * @pheader: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_pheader_fsize(unsigned char *pheader, int class, int endianess)
{
	uint64_t filesz = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_filesz :
		((Elf64_Phdr *) pheader)->p_filesz;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &filesz, class == ELFCLASS32 ? 4 : 8);

	printf(" 0x%0*lx", class == ELFCLASS32 ? 5 : 6, filesz);
}

/**
 * get_pheader_fsize - Get pheader fsize
 * @pheader: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 * Return: fsize
 */
uint64_t get_pheader_fsize(unsigned char *pheader, int class, int endianess)
{
	uint64_t filesz = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_filesz :
		((Elf64_Phdr *) pheader)->p_filesz;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &filesz, class == ELFCLASS32 ? 4 : 8);

	return (filesz);
}
