#include "parser.h"

int main(int argc, char* argv[])
{
	source_file* file = create_source_file("Add.asm");

	parse(file);

	destroy_source_file(file);

	return 0;
}