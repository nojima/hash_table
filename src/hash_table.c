#include "hash_table.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define INITIAL_CAPACITY 4

static size_t
hash(const char* str)
{
    size_t i, h = 0;
    for (i = 0; str[i] != '\0'; ++i)
        h = 31 * h + str[i];
    return h;
}

static inline size_t
hash_to_index(size_t h, size_t size)
{
    return h % size;
}

static void
append_entry(HashTableEntry** entries, size_t index, HashTableEntry* new_entry)
{
    HashTableEntry* p;

    assert(entries != NULL);
    assert(new_entry != NULL);
    new_entry->next = NULL;
    if (entries[index]) {
        for (p = entries[index]; p->next; p = p->next) {}
        p->next = new_entry;
    } else {
        entries[index] = new_entry;
    }
}

static void
extend_hash_table(HashTable* hash_table)
{
    HashTableEntry** old_entries;
    HashTableEntry** new_entries;
    HashTableEntry* entry;
    HashTableEntry* next;
    size_t old_capacity;
    size_t new_capacity;
    size_t new_index;
    size_t i;

    old_entries = hash_table->entries;
    old_capacity = hash_table->capacity;
    new_capacity = old_capacity * 2;
    new_entries = (HashTableEntry**)calloc(new_capacity, sizeof(HashTableEntry*));

    for (i = 0; i < old_capacity; ++i) {
        for (entry = old_entries[i]; entry; entry = next) {
            new_index = hash_to_index(hash(entry->key), new_capacity);
            next = entry->next;
            append_entry(new_entries, new_index, entry);
        }
    }

    free(old_entries);
    hash_table->entries = new_entries;
    hash_table->capacity = new_capacity;
}

HashTable*
HashTable_create()
{
    HashTable* hash_table = NULL;
    HashTableEntry** entries = NULL;

    hash_table = (HashTable*)calloc(1, sizeof(HashTable));
    if (!hash_table)
        goto error;

    entries = (HashTableEntry**)calloc(INITIAL_CAPACITY, sizeof(HashTableEntry));
    if (!entries)
        goto error;
    hash_table->size = 0;
    hash_table->capacity = INITIAL_CAPACITY;
    hash_table->entries = entries;

    return hash_table;

error:
    free(entries);
    free(hash_table);
    return NULL;
}

void
HashTable_destroy(HashTable* hash_table)
{
    size_t i;
    HashTableEntry* p;
    HashTableEntry* next;

    for (i = 0; i < hash_table->capacity; ++i) {
        if (!hash_table->entries[i])
            continue;
        for (p = hash_table->entries[i]; p; p = next) {
            next = p->next;
            free(p->key);
            free(p);
        }
    }
    free(hash_table->entries);
    free(hash_table);
}

void*
HashTable_get(const HashTable* hash_table, const char* key)
{
    size_t index;
    HashTableEntry* p;

    index = hash_to_index(hash(key), hash_table->capacity);
    for (p = hash_table->entries[index]; p; p = p->next) {
        if (strcmp(p->key, key) == 0)
            return p->value;
    }
    return NULL;
}

void
HashTable_set(HashTable* hash_table, const char* key, void* value)
{
    size_t index;
    HashTableEntry* p;

    index = hash_to_index(hash(key), hash_table->capacity);
    for (p = hash_table->entries[index]; p; p = p->next) {
        if (strcmp(p->key, key) == 0) {
            p->value = value;
            return;
        }
    }

    if (hash_table->size * 2 >= hash_table->capacity) {
        extend_hash_table(hash_table);
        index = hash_to_index(hash(key), hash_table->capacity);
    }
    p = (HashTableEntry*)calloc(1, sizeof(HashTableEntry));
    p->key = strdup(key);
    if (!p->key) {
        // TODO: メモリが割り当てられなかった場合の処理を書く
        assert(0);
    }
    p->value = value;
    append_entry(hash_table->entries, index, p);
    hash_table->size += 1;
}

int
HashTable_size(const HashTable* hash_table)
{
    return hash_table->size;
}
