#include "symbol_table.h"

symbol_table* create_table()
{
	symbol_table* table = malloc(sizeof(symbol_table));

	table->current_index = 0;

	// Predefined symbols
	add_entry(table, (table_entry){ .key = "SP", .value = 0 });
	add_entry(table, (table_entry){ .key = "LCL", .value = 1 });
	add_entry(table, (table_entry){ .key = "ARG", .value = 2 });
	add_entry(table, (table_entry){ .key = "THIS", .value = 3 });
	add_entry(table, (table_entry){ .key = "THAT", .value = 4 });
	add_entry(table, (table_entry){ .key = "R0", .value = 0 });
	add_entry(table, (table_entry){ .key = "R1", .value = 1 });
	add_entry(table, (table_entry){ .key = "R2", .value = 2 });
	add_entry(table, (table_entry){ .key = "R3", .value = 3 });
	add_entry(table, (table_entry){ .key = "R4", .value = 4 });
	add_entry(table, (table_entry){ .key = "R5", .value = 5 });
	add_entry(table, (table_entry){ .key = "R6", .value = 6 });
	add_entry(table, (table_entry){ .key = "R7", .value = 7 });
	add_entry(table, (table_entry){ .key = "R8", .value = 8 });
	add_entry(table, (table_entry){ .key = "R9", .value = 9 });
	add_entry(table, (table_entry){ .key = "R10", .value = 10 });
	add_entry(table, (table_entry){ .key = "R11", .value = 11 });
	add_entry(table, (table_entry){ .key = "R12", .value = 13 });
	add_entry(table, (table_entry){ .key = "R13", .value = 13 });
	add_entry(table, (table_entry){ .key = "R14", .value = 14 });
	add_entry(table, (table_entry){ .key = "R15", .value = 15 });
	add_entry(table, (table_entry){ .key = "SCREEN", .value = 0x4000 });
	add_entry(table, (table_entry){ .key = "KBD", .value = 0x6000 });

	table->next_ram_address = 16;

	return table;
}

void destroy_table(symbol_table* table)
{
	free(table);
}

void add_entry(symbol_table* table, table_entry entry)
{
	table->entries[table->current_index] = entry;
	table->current_index++;
}

int contains_entry(symbol_table* table, const char* key)
{
	for (int i = 0; i < table->current_index; ++i)
	{
		table_entry current_entry = table->entries[i];
		if (strcmp(key, current_entry.key) == 0)
		{
			return 1;
		}
	}

	return 0;
}

int adress_of(symbol_table* table, const char* key)
{
	for (int i = 0; i < table->current_index; ++i)
	{
		table_entry current_entry = table->entries[i];
		if (strcmp(key, current_entry.key) == 0)
		{
			return current_entry.value;
		}
	}

	return NULL;
}