// MIT License
// 
// Copyright (c) 2024 bobthehuge
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in 
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.

#include <err.h>
#include <string.h>

#include "bth_htab.h"

uint32_t one_at_a_time(char *key)
{
    size_t i = 0;
    uint32_t hash = 0;
    size_t len = strlen(key);

    while (i != len)
    {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash;
}

uint32_t djb2(char *key)
{
    uint32_t hash = 5381;
    int c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

struct htab *htab_init(size_t cap)
{
    struct htab *res = malloc(sizeof(struct htab));

    if (res == NULL)
    {
        errx(1, "Not enough memory!");
    }

    res->capacity = cap;
    res->size = 0;

    res->data = calloc(cap, sizeof(struct htab_pair));

    if (res->data == NULL)
    {
        errx(1, "Not enough memory!");
    }

    return res;
}

struct htab *htab_new()
{
    return htab_init(4);
}

void htab_clear(struct htab *ht)
{
    for (size_t i = 0; i < ht->capacity; ++i)
    {
        struct htab_pair *elt = ht->data[i].next;
        ht->data[i].next = NULL;

        while (elt != NULL)
        {
            struct htab_pair *nxt = elt->next;
            free(elt);
            elt = nxt;
        }
    }

    ht->size = 0;
}

void htab_free(struct htab *ht)
{
    htab_clear(ht);
    free(ht->data);
    free(ht);
}

struct htab_pair *htab_get(struct htab *ht, char *key)
{
    uint32_t h = HASH(key);
    size_t idx = h % ht->capacity;

    struct htab_pair *elt = ht->data[idx].next;

    while (elt != NULL && strcmp(key, elt->key))
    {
        elt = elt->next;
    }

    return elt;
}

struct htab_pair *htab_find(struct htab *ht, char *key, size_t *idx_ptr)
{
    uint32_t h = HASH(key);
    size_t hidx = h % ht->capacity;
    size_t bidx = 0;

    struct htab_pair *elt = ht->data[hidx].next;

    while (elt != NULL && strcmp(key, elt->key))
    {
        elt = elt->next;
        bidx++;
    }

    if (idx_ptr)
        *idx_ptr = bidx;
    return elt;
}

void htab_expand(struct htab *ht)
{
    struct htab *new = htab_init(ht->capacity * 2);

    for (size_t i = 0; i < ht->capacity; ++i)
    {
        struct htab_pair *elt = ht->data[i].next;

        while (elt != NULL)
        {
            struct htab_pair *nxt = elt->next;
            htab_insert(new, elt->key, elt->value);
            free(elt);
            elt = nxt;
        }
    }

    free(ht->data);
    *ht = *new;
    free(new);
}

int htab_insert(struct htab *ht, char *key, void *value)
{
    uint32_t h = HASH(key);
    size_t idx = h % ht->capacity;

    if (htab_get(ht, key) != NULL)
    {
        return 0;
    }

    if (ht->size * 100 / ht->capacity > 75)
    {
        htab_expand(ht);
    }

    struct htab_pair *p = malloc(sizeof(struct htab_pair));

    if (ht->data[idx].next == NULL)
    {
        ht->size++;
    }

    p->hkey = h;
    p->key = key;
    p->value = value;
    p->next = ht->data[idx].next;

    ht->data[idx].next = p;

    return 1;
}

void htab_remove(struct htab *ht, char *key)
{
    uint32_t h = HASH(key);
    size_t idx = h % ht->capacity;

    struct htab_pair *elt = ht->data + idx;

    if (elt == NULL)
    {
        return;
    }

    while (elt->next != NULL && strcmp(key, elt->next->key))
    {
        elt = elt->next;
    }

    if (elt->next != NULL)
    {
        struct htab_pair *tofree = elt->next;
        elt->next = elt->next->next;

        free(tofree);
    }

    if (ht->data[idx].next == NULL)
    {
        ht->size--;
    }
}
