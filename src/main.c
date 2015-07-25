#include "parser.h"

int main(int argc, char* argv[])
{
	source_file* source_file = create_source_file("Add.asm");

	parse_line(source_file);

	destroy_source_file(source_file);

	return 0;
}