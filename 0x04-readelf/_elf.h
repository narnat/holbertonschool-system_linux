#ifndef ELF_H
#define ELF_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#include <string.h>
#include <stdlib.h>

void reverse(unsigned char *bytes, size_t size);
int read_elf_header_bytes(unsigned char *bytes, const char *filename);
int check_elf(unsigned char *bytes);

/* Elf Header */
void print_magic_bytes(unsigned char *bytes);
void print_class(unsigned char *bytes);
void print_data(unsigned char *bytes);
void print_version(unsigned char *bytes);
void print_osabi(unsigned char *bytes);
void print_abi_version(unsigned char *bytes);
void print_type(unsigned char *bytes, int endianess);
void print_machine(unsigned char *bytes, int endianess);
void print_file_version(unsigned char *bytes, int endianess);
void print_entry_point_addr(unsigned char *bytes, int class, int endianess);
void print_program_headers(unsigned char *bytes, int class, int endianess);
void print_section_headers(unsigned char *bytes, int class, int endianess);
void print_flags(unsigned char *bytes, int class, int endianess);
void print_header_size(unsigned char *bytes, int class, int endianess);
void print_program_header_size(unsigned char *bytes, int class, int endianess);
void print_num_program_headers(unsigned char *bytes, int class, int endianess);
void print_section_hsize(unsigned char *bytes, int class, int endianess);
void print_num_section_headers(unsigned char *bytes, int class, int endianess);
void print_string_table_idx(unsigned char *bytes, int class, int endianess);
/* Elf Header */

/* Elf Section */
int read_bytes(unsigned char **bytes, const char *filename,
	       size_t offset, size_t size);
uint16_t get_num_section_headers(unsigned char *bytes, int class,
				 int endianess);
void *get_section_header_off(unsigned char *bytes, int class, int endianess);
uint16_t get_string_table_idx(unsigned char *bytes, int class, int endianess);
Elf64_Off get_section_off(unsigned char *data, int class, int endianess);
uint16_t get_section_hsize(unsigned char *data, int class, int endianess);
uint64_t get_section_size(unsigned char *bytes, int class, int endianess);

void print_section_header_names(int class);
void print_section_name(unsigned char *bytes, int class, int endianess,
			unsigned char *str_table);
void print_section_type(unsigned char *bytes, int class, int endianess);
void print_section_addr(unsigned char *bytes, int class, int endianess);
void print_section_off(unsigned char *bytes, int class, int endianess);
void print_section_size(unsigned char *bytes, int class, int endianess);
void print_section_es(unsigned char *bytes, int class, int endianess);
void print_section_flg(unsigned char *bytes, int class, int endianess);
void print_section_lk(unsigned char *bytes, int class, int endianess);
void print_section_inf(unsigned char *bytes, int class, int endianess);
void print_section_al(unsigned char *bytes, int class, int endianess);
void print_key_to_flags(int class);
/* End Elf Section */

/* Elf program header*/

void print_type_2(unsigned char *bytes, int endianess);
uint16_t get_num_program_headers(unsigned char *bytes,
				 int class, int endianess);
void print_entry_point_addr_2(unsigned char *bytes, int class, int endianess);
uint16_t get_program_header_size(unsigned char *bytes,
				 int class, int endianess);
Elf64_Off get_ph_offset(unsigned char *bytes, int class, int endianess);

void print_program_header_string(int class);
int print_pheader_type(unsigned char *pheader, int class, int endianess);
void print_pheader_offset(unsigned char *pheader, int class, int endianess);
Elf64_Off get_pheader_offset(unsigned char *pheader, int class, int endianess);

void print_pheader_vaddr(unsigned char *pheader, int class, int endianess);
Elf64_Off get_pheader_vaddr(unsigned char *pheader, int class, int endianess);
void print_pheader_paddr(unsigned char *pheader, int class, int endianess);
void print_pheader_fsize(unsigned char *pheader, int class, int endianess);
uint64_t get_pheader_fsize(unsigned char *pheader, int class, int endianess);

uint64_t get_pheader_memsz(unsigned char *pheader, int class, int endianess);
void print_pheader_msize(unsigned char *pheader, int class, int endianess);
void print_pheader_flg(unsigned char *pheader, int class, int endianess);
void print_pheader_align(unsigned char *pheader, int class, int endianess);
Elf64_Off get_program_offset(unsigned char *pheader, int class, int endianess);

void print_pheader_loop(unsigned char *pheader, int class, int endianess,
			uint16_t n_pheader, uint16_t header_size, char *filename);
void print_section_name_2(unsigned char *data, int class, int endianess,
			  unsigned char *str_table);
Elf64_Addr get_section_addr(unsigned char *data, int class, int endianess);
uint64_t get_sh_size(unsigned char *data, int class, int endianess);

/* End Elf program header*/

#endif /* ELF_H */
