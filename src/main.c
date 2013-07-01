#include "hash_table.h"
#include <assert.h>
#include <stdio.h>

int main()
{
    HashTable* h;

    h = HashTable_create();

    assert(HashTable_size(h) == 0);
    HashTable_set(h, "hoge", (void*)100);
    HashTable_set(h, "fuga", (void*)200);
    HashTable_set(h, "piyo", (void*)300);
    assert(HashTable_size(h) == 3);
    assert(HashTable_get(h, "hoge") == (void*)100);
    assert(HashTable_get(h, "fuga") == (void*)200);
    assert(HashTable_get(h, "piyo") == (void*)300);
    assert(HashTable_get(h, "poyopoyo") == NULL);
    assert(HashTable_size(h) == 3);

    HashTable_destroy(h);
    return 0;
}
