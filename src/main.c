#include "parser.h"


int main(int argc, char* argv[])
{
	symbol_table* table = create_table();
	source_file* file = create_source_file("Max.asm");

	parse_labels(file, table);
	//parse(file);

	destroy_source_file(file);
	destroy_table(table);

	return 0;
}
