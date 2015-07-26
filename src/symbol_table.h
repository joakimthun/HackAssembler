#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE

#include <string.h>
#include <stdlib.h>

#define TABLE_LENGTH 500

typedef struct {
	char key[100];
	unsigned int value;
} table_entry;

typedef struct {
	int current_index;
	table_entry entries[TABLE_LENGTH];
} symbol_table;

symbol_table* create_table();
void destroy_table(symbol_table* table);

void add_entry(symbol_table* table, table_entry entry);
int contains_entry(symbol_table* table, const char* key);
char* adress_of(symbol_table* table, const char* key);

#endif
