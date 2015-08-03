#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "symbol_table.h"
#include "codegen.h"

typedef struct {
	FILE* file;
	int line_number;
} source_file;

source_file* create_source_file(const char* path);
void destroy_source_file(source_file* file);
void parse_labels(source_file* in_file, symbol_table* table);
void parse(source_file* in_file, symbol_table* table, output_file* out_file);
void rewind_file(source_file* file);
output_file* create_output_file();
void destroy_output_file(output_file* file);
void read_char(char* line, int* index, char* out);

#endif

