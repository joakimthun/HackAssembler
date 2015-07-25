#include "parser.h"

#define LINE_LENGTH 200

source_file* create_source_file(const char* path)
{
	source_file* source_file = malloc(sizeof(source_file));
	source_file->file = fopen(path, "r");

	if (source_file->file) {
		return source_file;
	}

	return NULL;
}

void destroy_source_file(source_file* source_file)
{
	if (source_file->file)
		fclose(source_file->file);

	free(source_file);
}

void parse_line(source_file* source_file)
{
	int c;
	while ((c = getc(source_file->file)) != EOF)
		putchar(c);
}

char* get_source_line(source_file* source_file)
{
	char line_buffer[LINE_LENGTH];
	if (fgets(line_buffer, LINE_LENGTH, source_file->file) == NULL)
		return EOF;


}

void peek(char* buffer, int count, int index, char* line)
{
	for (int i = 0; i < count; ++i)
	{
		int offset = index + i;
		buffer[i] = *(line + offset);
	}
}

int is_comment(char* line, int index)
{
	char peek_buffer[2];
	peek(peek_buffer, 2, index, line);

	if (peek_buffer[0] == '/' && peek_buffer[1] == '/')
		return 1;

	return 0;
}

int is_blank(char* line, int index)
{
	char peek_buffer[1];
	peek(peek_buffer, 1, index, line);

	if (peek_buffer[0] == '\n')
		return 1;

	return 0;
}