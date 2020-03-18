#include "_elf.h"

/**
 * print_file_version - Print file version
 * @bytes: character array
 * @endianess: LSB or MSB
 */
void print_file_version(unsigned char *bytes, int endianess)
{
	uint16_t file_version = ((Elf64_Ehdr *) bytes)->e_version;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &file_version, 2);

	printf("  Version:                           ");

	switch (file_version)
	{
	case EV_NONE:
		/* puts("Invalid version"); */
		/* break; */
	case EV_CURRENT:
		puts("0x1");
		break;
	default:
		printf("%#x\n", file_version);
	}

}

/**
 * print_entry_point_addr - Print entry point address
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_entry_point_addr(unsigned char *bytes, int class, int endianess)
{
	Elf64_Addr *entry64;
	Elf32_Addr *entry32;

	printf("  Entry point address:               ");
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
 * print_program_headers - Print program headers
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_program_headers(unsigned char *bytes, int class, int endianess)
{
	Elf64_Off *headers64;
	Elf32_Off *headers32;

	printf("  Start of program headers:          ");
	if (class == ELFCLASS32)
	{
		headers32 = &((Elf32_Ehdr *) bytes)->e_phoff;
		if (endianess == ELFDATA2MSB)
			reverse((unsigned char *) headers32, 4);
		printf("%u", *headers32);

	}
	else
	{
		headers64 = &((Elf64_Ehdr *) bytes)->e_phoff;
		if (endianess == ELFDATA2MSB)
			reverse((unsigned char *) headers64, 8);
		printf("%lu", *headers64);

	}
	puts(" (bytes into file)");
}

/**
 * print_section_headers - Print section headers
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_section_headers(unsigned char *bytes, int class, int endianess)
{
	Elf64_Off *headers64;
	Elf32_Off *headers32;

	printf("  Start of section headers:          ");
	if (class == ELFCLASS32)
	{
		headers32 = &((Elf32_Ehdr *) bytes)->e_shoff;
		if (endianess == ELFDATA2MSB)
			reverse((unsigned char *) headers32, 4);
		printf("%u", *headers32);

	}
	else
	{
		headers64 = &((Elf64_Ehdr *) bytes)->e_shoff;
		if (endianess == ELFDATA2MSB)
			reverse((unsigned char *) headers64, 8);
		printf("%lu", *headers64);

	}
	puts(" (bytes into file)");
}

/**
 * print_flags - Print flags
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_flags(unsigned char *bytes, int class, int endianess)
{
	uint32_t flags = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_flags :
		((Elf64_Ehdr *) bytes)->e_flags;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &flags, 4);
	printf("  Flags:                             0x%x\n", flags);
}
