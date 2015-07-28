#ifndef CODEGEN_H
#define CODEGEN_H

#include <string.h>
#include <stdlib.h>

#include "symbol_table.h"

typedef struct {
	char file[10000];
	int index;
} output_file;

typedef struct {
	char dest[4];
	char comp[4];
	char jump[4];
} c_inst;

typedef struct {
	char* key;
	char value[7];
} code_entry;

void append_a_instruction(output_file* file, int value);
void append_c_instruction(output_file* file, c_inst inst);
void init_code_arrays();

#endif
