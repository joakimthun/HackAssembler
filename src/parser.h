#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	FILE* file;
	int line_number;
} source_file;

source_file* create_source_file(const char* path);
void destroy_source_file(source_file* file);

void parse(source_file* file);
int get_source_line(char* line_buffer, source_file* file);
int is_comment(char* line, int index);
int is_blank(char* line, int index);
void peek(char* buffer, int count, int index, char* line);

#endif
