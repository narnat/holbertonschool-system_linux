#include "_elf.h"

/**
 * reverse - Reverse char array, used to go from MSB -> LSB
 * @bytes: character array
 * @size: size of @bytes
*/
void reverse(unsigned char *bytes, size_t size)
{
	size_t start = 0, end = size - 1;
	int temp;

	while (start < end)
	{
		temp = bytes[start];
		bytes[start] = bytes[end];
		bytes[end] = temp;
		start++;
		end--;
	}
}

/**
 * read_elf_header_bytes - Reads 64 bytes from file and stores in @bytes
 * @bytes: character array
 * @filename: elf file
 * Return: 0 on success, 1 otherwise
*/
int read_elf_header_bytes(unsigned char *bytes, const char *filename)
{
	FILE *fp = NULL;

	fp = fopen(filename, "rb");
	if (!fp || fread(bytes, 64, 1, fp) == 0)
	{
		return (1);
	}

	fclose(fp);
	return (0);
}

/**
 * check_elf - Checks if it is an elf file
 * @bytes: character array
 * Return: 0 on success, 1 otherwise
 */
int check_elf(unsigned char *bytes)
{
	Elf64_Ehdr *header64;

	header64 = (Elf64_Ehdr *) bytes;

	if (memcmp(header64->e_ident, ELFMAG, SELFMAG) != 0)
	{
		fprintf(stderr,  "readelf: Error: Not an ELF file - it has");
		fprintf(stderr, " the wrong magic bytes at the start\n");
		return (1);
	}
	return (0);
}
