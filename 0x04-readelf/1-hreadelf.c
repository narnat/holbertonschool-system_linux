#include "_elf.h"


void print_section_name(unsigned char *data, int class, int endianess, unsigned char *str_table)
{
	uint32_t name = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_name :
		((Elf64_Shdr *) data)->sh_name;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &name, class == ELFCLASS32 ? 4 : 4);

	printf("%-17s", str_table + name);
}
void print_section_type(unsigned char *data, int class, int endianess)
{
	uint32_t type = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_type :
		((Elf64_Shdr *) data)->sh_type;
	char *str;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &type, class == ELFCLASS32 ? 4 : 4);

	switch (type)
	{
	case SHT_PROGBITS:
		str = "PROGBITS";
		break;
	case SHT_SYMTAB:
		str = "SYMTAB";
		break;
	case SHT_STRTAB:
		str = "STRTAB";
		break;
	case SHT_RELA:
		str = "RELA";
		break;
	case SHT_HASH:
		str = "HASH";
		break;
	case SHT_DYNAMIC:
		str = "DYNAMIC";
		break;
	case SHT_NOTE:
		str = "NOTE";
		break;
	case SHT_NOBITS:
		str = "NOBITS";
		break;
	case SHT_REL:
		str = "REL";
		break;
	case SHT_SHLIB:
		str = "SHLIB";
		break;
	case SHT_DYNSYM:
		str = "DYNSYM";
		break;
	/* case SHT_LOPROC: */
	/* 	str = "LOPROC"; */
	/* 	break; */
	/* case SHT_HIPROC: */
	/* 	str = "HIPROC"; */
	/* 	break; */
	/* case SHT_LOUSER: */
	/* 	str = "LOUSER"; */
	/* 	break; */
	/* case SHT_HIUSER: */
	/* 	str = "HIUSER"; */
	/* 	break; */
	case SHT_INIT_ARRAY:  /*Starting here not from man page*/
		str = "INIT_ARRAY";
		break;
	case SHT_FINI_ARRAY:
		str = "FINI_ARRAY";
		break;
	case SHT_PREINIT_ARRAY:
		str = "PREINIT_ARRAY";
		break;
	case SHT_GNU_HASH:
		str = "GNU_HASH";
		break;
	case SHT_GROUP:
		str = "GROUP";
		break;
	case SHT_SYMTAB_SHNDX:
		str =  "SYMTAB SECTION INDICES";
		break;
	case SHT_GNU_verdef:
		str = "VERDEF";
		break;
	case SHT_GNU_verneed:
		str =  "VERNEED";
		break;
	case SHT_GNU_versym:
		str = "VERSYM";
		break;
	case 0x6ffffff0:
		str = "VERSYM";
		break;
	case 0x6ffffffc:
		str = "VERDEF";
		break;
	case 0x7ffffffd:
		str = "AUXILIARY";
		break;
	case 0x7fffffff:
		str = "FILTER";
		break;
	case SHT_GNU_LIBLIST:
		str = "GNU_LIBLIST";
		break;
	case SHT_GNU_ATTRIBUTES:
		str = "GNU_ATTRIBUTES";
		break;
	case SHT_X86_64_UNWIND:
		str = "GNU_X86_64_UNWIND";
		break;
	case SHT_NULL:
	default:
		str = "NULL";
		break;
	}
	printf(" %-15s", str);
}
void print_section_addr(unsigned char *data, int class, int endianess)
{
	Elf64_Addr addr = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_addr :
		((Elf64_Shdr *) data)->sh_addr;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &addr, class == ELFCLASS32 ? 4 : 8);
	printf(" %0*lx", class == ELFCLASS32 ? 8 : 16, addr);
}
void print_section_off(unsigned char *data, int class, int endianess)
{
	Elf64_Off offset = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_offset :
		((Elf64_Shdr *) data)->sh_offset;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &offset, class == ELFCLASS32 ? 4 : 8);
	printf(" %06lx", offset);
}
void print_section_size(unsigned char *data, int class, int endianess)
{
	uint64_t size = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_size :
		((Elf64_Shdr *) data)->sh_size;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &size, class == ELFCLASS32 ? 4 : 8);
	printf(" %06lx", size);
}
void print_section_es(unsigned char *data, int class, int endianess)
{
	uint64_t entsize = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_entsize :
		((Elf64_Shdr *) data)->sh_entsize;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &entsize, class == ELFCLASS32 ? 4 : 8);
	printf(" %02lx", entsize);
}
void print_section_flg(unsigned char *data, int class, int endianess)
{
	uint64_t flags = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_flags :
		((Elf64_Shdr *) data)->sh_flags;
	char buf[16] = {0}, *start = buf;

	if (endianess == ELFDATA2MSB) /* TODO: be careful */
		reverse((unsigned char *) &flags, class == ELFCLASS32 ? 4 : 8);

	if (flags & SHF_WRITE)
		*start++ = 'W';
	if (flags & SHF_ALLOC)
		*start++ = 'A';
	if (flags & SHF_EXECINSTR)
		*start++ = 'X';
	if (flags & SHF_MERGE)
		*start++ = 'M';
	if (flags & SHF_STRINGS)
		*start++ = 'S';
	if (flags & SHF_INFO_LINK)
		*start++ = 'I';
	if (flags & SHF_LINK_ORDER)
		*start++ = 'L';
	if (flags & SHF_OS_NONCONFORMING)
		*start++ = 'O';
	if (flags & SHF_GROUP)
		*start++ = 'G';
	if (flags & SHF_TLS)
		*start++ = 'T';
	if (flags & SHF_COMPRESSED)
		*start++ = 'C';
	if (flags & SHF_EXCLUDE)
		*start++ = 'E';
	printf(" %3s", buf);
}
void print_section_lk(unsigned char *data, int class, int endianess)
{
	uint32_t link = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_link :
		((Elf64_Shdr *) data)->sh_link;

	if (endianess == ELFDATA2MSB) /* TODO: be careful */
		reverse((unsigned char *) &link, class == ELFCLASS32 ? 4 : 4);
	printf(" %2u", link);
}
void print_section_inf(unsigned char *data, int class, int endianess)
{
	uint32_t info = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_info :
		((Elf64_Shdr *) data)->sh_info;

	if (endianess == ELFDATA2MSB) /* TODO: be careful */
		reverse((unsigned char *) &info, class == ELFCLASS32 ? 4 : 4);
	printf(" %3u", info);
}
void print_section_al(unsigned char *data, int class, int endianess)
{
	uint64_t addralign = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_addralign :
		((Elf64_Shdr *) data)->sh_addralign;

	if (endianess == ELFDATA2MSB) /* TODO: be careful */
		reverse((unsigned char *) &addralign, class == ELFCLASS32 ? 4 : 8);
	printf(" %2lu\n", addralign);
}

/**
 * get_section_header_off - Print section headers
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void *get_section_header_off(unsigned char *bytes, int class, int endianess)
{
	void *header_off = class == ELFCLASS32 ?
		(void *) &((Elf32_Ehdr *) bytes)->e_shoff :
		(void *) &((Elf64_Ehdr *) bytes)->e_shoff;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) header_off, class == ELFCLASS32 ? 4 : 8);

	return (header_off);
}

Elf64_Off get_section_off(unsigned char *bytes, int class, int endianess)
{
	Elf64_Off off = class == ELFCLASS32 ?
		((Elf32_Shdr *) bytes)->sh_offset :
		((Elf64_Shdr *) bytes)->sh_offset;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &off, class == ELFCLASS32 ? 4 : 8);

	return (off);
}

uint64_t get_section_size(unsigned char *bytes, int class, int endianess)
{
	uint64_t size = class == ELFCLASS32 ?
		((Elf32_Shdr *) bytes)->sh_size :
		((Elf64_Shdr *) bytes)->sh_size;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &size, class == ELFCLASS32 ? 4 : 8);

	return (size);
}


/**
 * print_section_hsize - Print section size
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
uint16_t get_section_hsize(unsigned char *bytes, int class, int endianess)
{
	uint16_t shentsize = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_shentsize :
		((Elf64_Ehdr *) bytes)->e_shentsize;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &shentsize, 2);
	return (shentsize);
}

/**
 * get_num_section_headers - Print number of section headers
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
uint16_t get_num_section_headers(unsigned char *bytes, int class, int endianess)
{
	uint16_t shnum = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_shnum :
		((Elf64_Ehdr *) bytes)->e_shnum;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &shnum, 2);
	return (shnum);
}

/**
 * read_elf_header_bytes - Reads 64 bytes from file and stores in @bytes
 * @bytes: character array
 * @filename: elf file
 * Return: 0 on success, 1 otherwise
 */
int read_bytes(unsigned char **bytes, const char *filename, size_t offset, size_t size)
{
	FILE *fp = NULL;

	*bytes = malloc(sizeof (char) * size);
	if (!bytes)
		exit(EXIT_FAILURE);
	fp = fopen(filename, "rb");
	if (!fp || fseek(fp, offset, SEEK_SET) || fread(*bytes, size, 1, fp) == 0)
	{
		return (1);
	}
	fclose(fp);
	return (0);
}

/**
 * get_string_table_idx - Print Section header string table index
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
uint16_t get_string_table_idx(unsigned char *bytes, int class, int endianess)
{
	uint16_t shstrndx = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_shstrndx :
		((Elf64_Ehdr *) bytes)->e_shstrndx;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &shstrndx, 2);

	return (shstrndx);
}


void print_key_to_flags(int class)
{
	printf("Key to Flags:\n  W (write), A (alloc), ");
	printf("X (execute), M (merge), S (strings), I (info),\n  L (link order),");
	printf(" O (extra OS processing required), G (group), T (TLS),\n");
	printf("  C (compressed), x (unknown), o (OS specific), E (exclude),\n");
	if (class == ELFCLASS64)
		printf("  l (large), p (processor specific)\n");
	else
		printf("  p (processor specific)\n");
}

void print_section_header_names(int class)
{
	printf("Section Headers:\n");
	printf("  [Nr] Name              Type            ");
	if (class == ELFCLASS32)
		printf("Addr     Off    Size   ES Flg Lk Inf Al\n");
	else
		printf("Address          Off    Size   ES Flg Lk Inf Al\n");
}

/**
 * print_section_headers - Print all infor section headers
 * @bytes: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_section_header(unsigned char *bytes, char *filename, int class, int endianess)
{
	void *p = get_section_header_off(bytes, class, endianess);
	Elf64_Off offset = class == ELFCLASS32 ? *(Elf32_Off *) p : *(Elf64_Off *) p;
	uint16_t n_sections = get_num_section_headers(bytes, class, endianess);
	uint16_t header_size = get_section_hsize(bytes, class, endianess);
	unsigned char *data = NULL, *tmp = NULL, *str_table = NULL;
	size_t i, s_off, s_size;
	uint16_t shstrndx;

	printf("There are %d section headers, starting at offset %#lx:\n\n", n_sections, offset);
	if (read_bytes(&data, filename, offset, n_sections * header_size))
	{
		printf("hello\n");
		exit(EXIT_FAILURE);
	}

	shstrndx = get_string_table_idx(bytes, class, endianess);
	s_off = get_section_off(data + (shstrndx * header_size), class, endianess);
	s_size = get_section_size(data + (shstrndx * header_size), class, endianess);

	if (read_bytes(&str_table, filename, s_off, s_size))
	{
		printf("hello 222222\n");
		free(data);
		exit(EXIT_FAILURE);
	}

	print_section_header_names(class);
	tmp = data;
	for (i = 0; i < n_sections; ++i, tmp += header_size)
	{
		printf("  [%2lu] ", i);
		print_section_name(tmp, class, endianess, str_table);
		print_section_type(tmp, class, endianess);
		print_section_addr(tmp, class, endianess);
		print_section_off(tmp, class, endianess);
		print_section_size(tmp, class, endianess);
		print_section_es(tmp, class, endianess);
		print_section_flg(tmp, class, endianess);
		print_section_lk(tmp, class, endianess);
		print_section_inf(tmp, class, endianess);
		print_section_al(tmp, class, endianess);
	}

	print_key_to_flags(class);
		  free(data);
		  free(str_table);
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
		fprintf(stderr, "readelf: Warning: Nothing to do.\n");
		fprintf(stderr, "Usage: readelf <option(s)> elf-file(s)\n");
		return (EXIT_FAILURE);
	}
	if (access(argv[1], F_OK) == -1)
	{
		fprintf(stderr, "readelf: Error: '%s': No such file\n", argv[1]);
		return (EXIT_FAILURE);
	}
	if (access(argv[1], R_OK) == -1)
	{
		fprintf(stderr,
			"readelf: Error: Input file '%s' is not readable.\n", argv[1]);
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
