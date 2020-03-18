#include "_elf.h"
#include <elf.h>

void reverse(unsigned char *bytes, size_t size)
{
	size_t start = 0, end = size - 1;

	while (start < end)
	{
		int temp = bytes[start];
		bytes[start] = bytes[end];
		bytes[end] = temp;
		start++;
		end--;
	}
}

int read_header_bytes(unsigned char *bytes, const char *filename)
{
	FILE *fp = NULL;

	fp = fopen(filename, "rb");
	if(!fp || fread(bytes, 64, 1,fp) == 0)
	{
		return (1);
	}

	fclose(fp);
	return (0);
}

int check_elf(unsigned char *bytes)
{
	Elf64_Ehdr *header64;

	header64 = (Elf64_Ehdr *) bytes;

	if (memcmp(header64->e_ident, ELFMAG, SELFMAG) != 0)
	{
		fprintf(stderr, "readelf: Error: Not an ELF file - it has"
			" the wrong magic bytes at the start");
		return (1);
	}
	return (0);
}

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

void print_class(unsigned char *bytes)
{
	unsigned char class_name = ((Elf64_Ehdr *) bytes)->e_ident[EI_CLASS];

	printf("  Class:                             ");
	switch (class_name)
	{
	case (ELFCLASSNONE):
		puts("Invalid Class");
		break;
	case (ELFCLASS32):
		puts("ELF32");
		break;
	case (ELFCLASS64):
		puts("ELF64");
		break;
	}
}

void print_data(unsigned char *bytes)
{
	unsigned char data = ((Elf64_Ehdr *) bytes)->e_ident[EI_DATA];

	printf("  Data:                              ");
	switch (data)
	{
	case (ELFDATANONE):
		puts("Unknown data format");
		break;
	case (ELFDATA2LSB):
		puts("2's complement, little endian");
		break;
	case (ELFDATA2MSB):
		puts("2's complement, big endian");
		break;
	}
}

void print_version(unsigned char *bytes)
{
	unsigned char version = ((Elf64_Ehdr *) bytes)->e_ident[EI_VERSION];

	printf("  Version:                           %d", version);
	switch (version)
	{
	case (EV_NONE):
		puts("");
		break;
	case (EV_CURRENT):
		puts(" (current)");
		break;
	}
}

void print_osabi(unsigned char *bytes)
{
	unsigned char osabi = ((Elf64_Ehdr *) bytes)->e_ident[EI_OSABI];

	printf("  OS/ABI:                            ");
	switch (osabi)
	{
	case ELFOSABI_SYSV: /* Also ELFOSABI_NONE */
		puts("UNIX - System V");
		break;
	case ELFOSABI_HPUX:
		puts("UNIX - HP-UX");
		break;
	case ELFOSABI_NETBSD:
		puts("UNIX - NetBSD");
		break;
	case ELFOSABI_LINUX:
		puts("UNIX - GNU");
		break;
	case ELFOSABI_SOLARIS:
		puts("UNIX - Solaris");
		break;
	case ELFOSABI_IRIX:
		puts("UNIX - IRIX");
		break;
	case ELFOSABI_FREEBSD:
		puts("UNIX - FreeBSD");
		break;
	case ELFOSABI_TRU64:
		puts("UNIX - TRU64");
		break;
	case ELFOSABI_MODESTO:
		puts("Novell Modesto");
		break;
	case ELFOSABI_OPENBSD:
		puts("OpenBSD");
		break;
	case ELFOSABI_ARM_AEABI:
		puts("ARM EABI");
		break;
	case ELFOSABI_ARM:
		puts("ARM");
		break;
	case ELFOSABI_STANDALONE:
		puts("Stand-alone (embedded)");
		break;
	}
}

void print_abi_version(unsigned char *bytes)
{
	unsigned char abi_version = ((Elf64_Ehdr *) bytes)->e_ident[EI_ABIVERSION];

	printf("  ABI Version:                       %d\n", abi_version);
}

void print_type(unsigned char *bytes, int endianess)
{
	uint16_t type = ((Elf64_Ehdr *) bytes)->e_type;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &type, 2);
	printf("  Type:                              ");
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

void print_machine(unsigned char *bytes, int endianess)
{
	uint16_t machine = ((Elf64_Ehdr *) bytes)->e_machine;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &machine, 2);
	printf("  Machine:                           ");
	switch (machine)
	{
	case EM_NONE:
		puts("An unknown machine");
		break;
	case EM_M32:
		puts("AT&T WE 32100");
		break;
	case EM_SPARC:
		puts("Sparc");
		break;
	case EM_386:
		puts("Intel 80386");
		break;
	case EM_68K:
		puts("Motorola 68000");
		break;
	case EM_88K:
		puts("Motorola 88000");
		break;
	case EM_860:
		puts("Intel 80860");
		break;
	case EM_MIPS:
		puts("MIPS R3000");
		break;
	case EM_PARISC:
		puts("HPPA");
		break;
	case EM_SPARC32PLUS:
		puts("Sparc v8+");
		break;
	case EM_PPC:
		puts("PowerPC");
		break;
	case EM_PPC64:
		puts("PowerPC 64-bit");
		break;
	case EM_S390:
		puts("IBM S/390");
		break;
	case EM_ARM:
		puts("ARM");
		break;
	case EM_SH:
		puts("Renesas / SuperH SH");
		break;
	case EM_SPARCV9:
		puts("SPARC v9 64-bit");
		break;
	case EM_IA_64:
		puts("Intel IA-64");
		break;
	case EM_X86_64:
		puts("Advanced Micro Devices X86-64");
		break;
	case EM_VAX:
		puts("Stand-alone (embedded)");
		break;
	default:
		puts("Not implemented");
		break;
	}
}

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
	}

}

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

void print_flags(unsigned char *bytes, int class, int endianess)
{
	uint32_t flags = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_flags :
		((Elf64_Ehdr *) bytes)->e_flags;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &flags, 4);
	printf("  Flags:                             0x%x\n", flags);
}

void print_header_size(unsigned char *bytes, int class, int endianess)
{
	uint16_t ehsize = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_ehsize :
		((Elf64_Ehdr *) bytes)->e_ehsize;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &ehsize, 2);
	printf("  Size of this header:               %d (bytes)\n", ehsize);
}

void print_program_header_size(unsigned char *bytes, int class, int endianess)
{
	uint16_t phentsize = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_phentsize :
		((Elf64_Ehdr *) bytes)->e_phentsize;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &phentsize, 2);
	printf("  Size of program headers:           %d (bytes)\n", phentsize);
}

void print_num_program_headers(unsigned char *bytes, int class, int endianess)
{
	uint16_t phnum = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_phnum :
		((Elf64_Ehdr *) bytes)->e_phnum;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &phnum, 2);
	printf("  Number of program headers:         %d\n", phnum);
}

void print_section_hsize(unsigned char *bytes, int class, int endianess)
{
	uint16_t shentsize = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_shentsize :
		((Elf64_Ehdr *) bytes)->e_shentsize;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &shentsize, 2);
	printf("  Size of section headers:           %d (bytes)\n", shentsize);
}

void print_num_section_headers(unsigned char *bytes, int class, int endianess)
{
	uint16_t shnum = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_shnum :
		((Elf64_Ehdr *) bytes)->e_shnum;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &shnum, 2);
	printf("  Number of section headers:         %d\n", shnum);
}

void print_string_table_idx(unsigned char *bytes, int class, int endianess)
{
	uint16_t shstrndx = class == ELFCLASS32 ?
		((Elf32_Ehdr *) bytes)->e_shstrndx :
		((Elf64_Ehdr *) bytes)->e_shstrndx;

	if (endianess == ELFDATA2MSB)
		reverse((unsigned char *) &shstrndx, 2);

	printf("  Section header string table index: %d\n", shstrndx);
}

void print_header(unsigned char *bytes, int class, int endianess)
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

int main(int argc, char *argv[])
{
	unsigned char bytes[64];

	if (argc != 2 || read_header_bytes(bytes, argv[1]))
	{
		fprintf(stderr, "Usage: readelf <option(s)> elf-file(s)\n");
		return (EXIT_FAILURE);
	}

	if (check_elf(bytes))
	{
		return (EXIT_FAILURE);
	}
	print_header(bytes,
		     bytes[4] == ELFCLASS32 ? ELFCLASS32 : ELFCLASS64,
		     bytes[5] == ELFDATA2MSB ? ELFDATA2MSB : ELFDATA2LSB);
	return 0;
}
