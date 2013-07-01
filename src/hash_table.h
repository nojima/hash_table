#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>

typedef struct HashTableEntry {
    struct HashTableEntry* next;
    char* key;
    void* value;
} HashTableEntry;

typedef struct {
    size_t size;
    size_t capacity;
    HashTableEntry** entries;
} HashTable;

HashTable*
HashTable_create();

void
HashTable_destroy(HashTable* hash_table);

void*
HashTable_get(const HashTable* hash_table, const char* key);

void
HashTable_set(HashTable* hash_table, const char* key, void* value);

int
HashTable_size(const HashTable* hash_table);

#endif
