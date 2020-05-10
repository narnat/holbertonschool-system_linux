#include "objdump.h"

/**
 * print_hex - Print hex of sections
 * @bytes: nth section
 * @printed: Number of printed bytes
 * @size: number of total bytes of section
 */
void print_hex(uint8_t *bytes, uint64_t printed, uint64_t size)
{
	size_t i;

	for (i = printed; i < printed + ALIGNMENT; ++i)
	{
		if (i % 4 == 0)
			printf(" ");
		if (i >= size)
			printf("  ");
		else
			printf("%02x", *(bytes + i));
	}
}

/**
 * print_ascii - Print ascii of sections
 * @bytes: nth section
 * @printed: Number of printed bytes
 * @size: number of total bytes of section
 */
void print_ascii(uint8_t *bytes, uint64_t printed, uint64_t size)
{
	size_t i;
	uint8_t c;

	printf("  ");
	for (i = printed; i < printed + ALIGNMENT; ++i)
	{
		if (i >= size)
			printf(" ");
		else
		{
			c = *(bytes + i);
			if (c < 32 || c > 126)
				printf(".");
			else
				printf("%c", c);
		}
	}
}

/**
 * print_bytes - Print section bytes, objdump -s
 * @elf: elf headers
 * @shdr: nth section header
 */
void print_bytes(elf_t *elf, void *shdr)
{
	uint64_t size, offset, addr, aligned_size, i;

	size = GET_SHDR(elf->cls, shdr, sh_size);
	offset = GET_SHDR(elf->cls, shdr, sh_offset);
	addr = GET_SHDR(elf->cls, shdr, sh_addr);
	CONVERT(elf->data, size, elf->cls, 4, 8);
	CONVERT(elf->data, offset, elf->cls, 4, 8);
	CONVERT(elf->data, addr, elf->cls, 4, 8);
	if (size % ALIGNMENT)
		aligned_size = size + ALIGNMENT - size % ALIGNMENT;
	else
		aligned_size = size;

	for (i = 0; i < aligned_size; i += ALIGNMENT)
	{
		printf(" %04lx", addr + i);
		print_hex((uint8_t *)(elf->ehdr) + offset, i, size);
		print_ascii((uint8_t *)(elf->ehdr) + offset, i, size);
		printf("\n");
	}
}

/**
 * print_sections - Print section bytes, like objdump -s
 * @elf: elf headers
 * @str_tab: section string table
 */
void print_sections(elf_t *elf, uint8_t *str_tab)
{
	uint16_t size = elf->shdr.entsize, num = elf->shdr.num;
	void *addr = elf->shdr.addr;
	size_t i;
	uint32_t name = 0;

	for (i = 0; i < num; ++i)
	{
		if (!skip_section(elf, (uint8_t *)(addr) + i * size))
		{
			name = GET_SHDR(elf->cls, (uint8_t *)(addr) + i * size, sh_name);
			CONVERT(elf->data, name, elf->cls, 4, 4);
			printf("Contents of section %s:\n", str_tab + name);
			print_bytes(elf, (uint8_t *)(addr) + i * size);
		}
	}
}

/**
 * objdump - objdump -sf like printing
 * @elf: elf sections
 */
void objdump(elf_t *elf)
{
	uint8_t *sh_strtab;
	uint16_t indx, sh_size = elf->shdr.entsize;
	Elf64_Off str_off;

	indx = GET_EHDR(elf->cls, elf->ehdr, e_shstrndx);
	CONVERT(elf->data, indx, elf->cls, 2, 2);
	str_off = GET_SHDR(elf->cls, ((uint8_t *)(elf->shdr.addr) + sh_size * indx),
					   sh_offset);
	CONVERT(elf->data, str_off, elf->cls, 4, 8);
	sh_strtab = (uint8_t *)(elf->ehdr) + str_off;
	print_file_headers(elf);
	print_sections(elf, sh_strtab);
}
