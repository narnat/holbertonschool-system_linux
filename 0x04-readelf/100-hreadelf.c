#include "_elf.h"

int get_sh_idx_by_type(unsigned char *sh_headers, int class, int endianess, uint32_t sh_type, short n_sh, short sh_size)
{
	int i;
	uint32_t type;

	for (i = 0; i < n_sh; ++i, sh_headers += sh_size)
	{
		type = class == ELFCLASS32 ?
			((Elf32_Shdr *) sh_headers)->sh_type :
			((Elf64_Shdr *) sh_headers)->sh_type ;

		if (endianess == ELFDATA2MSB)
			reverse((unsigned char *) &type, 4);

		if (type == sh_type)
			return (i);
	}

	return (-1);
}

/**
 * print_section_es - print section entry size
 * @data: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
uint64_t get_section_es(unsigned char *data, int class, int endianess)
{
	uint64_t entsize = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_entsize :
		((Elf64_Shdr *) data)->sh_entsize;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &entsize, class == ELFCLASS32 ? 4 : 8);
	return (entsize);
}

void print_sym_value(unsigned char *sym_table, int class, int endianess)
{
	Elf64_Addr value = class == ELFCLASS32 ?
		((Elf32_Sym *) sym_table)->st_value :
		((Elf64_Sym *) sym_table)->st_value;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &value, class == ELFCLASS32 ? 4 : 8);

	printf(" %0*lx", class == ELFCLASS32 ? 8 : 16, value);
}
void print_sym_size(unsigned char *sym_table, int class, int endianess)
{
	uint64_t size = class == ELFCLASS32 ?
		((Elf32_Sym *) sym_table)->st_size :
		((Elf64_Sym *) sym_table)->st_size;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &size, class == ELFCLASS32 ? 4 : 8);

	printf(" %5lu", size);
}

void print_sym_type(unsigned char *sym_table, int class)
{

	unsigned char info = class == ELFCLASS32 ?
		((Elf32_Sym *) sym_table)->st_info :
		((Elf64_Sym *) sym_table)->st_info;
	int val = class == ELFCLASS32 ?
		ELF32_ST_TYPE(info) :
		ELF64_ST_TYPE(info);
	char *str;

	switch (val)
	{
	case STT_OBJECT:
		str = "OBJECT";
		break;
	case STT_FUNC:
		str = "FUNC";
		break;
	case STT_SECTION:
		str = "SECTION";
		break;
	case STT_FILE:
		str = "FILE";
		break;
	case STT_NOTYPE:
	default:
		str = "NOTYPE";
		break;
	}
	printf(" %-7s", str);
}

void print_sym_bind(unsigned char *sym_table, int class)
{
	unsigned char info = class == ELFCLASS32 ?
		((Elf32_Sym *) sym_table)->st_info :
		((Elf64_Sym *) sym_table)->st_info;
	int val = class == ELFCLASS32 ?
		ELF32_ST_BIND(info) :
		ELF64_ST_BIND(info);
	char *str;

	switch (val)
	{
	case STB_LOCAL:
		str = "LOCAL";
		break;
	case STB_GLOBAL:
		str = "GLOBAL";
		break;
	case STB_WEAK:
		str = "WEAK";
		break;
	default:
		str = "UNKNOWN";
		break;
	}
	printf(" %-6s", str);
}

void print_sym_vis(unsigned char *sym_table, int class)
{
	unsigned char other = class == ELFCLASS32 ?
		((Elf32_Sym *) sym_table)->st_other :
		((Elf64_Sym *) sym_table)->st_other;
	int val = class == ELFCLASS32 ?
		ELF32_ST_VISIBILITY(other) :
		ELF64_ST_VISIBILITY(other);
	char *str;

	switch (val)
	{
	case STV_INTERNAL:
		str = "INTERNAL";
		break;
	case STV_HIDDEN:
		str = "HIDDEN";
		break;
	case STV_PROTECTED:
		str = "PROTECTED";
		break;
	case STV_DEFAULT:
	default:
		str = "DEFAULT";
		break;
	}
	printf(" %-8s", str);
}
void print_sym_ndx(unsigned char *sym_table, int class, int endianess)
{
	uint16_t shndx = class == ELFCLASS32 ?
		((Elf32_Sym *) sym_table)->st_shndx :
		((Elf64_Sym *) sym_table)->st_shndx;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &shndx, class == ELFCLASS32 ? 2 : 2);

	if (shndx == 0)
		printf(" UND");
	else if (shndx == 65521)
		printf(" ABS");
	else if (shndx == 65522)
		printf(" COM");
	else
		printf(" %3u", shndx);
}
void print_sym_name(unsigned char *sym_table, int class, int endianess, unsigned char *str_table)
{
	uint32_t name = class == ELFCLASS32 ?
		((Elf32_Sym *) sym_table)->st_name :
		((Elf64_Sym *) sym_table)->st_name;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &name, class == ELFCLASS32 ? 4 : 4);

	printf(" %s", str_table + name);
}

void print_dyn_version(unsigned char *ver, uint64_t n_sym, int endianess)
{
	uint16_t *v = (uint16_t *)ver;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &v[n_sym], 2);

	if (v[n_sym] > 1)
	{
		if (v[n_sym] == 2)
			printf("@GLIBC_2.2.5");
		if (v[n_sym] == 3)
			printf("@GLIBC_2.4");
		printf(" (%d)", v[n_sym]);
	}
}

void print_symbol_table_entries(unsigned char *sym_table, int class,
				int endianess, unsigned char *str_table,
				uint64_t s_size, uint64_t sh_ent_size, int type,
				unsigned char *ver)
{
	uint64_t n_sym = s_size / sh_ent_size;
	uint64_t i;

	printf("   Num:    Value%*s Type    Bind   Vis      Ndx Name\n", class == ELFCLASS32 ? 6: 14, "Size");
	for (i = 0; i < n_sym; ++i, sym_table += sh_ent_size)
	{
		printf("%6lu:", i);
		print_sym_value(sym_table, class, endianess);
		print_sym_size(sym_table, class, endianess);
		print_sym_type(sym_table, class);
		print_sym_bind(sym_table, class);
		print_sym_vis(sym_table, class);
		print_sym_ndx(sym_table, class, endianess);
		print_sym_name(sym_table, class, endianess, str_table);
		if (type == SHT_DYNSYM && ver)
		{
			print_dyn_version(ver, i, endianess);
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
void print_symbol_table(unsigned char *bytes, char *filename, int class,
			  int endianess)
{
	void *p = get_section_header_off(bytes, class, endianess);
	Elf64_Off offset = class == ELFCLASS32 ? *(Elf32_Off *) p : *(Elf64_Off *) p;
	unsigned char *sh_headers = NULL, *sym_table = NULL, *str_table = NULL, *ver = NULL, *ver_r = NULL;
	uint64_t s_off, s_size;
	uint64_t sh_ent_size;
	uint16_t n_sections = get_num_section_headers(bytes, class, endianess);
	uint16_t sh_size = get_section_hsize(bytes, class, endianess);
	int dyn, sym, version;
	if (read_bytes(&sh_headers, filename, offset, n_sections * sh_size))
	{
		exit(EXIT_FAILURE);
	}

	dyn = get_sh_idx_by_type(sh_headers, class, endianess, SHT_DYNSYM, n_sections, sh_size);
	sym = get_sh_idx_by_type(sh_headers, class, endianess, SHT_SYMTAB, n_sections, sh_size);

	if (dyn != -1)
	{
		s_off = get_section_off(sh_headers + ((dyn + 1) * sh_size), class, endianess);
		s_size = get_section_size(sh_headers + ((dyn + 1) * sh_size), class, endianess);

		if (read_bytes(&str_table, filename, s_off, s_size))
		{
			free(sh_headers);
			exit(EXIT_FAILURE);
		}

		s_off = get_section_off(sh_headers + ((dyn + 2) * sh_size), class, endianess);
		s_size = get_section_size(sh_headers + ((dyn + 2) * sh_size), class, endianess);
		version = get_sh_idx_by_type(sh_headers, class, endianess, SHT_GNU_versym, n_sections, sh_size);
		if (version != -1 && read_bytes(&ver, filename, s_off, s_size))
		{
			free(sh_headers);
			exit(EXIT_FAILURE);
		}

		s_off = get_section_off(sh_headers + ((dyn + 3) * sh_size), class, endianess);
		s_size = get_section_size(sh_headers + ((dyn + 3) * sh_size), class, endianess);

		if (read_bytes(&ver_r, filename, s_off, s_size))
		{
			free(sh_headers);
			exit(EXIT_FAILURE);
		}

		s_off = get_section_off(sh_headers + (dyn * sh_size), class, endianess);
		s_size = get_section_size(sh_headers + (dyn * sh_size), class, endianess);
		sh_ent_size = get_section_es(sh_headers + (dyn * sh_size), class, endianess);

		if (read_bytes(&sym_table, filename, s_off, s_size))
		{
			free(sh_headers);
			exit(EXIT_FAILURE);
		}

		printf("\nSymbol table '.dynsym' contains %lu entries:\n", s_size / sh_ent_size);
		print_symbol_table_entries(sym_table, class,
					   endianess,str_table, s_size, sh_ent_size, SHT_DYNSYM, ver);
		free(ver), free(ver_r);
	}

	if (sym != -1)
	{
		s_off = get_section_off(sh_headers + ((sym + 1) * sh_size), class, endianess);
		s_size = get_section_size(sh_headers + ((sym + 1) * sh_size), class, endianess);

		if (read_bytes(&str_table, filename, s_off, s_size))
		{
			free(sh_headers);
			exit(EXIT_FAILURE);
		}

		s_off = get_section_off(sh_headers + (sym * sh_size), class, endianess);
		s_size = get_section_size(sh_headers + (sym * sh_size), class, endianess);
		sh_ent_size = get_section_es(sh_headers + (sym * sh_size), class, endianess);
		if (read_bytes(&sym_table, filename, s_off, s_size))
		{
			free(sh_headers);
			exit(EXIT_FAILURE);
		}

		printf("\nSymbol table '.symtab' contains %lu entries:\n", s_size / sh_ent_size);
		print_symbol_table_entries(sym_table, class, endianess, str_table, s_size, sh_ent_size, SHT_SYMTAB, ver);
	}

/* s_off = get_section_off(sh_headers + (shstrndx * sh_size), class, endianess); */
/* s_size = get_section_size(sh_headers + (shstrndx * sh_size), class, endianess); */

/* if (read_bytes(&str_table, filename, s_off, s_size)) */
/* { */
/* 	free(sh_headers); */
/* 	exit(EXIT_FAILURE); */
/* } */

/* if (read_bytes(&pheader, filename, ph_offset, n_pheader * pheader_size)) */
/* { */
/* 	free(sh_headers); */
/* 	free(str_table); */
/* 	exit(EXIT_FAILURE); */
/* } */
/* free(sh_headers); */
/* free(str_table); */
/* free(pheader); */
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
	print_symbol_table(bytes, argv[1],
			   bytes[4] == ELFCLASS32 ? ELFCLASS32 : ELFCLASS64,
			     bytes[5] == ELFDATA2MSB ? ELFDATA2MSB : ELFDATA2LSB);
	return (0);
}
