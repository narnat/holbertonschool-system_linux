#include "_elf.h"

/**
 * print_segment_section_map - Print section mapping
 * @pheader: Character array
 * @sheader: Section header array
 * @strtab: String table array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 * @ph_n: Number of Phdr
 * @ph_size: Size of Phdr
 * @sh_size: Size of Shdr
 * @sh_n: Number of Shdr
 */
void print_segment_section_map(unsigned char *pheader, unsigned char *sheader,
			       unsigned char *strtab, int class, int endianess,
			       int ph_n, int ph_size, int sh_size, int sh_n)
{
	int i, j;
	uint64_t offset, filesz, sh_off, sh_siz;
	unsigned char *tmp;

	printf("\n Section to Segment mapping:\n");
	printf("  Segment Sections...\n");
	for (i = 0; i < ph_n; ++i, pheader += ph_size)
	{
		printf("   %02d     ", i);
		offset = get_pheader_vaddr(pheader, class, endianess);
		filesz = get_pheader_memsz(pheader, class, endianess);
		tmp = sheader;
		for (j = 0; j < sh_n; ++j, tmp += sh_size)
		{
			sh_off = get_section_addr(tmp, class, endianess);
			sh_siz = get_sh_size(tmp, class, endianess);
			if (sh_siz && sh_off >= offset && sh_off < filesz + offset)
				print_section_name_2(tmp, class, endianess, strtab);
			if (sh_off > filesz + offset)
				break;
		}
		putchar('\n');
	}

}

/**
 * print_section_headers - Print info about section headers, like readelf -S -W
 * @bytes: character array
 * @filename: elf file
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_program_header(unsigned char *bytes, char *filename, int class,
			  int endianess)
{
	void *p = get_section_header_off(bytes, class, endianess);
	Elf64_Off ph_offset = get_ph_offset(bytes, class, endianess);
	Elf64_Off offset = class == ELFCLASS32 ? *(Elf32_Off *) p : *(Elf64_Off *) p;
	unsigned char *data = NULL, *str_table = NULL, *pheader;
	size_t s_off, s_size;
	uint16_t n_sections = get_num_section_headers(bytes, class, endianess);
	uint16_t n_pheader = get_num_program_headers(bytes, class, endianess);
	uint16_t header_size = get_section_hsize(bytes, class, endianess);
	uint16_t pheader_size = get_program_header_size(bytes, class, endianess);
	uint16_t shstrndx;

	print_type_2(bytes, endianess);
	print_entry_point_addr_2(bytes, class, endianess);
	printf("There are %d program headers, starting at offset %lu\n\n",
	       n_pheader, ph_offset);
	print_program_header_string(class);
	if (read_bytes(&data, filename, offset, n_sections * header_size))
		exit(EXIT_FAILURE);
	shstrndx = get_string_table_idx(bytes, class, endianess);
	s_off = get_section_off(data + (shstrndx * header_size), class, endianess);
	s_size = get_section_size(data + (shstrndx * header_size), class, endianess);
	if (read_bytes(&str_table, filename, s_off, s_size))
	{
		free(data);
		exit(EXIT_FAILURE);
	}
	if (read_bytes(&pheader, filename, ph_offset, n_pheader * pheader_size))
	{
		free(data);
		free(str_table);
		exit(EXIT_FAILURE);
	}
	print_pheader_loop(pheader, class, endianess, n_pheader,
			   pheader_size, filename);
	print_segment_section_map(pheader, data, str_table, class, endianess,
				  n_pheader, pheader_size, header_size, n_sections);
	free(data);
	free(str_table);
	free(pheader);
}

/**
 * check_type - Check elf type
 * @bytes: character array
 * @endianess: LSB or MSB
 */
void check_type(unsigned char *bytes, int endianess)
{
	uint16_t type = ((Elf64_Ehdr *) bytes)->e_type;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &type, 2);

	if (type != ET_EXEC && type != ET_DYN)
	{
		printf("\nThere are no program headers in this file.\n");
		exit(EXIT_SUCCESS);
	}
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
		return (EXIT_SUCCESS);
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
		return (EXIT_FAILURE);
	check_type(bytes, bytes[5] == ELFDATA2MSB ? ELFDATA2MSB : ELFDATA2LSB);
	print_program_header(bytes, argv[1],
			 bytes[4] == ELFCLASS32 ? ELFCLASS32 : ELFCLASS64,
			 bytes[5] == ELFDATA2MSB ? ELFDATA2MSB : ELFDATA2LSB);
	return (0);
}
