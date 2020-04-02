#include "_elf.h"

/**
 * print_program_header_string - print header string
 * @class: ELFCLASS32 or ELFCLASS64
 */
void print_program_header_string(int class)
{
	printf("Program Headers:\n");
	printf("  Type           Offset   VirtAddr");
	if (class == ELFCLASS32)
		printf("   PhysAddr   FileSiz MemSiz  Flg Align\n");
	else
		printf("           PhysAddr           FileSiz  MemSiz   Flg Align\n");
}


/**
 * get_pheader_type - Get pheader type
 * @type: Type of header
 * Return: Type
 */
static char *get_pheader_type(uint32_t type)
{
	switch (type)
	{
	case PT_LOAD:
		return ("LOAD");
	case PT_DYNAMIC:
		return ("DYNAMIC");
	case PT_NOTE:
		return ("NOTE");
	case PT_SHLIB:
		return ("SHLIB");
	case PT_PHDR:
		return ("PHDR");
	case PT_GNU_STACK:
		return ("GNU_STACK");
	case PT_GNU_EH_FRAME:
		return ("GNU_EH_FRAME");
	case PT_GNU_RELRO:
		return ("GNU_RELRO");
	case 0x6464e550:
		return ("LOOS+464e550");
	}
	return ("NULL");
}

/**
 * print_pheader_type - Get number of program headers
 * @pheader: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 * Return: number of program headers
 */
int print_pheader_type(unsigned char *pheader, int class, int endianess)
{
	uint32_t type = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_type :
		((Elf64_Phdr *) pheader)->p_type;
	char *str;
	int is_interp = 0;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &type, 4);

	str = get_pheader_type(type);
	if (type == PT_INTERP)
	{
		str = "INTERP";
		is_interp = 1;
	}
	printf("  %-14s", str);
	return (is_interp);
}

/**
 * print_pheader_offset - Print pheader offset
 * @pheader: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_pheader_offset(unsigned char *pheader, int class, int endianess)
{
	Elf64_Off offset = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_offset :
		((Elf64_Phdr *) pheader)->p_offset;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &offset, class == ELFCLASS32 ? 4 : 8);

	printf(" 0x%06lx", offset);
}

/**
 * get_pheader_offset - Get number pheadr offset
 * @pheader: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 * Return: pheader offset
 */
Elf64_Off get_pheader_offset(unsigned char *pheader, int class, int endianess)
{
	Elf64_Off offset = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_offset :
		((Elf64_Phdr *) pheader)->p_offset;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &offset, class == ELFCLASS32 ? 4 : 8);

	return (offset);
}
