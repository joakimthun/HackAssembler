#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "symbol_table.h"

typedef struct {
	FILE* file;
	int line_number;
} source_file;

source_file* create_source_file(const char* path);
void destroy_source_file(source_file* file);

void parse_labels(source_file* file, symbol_table* table);
void parse(source_file* file, symbol_table* table);

int get_source_line(char* line_buffer, source_file* file);

int parse_variable(char* line, int index, symbol_table* table, source_file* file);
int parse_label(char* line, int index, symbol_table* table, source_file* file);

int is_comment(char* line, int index);
int is_blank(char* line, int index);
void peek(char* buffer, int count, int index, char* line);
void read_char(char* line, int* index, char* out);

#endif
