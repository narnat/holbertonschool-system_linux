#include "_elf.h"

/**
 * print_type_2 - Print elf type
 * @bytes: character array
 * @endianess: LSB or MSB
 */
void print_type_2(unsigned char *bytes, int endianess)
{
	uint16_t type = ((Elf64_Ehdr *) bytes)->e_type;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &type, 2);
	printf("\nElf file type is ");
	switch (type)
	{
	case ET_NONE:
		puts("NONE (None)");
		break;
	case ET_REL:
		puts("REL (Relocatable file)");
		break;
	case ET_EXEC:
		puts("EXEC (Executable file)");
		break;
	case ET_DYN:
		puts("DYN (Shared object file)");
		break;
	case ET_CORE:
		puts("CORE (Core file)");
		break;
	default:
		puts("Unknown type");
		break;
	}
}

/**
 * print_entry_point_addr_2 - Print entry point address
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_entry_point_addr_2(unsigned char *bytes, int class, int endianess)
{
	Elf64_Addr *entry64;
	Elf32_Addr *entry32;

	printf("Entry point ");
	if (class == ELFCLASS32)
	{
		entry32 = &((Elf32_Ehdr *) bytes)->e_entry;
		if (endianess == ELFDATA2MSB)
			reverse((unsigned char *) entry32, 4);
		printf("%#x\n", *entry32);

	}
	else
	{
		entry64 = &((Elf64_Ehdr *) bytes)->e_entry;
		if (endianess == ELFDATA2MSB)
			reverse((unsigned char *) entry64, 8);
		printf("%#lx\n", *entry64);

	}
}

/**
 * get_ph_offset - Get program header offset
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 * Return: return offset
 */
Elf64_Off get_ph_offset(unsigned char *bytes, int class, int endianess)
{
	Elf64_Off phoff = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_phoff :
		((Elf64_Ehdr *) bytes)->e_phoff;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &phoff, class == ELFCLASS32 ? 4 : 8);
	return (phoff);
}

/**
 * get_program_header_size - Get program_header_size
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 * Return: program header size
 */
uint16_t get_program_header_size(unsigned char *bytes,
				 int class, int endianess)
{
	uint16_t phentsize = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_phentsize :
		((Elf64_Ehdr *) bytes)->e_phentsize;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &phentsize, 2);
	return (phentsize);
}

/**
 * get_num_program_headers - Get number of program headers
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 * Return: number of program headers
 */
uint16_t get_num_program_headers(unsigned char *bytes,
				 int class, int endianess)
{
	uint16_t phnum = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_phnum :
		((Elf64_Ehdr *) bytes)->e_phnum;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &phnum, 2);
	return (phnum);
}
