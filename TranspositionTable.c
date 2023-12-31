#include "TranspositionTable.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Table *tt_create(int size) {
	Table *table = (Table *)malloc(sizeof(Table));
	if (table) {
		table->Entries = (Entry *)malloc(sizeof(Entry) * size); //8388593
		table->size = size;
		return table;
	}
	return NULL;
}

void tt_delete(Table **t) {
	free((*t)->Entries);
	free(*t);
}

unsigned int tableIndex(Table *table, uint64_t key) {
	return key % table->size;
}

void reset(Table *table) {
	memset(table->Entries, 0, table->size * sizeof(Entry));
}

void put(Table *table, uint64_t key, uint8_t val) {
	unsigned int i = tableIndex(table, key);
	table->Entries[i].key = key;
	table->Entries[i].val = val;
}

uint8_t get(Table *table, uint64_t key) {
	unsigned int i = tableIndex(table, key);
	if (table->Entries[i].key == key) {
		return table->Entries[i].val;
	}
	else {
		return 0;
	}
}

void printAll(Table *table) {
	for (int i = 0; i < table->size; i += 1) {
		printf("I %d\n", i);
		printf("KEY %" PRIu64 "\n", table->Entries[i].key);
		printf("VAL %d\n", table->Entries[i].val);
	}
}	
