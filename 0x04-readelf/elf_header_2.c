#include "_elf.h"

/**
 * print_abi_version - Print abi version
 * @bytes: character array
 */
void print_abi_version(unsigned char *bytes)
{
	unsigned char abi_version = ((Elf64_Ehdr *) bytes)->e_ident[EI_ABIVERSION];

	printf("  ABI Version:                       %d\n", abi_version);
}

/**
 * print_type - Print elf type
 * @bytes: character array
 * @endianess: LSB or MSB
 */
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

/**
 * print_machine_3 - Print machine architecture
 * @machine: machine architecture
 */
void print_machine_3(uint16_t machine)
{
	switch (machine)
	{
	case EM_X86_64:
		puts("Advanced Micro Devices X86-64");
		break;
	case EM_VAX:
		puts("Stand-alone (embedded)");
		break;
	case EM_FAKE_ALPHA:
		puts("Alpha");
		break;
	case EM_AARCH64:
		puts("AArch64");
		break;
	default:
		printf("%d\n", machine);
		break;
	}
}

/**
 * print_machine_2 - Print machine architecture
 * @machine: machine architecture
 */
void print_machine_2(uint16_t machine)
{
	switch (machine)
	{
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
	default:
		print_machine_3(machine);
		break;
	}
}

/**
 * print_machine - Print machine architecture
 * @bytes: character array
 * @endianess: LSB or MSB
 */
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
	default:
		print_machine_2(machine);
	}
}
