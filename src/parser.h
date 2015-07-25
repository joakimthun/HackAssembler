#include <stdio.h>

typedef struct {
	FILE* file;
	int line_number;
} source_file;

source_file* create_source_file(const char* path);
void destroy_source_file(source_file* file);

void parse_line(source_file* file);
