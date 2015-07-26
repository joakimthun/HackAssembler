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

void parse_labels(source_file* file, symbol_table* table)
{
	char line_buffer[LINE_LENGTH];
	while (get_source_line(line_buffer, file) != EOF)
	{
		int i = 0;
		if (is_comment(line_buffer, i))
			continue;

		char c = 0;
		read_char(line_buffer, &i, &c);
		while (c != '\n')
		{
			if (isblank(c))
			{
				read_char(line_buffer, &i, &c);
				continue;
			}

			if (is_comment(line_buffer, i))
				break;

			if (c == '(')
			{
				parse_label(line_buffer, i, table, file);
				break;
			}

			file->line_number++;
			break;
		}
	}
}

void parse(source_file* file, symbol_table* table)
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

int parse_variable(char* line, int index, symbol_table* table, source_file* file)
{
	int start = index;
	char c;
	read_char(line, &start, &c);
	char buffer[100];
	int j = 0;
	while (!isspace(c))
	{
		buffer[j] = c;
		j++;
		read_char(line, &start, &c);
	}

	buffer[j] = '\0';

	if (!contains_entry(table, buffer))
	{
		table_entry entry;
		entry.value = file->line_number + 1;
		memcpy(entry.key, buffer, strlen(buffer) + 1);
		add_entry(table, entry);
	}
}

int parse_label(char* line, int index, symbol_table* table, source_file* file)
{
	int start = index;
	char c;
	read_char(line, &start, &c);
	char buffer[100];
	int j = 0;
	while (c != ')')
	{
		buffer[j] = c;
		j++;
		read_char(line, &start, &c);
	}

	buffer[j] = '\0';

	if (!contains_entry(table, buffer))
	{
		table_entry entry;
		entry.value = file->line_number + 1;
		memcpy(entry.key, buffer, strlen(buffer) + 1);
		add_entry(table, entry);
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

void peek(char* buffer, int count, int index, char* line)
{
	for (int i = 0; i < count; ++i)
	{
		int offset = index + i;
		buffer[i] = *(line + offset);
	}
}

void read_char(char* line, int* index, char* out)
{
	*out = *(line + *index);
	(*index)++;
};