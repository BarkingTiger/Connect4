#include <inttypes.h>
#include <string.h>

int size = 0;

struct Entry {
	uint64_t key: 56;
	uint8_t val;
};

struct Entry *Table;

unsigned int tableIndex(uint64_t key) {
	return key % size;
}

void reset() {
	memset(&Table[0], 0, size * sizeof(struct Entry));
	size = 0;
}

void put(uint64_t key, uint8_t val) {
	unsigned int i = tableIndex(key);
	Table[i].key = key;
	Table[i].val = val;
}

uint8_t get(uint64_t key) {
	unsigned int i = tableIndex(key);
	if (Table[i].key == key) {
		return Table[i].val;
	}
	else {
		return 0;
	}
}
