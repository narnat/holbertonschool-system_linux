#include "_elf.h"

/**
 * print_class - 32bit or 64bit
 * @bytes: character array
 */
void print_class(unsigned char *bytes)
{
	unsigned char class_name = ((Elf64_Ehdr *) bytes)->e_ident[EI_CLASS];

	printf("  Class:                             ");
	switch (class_name)
	{
	case (ELFCLASS32):
		puts("ELF32");
		break;
	case (ELFCLASS64):
		puts("ELF64");
		break;
	case ELFCLASSNONE:
	default:
		puts("Invalid Class");
		break;
	}
}

/**
 * print_data - Print endianess
 * @bytes: character array
 */
void print_data(unsigned char *bytes)
{
	unsigned char data = ((Elf64_Ehdr *) bytes)->e_ident[EI_DATA];

	printf("  Data:                              ");
	switch (data)
	{
	case (ELFDATA2LSB):
		puts("2's complement, little endian");
		break;
	case (ELFDATA2MSB):
		puts("2's complement, big endian");
		break;
	case (ELFDATANONE):
	default:
		puts("Unknown data format");
		break;
	}
}

/**
 * print_version - Print version info
 * @bytes: character array
 */
void print_version(unsigned char *bytes)
{
	unsigned char version = ((Elf64_Ehdr *) bytes)->e_ident[EI_VERSION];

	printf("  Version:                           %d", version);
	switch (version)
	{
	case (EV_CURRENT):
		puts(" (current)");
		break;
	case (EV_NONE):
	default:
		puts("");
		break;
	}
}

/**
 * print_osabi_2 - Print osabi
 * @osabi: osabi
 */
void print_osabi_2(unsigned char osabi)
{
	switch (osabi)
	{
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
	default:
		printf("<unknown: %x>\n", osabi);
		break;
	}
}

/**
 * print_osabi - Print osabi
 * @bytes: character array
 */
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
	default:
		print_osabi_2(osabi);
		break;
	}
}
