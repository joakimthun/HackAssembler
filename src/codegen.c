#include "codegen.h"

void gen_dest(char* array_16, char* dest);
void gen_comp(char* array_16, char* comp);
void gen_jump(char* array_16, char* jump);
void to_binary15(int value, char* array_16);
void write(char* array_16, output_file* file);
char* find_entry(code_entry* array_ptr, int array_length, char* key);

code_entry dest_code[DEST_CODE_LENGTH];
code_entry comp_code[COMP_CODE_LENGTH];
code_entry jump_code[JUMP_CODE_LENGTH];

void append_a_instruction(output_file* file, int value)
{
	char binary[16];

	// A instructions has a leading 0
	binary[15] = '0';

	to_binary15(value, binary);
	write(binary, file);
}

void append_c_instruction(output_file* file, c_inst inst)
{
	// 1 1 1 a c1 c2 c3 c4 c5 c6 d1 d2 d3 j1 j2 j3

	char binary[16];

	// C instructions has 3 leading 1's
	binary[15] = '1';
	binary[14] = '1';
	binary[13] = '1';

	for (int i = 15; i >= 0; i--)
	{
		binary[i] = '1';
	}

	gen_dest(binary, inst.dest);
	gen_comp(binary, inst.comp);
	gen_jump(binary, inst.jump);

	write(binary, file);
}

void gen_dest(char* array_16, char* dest)
{
	char* value = find_entry(dest_code, DEST_CODE_LENGTH, dest);

	array_16[5] = value[0]; // d1
	array_16[4] = value[1]; // d2
	array_16[3] = value[2]; // d3
}

void gen_comp(char* array_16, char* comp)
{
	char* value = find_entry(comp_code, COMP_CODE_LENGTH, comp);

	array_16[12] = value[0]; // a
	array_16[11] = value[1]; // c1
	array_16[10] = value[2]; // c2
	array_16[9] = value[3]; // c3
	array_16[8] = value[4]; // c4
	array_16[7] = value[5]; // c5
	array_16[6] = value[6]; // c6
}

void gen_jump(char* array_16, char* jump)
{
	char* value = find_entry(jump_code, JUMP_CODE_LENGTH, jump);

	array_16[2] = value[0]; // j1
	array_16[1] = value[1]; // j2
	array_16[0] = value[2]; // j3
}

void write(char* array_16, output_file* file)
{
	for (int i = 15; i >= 0; i--)
	{
		file->file[file->index] = array_16[i];
		file->index++;
	}

	file->file[file->index] = '\n';
	file->index++;
}

void to_binary15(int value, char* array_16)
{
	// Ugly and hackish but it works 
	array_16[14] = value & 0x4000 ? '1' : '0'; // 16384
	array_16[13] = value & 0x2000 ? '1' : '0'; // 8192
	array_16[12] = value & 0x1000 ? '1' : '0'; // 4096
	array_16[11] = value & 0x800 ? '1' : '0'; // 2048
	array_16[10] = value & 0x400 ? '1' : '0'; // 1024
	array_16[9] = value & 0x200 ? '1' : '0'; // 512
	array_16[8] = value & 0x100 ? '1' : '0'; // 256
	array_16[7] = value & 0x80 ? '1' : '0'; // 128
	array_16[6] = value & 0x40 ? '1' : '0'; // 64
	array_16[5] = value & 0x20 ? '1' : '0'; // 32
	array_16[4] = value & 0x10 ? '1' : '0'; // 16
	array_16[3] = value & 0x08 ? '1' : '0'; // 8
	array_16[2] = value & 0x04 ? '1' : '0'; // 4
	array_16[1] = value & 0x02 ? '1' : '0'; // 2
	array_16[0] = value & 0x01 ? '1' : '0'; // 1
}

char* find_entry(code_entry* array_ptr, int array_length, char* key)
{
	if (key[0] == '0')
		return array_ptr[0].value;

	for (int i = 0; i <= array_length; ++i)
	{
		code_entry current_entry = array_ptr[i];
		if (strcmp(key, current_entry.key) == 0)
		{
			return current_entry.value;
		}
	}

	return NULL;
}

void init_code_arrays()
{
	dest_code[0] = (code_entry){ .key = "0", .value = "000" };
	dest_code[1] = (code_entry){ .key = "M", .value = "001" };
	dest_code[2] = (code_entry){ .key = "D", .value = "010" };
	dest_code[3] = (code_entry){ .key = "MD", .value = "011" };
	dest_code[4] = (code_entry){ .key = "A", .value = "100" };
	dest_code[5] = (code_entry){ .key = "AM", .value = "101" };
	dest_code[6] = (code_entry){ .key = "AD", .value = "110" };
	dest_code[7] = (code_entry){ .key = "AMD", .value = "111" };

	// A = 0
	comp_code[0] = (code_entry){ .key = "0", .value = "0101010" };
	comp_code[1] = (code_entry){ .key = "1", .value = "0111111" };
	comp_code[2] = (code_entry){ .key = "-1", .value = "0111010" };
	comp_code[3] = (code_entry){ .key = "D", .value = "0001100" };
	comp_code[4] = (code_entry){ .key = "A", .value = "0110000" };
	comp_code[5] = (code_entry){ .key = "!D", .value = "0001101" };
	comp_code[6] = (code_entry){ .key = "!A", .value = "0110001" };
	comp_code[7] = (code_entry){ .key = "-D", .value = "0001111" };
	comp_code[8] = (code_entry){ .key = "-A", .value = "0110011" };
	comp_code[9] = (code_entry){ .key = "D+1", .value = "0011111" };
	comp_code[10] = (code_entry){ .key = "A+1", .value = "0110111" };
	comp_code[11] = (code_entry){ .key = "D-1", .value = "0001110" };
	comp_code[12] = (code_entry){ .key = "A-1", .value = "0110010" };
	comp_code[13] = (code_entry){ .key = "D+A", .value = "0000010" };
	comp_code[14] = (code_entry){ .key = "D-A", .value = "0010011" };
	comp_code[15] = (code_entry){ .key = "A-D", .value = "0000111" };
	comp_code[16] = (code_entry){ .key = "D&A", .value = "0000000" };
	comp_code[17] = (code_entry){ .key = "D|A", .value = "0010101" };

	// A = 1
	comp_code[18] = (code_entry){ .key = "M", .value = "1110000" };
	comp_code[19] = (code_entry){ .key = "!M", .value = "1110001" };
	comp_code[20] = (code_entry){ .key = "-M", .value = "1110011" };
	comp_code[21] = (code_entry){ .key = "M+1", .value = "1110111" };
	comp_code[22] = (code_entry){ .key = "M-1", .value = "1110010" };
	comp_code[23] = (code_entry){ .key = "D+M", .value = "1000010" };
	comp_code[24] = (code_entry){ .key = "D-M", .value = "1010011" };
	comp_code[25] = (code_entry){ .key = "M-D", .value = "1000111" };
	comp_code[26] = (code_entry){ .key = "D&M", .value = "1000000" };
	comp_code[27] = (code_entry){ .key = "D|M", .value = "1010101" };

	jump_code[0] = (code_entry){ .key = "0", .value = "000" };
	jump_code[1] = (code_entry){ .key = "JGT", .value = "001" };
	jump_code[2] = (code_entry){ .key = "JEQ", .value = "010" };
	jump_code[3] = (code_entry){ .key = "JGE", .value = "011" };
	jump_code[4] = (code_entry){ .key = "JLT", .value = "100" };
	jump_code[5] = (code_entry){ .key = "JNE", .value = "101" };
	jump_code[6] = (code_entry){ .key = "JLE", .value = "110" };
	jump_code[7] = (code_entry){ .key = "JMP", .value = "111" };
}
