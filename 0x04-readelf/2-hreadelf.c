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
 * print_type - Print elf type
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
 * print_entry_point_addr - Print entry point address
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
 * print_program_headers - Print program headers
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
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
 * print_program_header_size - Print program_header_size
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
uint16_t get_program_header_size(unsigned char *bytes, int class, int endianess)
{
	uint16_t phentsize = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_phentsize :
		((Elf64_Ehdr *) bytes)->e_phentsize;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &phentsize, 2);
	return (phentsize);
}

/**
 * print_num_program_headers - Print num_program_headers
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
uint16_t get_num_program_headers(unsigned char *bytes, int class, int endianess)
{
	uint16_t phnum = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_phnum :
		((Elf64_Ehdr *) bytes)->e_phnum;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &phnum, 2);
	return (phnum);
}

void print_program_header_string(int class)
{
	printf("Program Headers:\n");
	printf("  Type           Offset   VirtAddr");
	if (class == ELFCLASS32)
		printf("   PhysAddr   FileSiz MemSiz  Flg Align\n");
	else
		printf("           PhysAddr           FileSiz  MemSiz   Flg Align\n");
}

int print_pheader_type(unsigned char *pheader, int class, int endianess)
{
	uint32_t type = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_type :
		((Elf64_Phdr *) pheader)->p_type;
	char *str;
	int is_interp = 0;
	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &type, 4);

	switch (type)
	{
	case PT_LOAD:
		str = "LOAD";
		break;
	case PT_DYNAMIC:
		str = "DYNAMIC";
		break;
	case PT_INTERP:
		str = "INTERP";
		is_interp = 1;
		break;
	case PT_NOTE:
		str = "NOTE";
		break;
	case PT_SHLIB:
		str = "SHLIB";
		break;
	case PT_PHDR:
		str = "PHDR";
		break;
	case PT_GNU_STACK:
		str = "GNU_STACK";
		break;
	case PT_GNU_EH_FRAME:
		str = "GNU_EH_FRAME";
		break;
	case PT_GNU_RELRO:
		str = "GNU_RELRO";
		break;
	case 0x6464e550:
		str = "LOOS+464e550";
		break;
	case PT_NULL:
	default:
		str = "NULL";
	}
	printf("  %-14s", str);
	return (is_interp);
}

void print_pheader_offset(unsigned char *pheader, int class, int endianess)
{
	Elf64_Off offset = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_offset :
		((Elf64_Phdr *) pheader)->p_offset;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &offset, class == ELFCLASS32 ? 4 : 8);

	printf(" 0x%06lx", offset);
}

Elf64_Off get_pheader_offset(unsigned char *pheader, int class, int endianess)
{
	Elf64_Off offset = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_offset :
		((Elf64_Phdr *) pheader)->p_offset;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &offset, class == ELFCLASS32 ? 4 : 8);

	return (offset);
}

void print_pheader_vaddr(unsigned char *pheader, int class, int endianess)
{
	Elf64_Off vaddr = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_vaddr :
		((Elf64_Phdr *) pheader)->p_vaddr;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &vaddr, class == ELFCLASS32 ? 4 : 8);

	printf(" 0x%0*lx", class == ELFCLASS32 ? 8 : 16, vaddr);
}

Elf64_Off get_pheader_vaddr(unsigned char *pheader, int class, int endianess)
{
	Elf64_Off vaddr = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_vaddr :
		((Elf64_Phdr *) pheader)->p_vaddr;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &vaddr, class == ELFCLASS32 ? 4 : 8);

	return (vaddr);
}

void print_pheader_paddr(unsigned char *pheader, int class, int endianess)
{
	Elf64_Off paddr = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_paddr :
		((Elf64_Phdr *) pheader)->p_paddr;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &paddr, class == ELFCLASS32 ? 4 : 8);

	printf(" 0x%0*lx", class == ELFCLASS32 ? 8 : 16, paddr);
}

void print_pheader_fsize(unsigned char *pheader, int class, int endianess)
{
	uint64_t filesz = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_filesz :
		((Elf64_Phdr *) pheader)->p_filesz;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &filesz, class == ELFCLASS32 ? 4 : 8);

	printf(" 0x%0*lx", class == ELFCLASS32 ? 5 : 6, filesz);
}

uint64_t get_pheader_fsize(unsigned char *pheader, int class, int endianess)
{
	uint64_t filesz = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_filesz :
		((Elf64_Phdr *) pheader)->p_filesz;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &filesz, class == ELFCLASS32 ? 4 : 8);

	return (filesz);
}

uint64_t get_pheader_memsz(unsigned char *pheader, int class, int endianess)
{
	uint64_t memsz = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_memsz :
		((Elf64_Phdr *) pheader)->p_memsz;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &memsz, class == ELFCLASS32 ? 4 : 8);

	return (memsz);
}

void print_pheader_msize(unsigned char *pheader, int class, int endianess)
{
	uint64_t memsz = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_memsz :
		((Elf64_Phdr *) pheader)->p_memsz;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &memsz, class == ELFCLASS32 ? 4 : 8);

	printf(" 0x%0*lx", class == ELFCLASS32 ? 5 : 6, memsz);
}

void print_pheader_flg(unsigned char *pheader, int class, int endianess)
{
	uint32_t flags = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_flags :
		((Elf64_Phdr *) pheader)->p_flags;
	char str[4] = {0};

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &flags, class == ELFCLASS32 ? 4 : 8);

	str[0] = flags & PF_R ? 'R' : ' ';
	str[1] = flags & PF_W ? 'W' : ' ';
	str[2] = flags & PF_X ? 'E' : ' ';

	printf(" %s", str);
}

void print_pheader_align(unsigned char *pheader, int class, int endianess)
{
	uint64_t align = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_align :
		((Elf64_Phdr *) pheader)->p_align;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &align, class == ELFCLASS32 ? 4 : 8);

	printf(" %#lx\n", align);
}

Elf64_Off get_program_offset(unsigned char *pheader, int class, int endianess)
{
	Elf64_Off offset = class == ELFCLASS32 ?
		((Elf32_Phdr *) pheader)->p_offset :
		((Elf64_Phdr *) pheader)->p_offset;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &offset, class == ELFCLASS32 ? 4 : 8);

	return (offset);
}

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
 * print_section_name - print section name
 * @data: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 * @str_table: string table
 */
void get_section_name(unsigned char *data, int class, int endianess,
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
 * get_section_addr - print section addr
 * @data: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
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
		/* printf("***************Start section***************\n"); */
		for (j = 0; j < sh_n; ++j, tmp += sh_size)
		{
			sh_off = get_section_addr(tmp, class, endianess);
			sh_siz = get_sh_size(tmp, class, endianess);
			/* printf("\nshadr: %lx, off: %lx, filesz: %lx  ", sh_addr, offset, offset + filesz); */
			if (sh_siz && sh_off >= offset && sh_off < filesz + offset)
			{
				get_section_name(tmp, class, endianess, strtab);
			}
			if (sh_off > filesz + offset)
			{
				/* printf(" "); */
				break;
			}
		}
		/* printf("***************END_SECTION***************\n"); */
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
	check_type(bytes, bytes[5] == ELFDATA2MSB ? ELFDATA2MSB : ELFDATA2LSB);
	print_program_header(bytes, argv[1],
			 bytes[4] == ELFCLASS32 ? ELFCLASS32 : ELFCLASS64,
			 bytes[5] == ELFDATA2MSB ? ELFDATA2MSB : ELFDATA2LSB);
	return (0);
}
