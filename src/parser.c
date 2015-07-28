#include "parser.h"

#define LINE_LENGTH 200

int get_source_line(char* line_buffer, source_file* file);
void parse_a_instruction(char* line, int index, symbol_table* table, output_file* out_file);
void parse_c_instruction(char* line, int index, symbol_table* table, output_file* out_file);
void parse_label(char* line, int index, symbol_table* table, source_file* in_file);
int is_comment(char* line, int index);
int is_blank(char* line, int index);
void peek(char* buffer, int count, int index, char* line);
void read_char(char* line, int* index, char* out);
char next_symbol(char* line, int index);

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

void parse(source_file* in_file, symbol_table* table, output_file* out_file)
{
	init_code_arrays();

	char line_buffer[LINE_LENGTH];
	while (get_source_line(line_buffer, in_file) != EOF)
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

			if (is_comment(line_buffer, i) || c == '(')
				break;

			if (c == '@')
			{
				parse_a_instruction(line_buffer, i, table, out_file);
			}
			else
			{
				parse_c_instruction(line_buffer, i - 1, table, out_file);
			}

			break;
		}
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

void parse_a_instruction(char* line, int index, symbol_table* table, output_file* out_file)
{
	char c;
	read_char(line, &index, &c);
	char buffer[100];
	int i = 0;
	while (!isspace(c) && !is_comment(line, index))
	{
		buffer[i] = c;
		i++;
		read_char(line, &index, &c);
	}

	buffer[i] = '\0';

	if (isdigit(buffer[0]))
	{
		append_a_instruction(out_file, atoi(buffer));
		return;
	}

	if (!contains_entry(table, buffer))
	{
		table_entry entry;
		entry.value = table->next_ram_address;
		table->next_ram_address++;
		memcpy(entry.key, buffer, strlen(buffer) + 1);
		add_entry(table, entry);
	}

	int variable = adress_of(table, buffer);
	append_a_instruction(out_file, variable);
}

void parse_c_instruction(char* line, int index, symbol_table* table, output_file* out_file)
{
	c_inst inst;
	char c;
	read_char(line, &index, &c);
	char buffer[100];
	int i = 0;
	char ns = next_symbol(line, index);
	while (ns != 0 && c != ns)
	{
		buffer[i] = c;
		i++;
		read_char(line, &index, &c);
	}

	buffer[i] = '\0';

	if (ns == '=')
	{
		memcpy(inst.dest, buffer, 3);
		read_char(line, &index, &c);
	}
	else
	{
		inst.dest[0] = '0';
		read_char(line, &index, &c);
	}

	if (ns != ';')
	{

		i = 0;
		while (c != ';' && !isspace(c) && !is_comment(line, index))
		{
			buffer[i] = c;
			i++;
			read_char(line, &index, &c);
		}

		buffer[i] = '\0';
	}

	memcpy(inst.comp, buffer, 4);

	ns = next_symbol(line, index - 2);
	if (ns == ';')
	{

		//read_char(line, &index, &c);

		i = 0;
		while (!isspace(c) && !is_comment(line, index))
		{
			buffer[i] = c;
			i++;
			read_char(line, &index, &c);
		}
		
		buffer[i] = '\0';

		memcpy(inst.jump, buffer, 4);
	}
	else
	{
		inst.jump[0] = '0';
	}

	append_c_instruction(out_file, inst);
}

void parse_label(char* line, int index, symbol_table* table, source_file* file)
{
	char c;
	read_char(line, &index, &c);
	char buffer[100];
	int i = 0;
	while (c != ')')
	{
		buffer[i] = c;
		i++;
		read_char(line, &index, &c);
	}

	buffer[i] = '\0';

	if (!contains_entry(table, buffer))
	{
		table_entry entry;
		entry.value = file->line_number + 1;
		memcpy(entry.key, buffer, strlen(buffer) + 1);
		add_entry(table, entry);
	}
}

char next_symbol(char* line, int index)
{
	char peek_buffer[10];
	peek(peek_buffer, 2, index, line);

	for (int i = 0; i < 10; i++)
	{
		if (peek_buffer[i] == '=')
			return '=';
		if (peek_buffer[i] == ';')
			return ';';
	}

	return 0;
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

void rewind_file(source_file* file)
{
	rewind(file->file);
}


output_file* create_output_file(char* name)
{
	output_file* file = malloc(sizeof(output_file));
	file->index = 0;
	return file;
}

void destroy_output_file(output_file* file)
{
	free(file);
}