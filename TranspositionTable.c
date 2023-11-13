#include "TranspositionTable.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

Table *tt_create(int capacity) {
	Table *table = (Table *)malloc(sizeof(Table));
	if (table) {
		table->Entries = (Entry *)malloc(sizeof(Entry) * capacity); //8388593
		table->size = 0;
		table->capacity = capacity;
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
	memset(table->Entries, 0, table->capacity * sizeof(Entry));
	table->size = 0;
}

void put(Table *table, uint64_t key, uint8_t val) {
	unsigned int i = tableIndex(table, key);
	if (table->Entries[i].key == 0) {
		table->size += 1;
	}
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
