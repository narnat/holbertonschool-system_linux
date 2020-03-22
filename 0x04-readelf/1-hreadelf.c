#include "_elf.h"

/**
 * print_section_header_loop - Print info about section headers, loop part
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 * @n_sections: number of sections
 * @header_size: size of section header
 * @str_table: string table section, .shtstrtab
 */
void print_section_header_loop(unsigned char *bytes, int class, int endianess,
			       int n_sections, int header_size, unsigned char *str_table)
{
	int i;

	print_section_header_names(class);

	for (i = 0; i < n_sections; ++i, bytes += header_size)
	{
		printf("  [%2d] ", i);
		print_section_name(bytes, class, endianess, str_table);
		print_section_type(bytes, class, endianess);
		print_section_addr(bytes, class, endianess);
		print_section_off(bytes, class, endianess);
		print_section_size(bytes, class, endianess);
		print_section_es(bytes, class, endianess);
		print_section_flg(bytes, class, endianess);
		print_section_lk(bytes, class, endianess);
		print_section_inf(bytes, class, endianess);
		print_section_al(bytes, class, endianess);
	}

	print_key_to_flags(class);
}

/**
 * print_section_headers - Print info about section headers, like readelf -S -W
 * @bytes: character array
 * @filename: elf file
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_section_header(unsigned char *bytes, char *filename, int class,
			  int endianess)
{
	void *p = get_section_header_off(bytes, class, endianess);
	Elf64_Off offset = class == ELFCLASS32 ? *(Elf32_Off *) p : *(Elf64_Off *) p;
	uint16_t n_sections = get_num_section_headers(bytes, class, endianess);
	uint16_t header_size = get_section_hsize(bytes, class, endianess);
	unsigned char *data = NULL, *str_table = NULL;
	size_t s_off, s_size;
	uint16_t shstrndx;

	printf("There are %d section headers, starting at offset %#lx:\n\n",
	       n_sections, offset);

	if (read_bytes(&data, filename, offset, n_sections * header_size))
	{
		exit(EXIT_FAILURE);
	}

	shstrndx = get_string_table_idx(bytes, class, endianess);
	s_off = get_section_off(data + (shstrndx * header_size), class, endianess);
	s_size = get_section_size(data + (shstrndx * header_size), class, endianess);

	if (read_bytes(&str_table, filename, s_off, s_size))
	{
		free(data);
		exit(EXIT_FAILURE);
	}
	print_section_header_loop(data, class, endianess,
				  n_sections, header_size, str_table);
	free(data);
	free(str_table);
}

/**
 * main - Entry point, output is similar to readelf -S -h
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
	print_section_header(bytes, argv[1],
			 bytes[4] == ELFCLASS32 ? ELFCLASS32 : ELFCLASS64,
			 bytes[5] == ELFDATA2MSB ? ELFDATA2MSB : ELFDATA2LSB);
	return (0);
}
