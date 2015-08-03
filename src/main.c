#include "parser.h"

int write_to_disk(output_file* output, char* filename);
char* get_filename(char* in_filename, char* buffer);


int main(int argc, char* argv[])
{
	int success = 0;

	if (argc < 2)
	{
		printf("No input file.");
		return 1;
	}
	else
	{
		source_file* in_file = create_source_file(argv[1]);

		if (!in_file->file)
		{
			printf("Invalid input file.");
			destroy_source_file(in_file);
			return 1;
		}

		symbol_table* table = create_table();
		output_file* out_file = create_output_file();

		parse_labels(in_file, table);
		rewind_file(in_file);

		parse(in_file, table, out_file);

		char out_filename[100];
		get_filename(argv[1], out_filename);

		success = write_to_disk(out_file, out_filename);

		destroy_output_file(out_file);
		destroy_source_file(in_file);
		destroy_table(table);
	}

	return success;
}

int write_to_disk(output_file* out_file, char* filename)
{
	FILE* file = fopen(filename, "w+");
	if (!file)
		return 1;

	fprintf(file, out_file->file);
	fclose(file);

	return 0;
}

char* get_filename(char* in_filename, char* buffer)
{
	int index = 0;
	char c;
	read_char(in_filename, &index, &c);
	while (c != '.')
	{
		buffer[index - 1] = c;
		read_char(in_filename, &index, &c);
	}
	
	buffer[index - 1] = '\0';

	strcat(buffer, ".hack");

	return buffer;
}
