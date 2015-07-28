#include "parser.h"


int main(int argc, char* argv[])
{
	symbol_table* table = create_table();
	source_file* in_file = create_source_file("Max.asm");

	parse_labels(in_file, table);
	rewind_file(in_file);

	output_file* out_file = create_output_file("");
	parse(in_file, table, out_file);

	destroy_output_file(out_file);
	destroy_source_file(in_file);
	destroy_table(table);

	return 0;
}
