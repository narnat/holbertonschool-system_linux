#include "_elf.h"

/**
 * print_magic_bytes - Prints magic bytes of elf, first 16 bytes
 * @bytes: character array
 */
void print_magic_bytes(unsigned char *bytes)
{
	size_t i;
	unsigned char *ident = ((Elf64_Ehdr *)bytes)->e_ident;

	printf("  Magic:   ");
	for (i = 0; i < 16; ++i)
	{
		printf("%02x ", ident[i]);
	}
	printf("\n");
}

/**
 * print_string_table_idx - Print Section header string table index
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_string_table_idx(unsigned char *bytes, int class, int endianess)
{
	uint16_t shstrndx = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_shstrndx :
		((Elf64_Ehdr *) bytes)->e_shstrndx;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &shstrndx, 2);

	printf("  Section header string table index: %d\n", shstrndx);
}

/**
 * print_elf_header - Print all infor from elf header
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_elf_header(unsigned char *bytes, int class, int endianess)
{
	printf("ELF Header:\n");
	print_magic_bytes(bytes);
	print_class(bytes);
	print_data(bytes);
	print_version(bytes);
	print_osabi(bytes);
	print_abi_version(bytes);
	print_type(bytes, endianess);
	print_machine(bytes, endianess);
	print_file_version(bytes, endianess);
	print_entry_point_addr(bytes, class, endianess);
	print_program_headers(bytes, class, endianess);
	print_section_headers(bytes, class, endianess);
	print_flags(bytes, class, endianess);
	print_header_size(bytes, class, endianess);
	print_program_header_size(bytes, class, endianess);
	print_num_program_headers(bytes, class, endianess);
	print_section_hsize(bytes, class, endianess);
	print_num_section_headers(bytes, class, endianess);
	print_string_table_idx(bytes, class, endianess);
}

/**
 * main - Entry point, output is similar to readelf -W -h
 * @argc: argument count
 * @argv: argument vector
 * Return: 0 on success, 1 otherwise
 */
int main(int argc, char *argv[])
{
	unsigned char bytes[64];

	if (argc != 2)
	{
		/* fprintf(stderr, "readelf: Warning: Nothing to do.\n"); */
		/* fprintf(stderr, "Usage: readelf <option(s)> elf-file(s)\n"); */
		return (EXIT_SUCCESS);
	}
	if (access(argv[1], F_OK) == -1)
	{
		fprintf(stderr, "readelf: Error: '%s': No such file\n", argv[1]);
		return (EXIT_FAILURE);
	}
	if (access(argv[1], R_OK) == -1)
	{
		fprintf(stderr,
			"readelf: Error: %s: Failed to read file's magic number\n",
			argv[1]);
		return (EXIT_FAILURE);
	}
	if (read_elf_header_bytes(bytes, argv[1]))
	{
		perror("readelf: Error: ");
		return (EXIT_FAILURE);
	}
	if (check_elf(bytes))
	{
		return (EXIT_FAILURE);
	}
	print_elf_header(bytes,
		     bytes[4] == ELFCLASS32 ? ELFCLASS32 : ELFCLASS64,
		     bytes[5] == ELFDATA2MSB ? ELFDATA2MSB : ELFDATA2LSB);
	return (0);
}
