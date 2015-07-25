#include "parser.h"

#define LINE_LENGTH 200

source_file* create_source_file(const char* path)
{
	source_file* file = malloc(sizeof(source_file));

	file->line_number = -1;
	file->file = fopen(path, "r");

	if (file->file) {
		return file;
	}
	else
	{
		free(file);
		return NULL;
	}
}

void destroy_source_file(source_file* file)
{
	if (file->file)
		fclose(file->file);

	free(file);
}

void parse(source_file* file)
{
	char line_buffer[LINE_LENGTH];
	while(get_source_line(line_buffer, file) != EOF)
	{
		if (is_comment(line_buffer, 0) || is_blank(line_buffer, 0))
			continue;

		file->line_number++;
	}
}

int get_source_line(char* line_buffer, source_file* file)
{
	if (fgets(line_buffer, LINE_LENGTH, file->file) == NULL)
	{
		return EOF;
	}

	return 1;
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

void peek(char* buffer, int count, int index, char* line)
{
	for (int i = 0; i < count; ++i)
	{
		int offset = index + i;
		buffer[i] = *(line + offset);
	}
}