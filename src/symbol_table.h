#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string.h>
#include <stdlib.h>

#define TABLE_LENGTH 1000

typedef struct {
	char key[100];
	unsigned int value;
} table_entry;

typedef struct {
	int current_index;
	table_entry entries[TABLE_LENGTH];
	unsigned int next_ram_address;
} symbol_table;

symbol_table* create_table();
void destroy_table(symbol_table* table);

void add_entry(symbol_table* table, table_entry entry);
int contains_entry(symbol_table* table, const char* key);
int adress_of(symbol_table* table, const char* key);

#endif
