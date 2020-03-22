#include "_elf.h"

/* /\** */
/*  * get_num_section_headers - get number of section headers */
/*  * @bytes: character array */
/*  * @class: ELFCLASS32 or ELFCLASS64 */
/*  * @endianess: LSB or MSB */
/*  * Return: number of section headers */
/*  *\/ */
/* uint16_t get_num_section_headers(unsigned char *bytes, int class, */
/* 				 int endianess) */
/* { */
/* 	uint16_t shnum = class == ELFCLASS32 ? */
/* 		((Elf32_Ehdr *) bytes)->e_shnum : */
/* 		((Elf64_Ehdr *) bytes)->e_shnum; */

/* 	if (endianess == ELFDATA2MSB) */
/* 		reverse((unsigned char *) &shnum, 2); */
/* 	return (shnum); */
/* } */

/**
 * get_section_type_name_2 - get section type name
 * @type: type index
 * Return: section type name
 */
static char *get_section_type_name_2(uint32_t type)
{
	switch (type)
	{
	case SHT_PREINIT_ARRAY:
		return ("PREINIT_ARRAY");
	case SHT_GNU_HASH:
		return ("GNU_HASH");
	case SHT_GROUP:
		return ("GROUP");
	case SHT_SYMTAB_SHNDX:
		return ("SYMTAB SECTION INDICES");
	case SHT_GNU_verdef:
		return ("VERDEF");
	case SHT_GNU_verneed:
		return ("VERNEED");
	case SHT_GNU_versym:
		return ("VERSYM");
	case 0x6ffffff0:
		return ("VERSYM");
	case 0x6ffffffc:
		return ("VERDEF");
	case 0x7ffffffd:
		return ("AUXILIARY");
	case 0x7fffffff:
		return ("FILTER");
	case 0x6ffffff1:
		return ("LOOS+ffffff1");
	case 0x6ffffff3:
		return ("LOOS+ffffff3");
	case SHT_GNU_LIBLIST:
		return ("GNU_LIBLIST");
	case SHT_GNU_ATTRIBUTES:
		return ("GNU_ATTRIBUTES");
	case SHT_NULL:
	default:
		return ("NULL");
	}
}
/**
 * get_section_type_name - get section type name
 * @type: type index
 * Return: section type name
 */
static char *get_section_type_name(uint32_t type)
{
	switch (type)
	{
	case SHT_PROGBITS:
		return ("PROGBITS");
	case SHT_SYMTAB:
		return ("SYMTAB");
	case SHT_STRTAB:
		return ("STRTAB");
	case SHT_RELA:
		return ("RELA");
	case SHT_HASH:
		return ("HASH");
	case SHT_DYNAMIC:
		return ("DYNAMIC");
	case SHT_NOTE:
		return ("NOTE");
	case SHT_NOBITS:
		return ("NOBITS");
	case SHT_REL:
		return ("REL");
	case SHT_SHLIB:
		return ("SHLIB");
	case SHT_DYNSYM:
		return ("DYNSYM");
	case SHT_INIT_ARRAY:  /*Starting here not from man page*/
		return ("INIT_ARRAY");
	case SHT_FINI_ARRAY:
		return ("FINI_ARRAY");
	default:
		return (get_section_type_name_2(type));
	}
}

/**
 * print_section_type - print section type
 * @data: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
void print_section_type(unsigned char *data, int class, int endianess)
{
	uint32_t type = class == ELFCLASS32 ?
		((Elf32_Shdr *) data)->sh_type :
		((Elf64_Shdr *) data)->sh_type;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &type, class == ELFCLASS32 ? 4 : 4);

	printf(" %-15s", get_section_type_name(type));
}

/**
 * print_section_flg - print section flags
 * @data: character array
 * @class: ELFCLASS32 or ELFCLASS64
 * @endianess: LSB or MSB
 */
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
	/* if (flags & SHF_COMPRESSED)	Won't work with Ubuntu 14*/
	/*	*start++ = 'C'; */
	if (flags & SHF_EXCLUDE)
		*start++ = 'E';
	printf(" %3s", buf);
}
