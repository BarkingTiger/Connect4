#include <inttypes.h>

struct Entry {
        uint64_t key: 56;
        uint8_t val;
};

typedef struct Entry Entry;

struct Table{
        Entry *Entries;
        int size;
        int capacity;
};

typedef struct Table Table;

Table *tt_create(int capacity);

void tt_delete(Table **t);

unsigned int tableIndex(Table *table, uint64_t key);

void reset(Table *table);

void put(Table *table, uint64_t key, uint8_t val);

uint8_t get(Table *table, uint64_t key);
