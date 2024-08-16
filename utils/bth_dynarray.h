#ifndef BTH_DYNARRAY_H
#define BTH_DYNARRAY_H

// total size is isize * cap
struct bth_dynarray
{
    // item count
    unsigned int len;
    // item size
    unsigned int isize;
    // allocated capacity (not total size)
    unsigned int cap;
    void *items;
};

struct bth_dynarray bth_dynarray_init(
    unsigned int isize,
    unsigned int prealloc
);
void bth_dynarray_get(struct bth_dynarray *da, unsigned int index, void *e);
void bth_dynarray_set(struct bth_dynarray *da, unsigned int index, void *e);
void bth_dynarray_append(struct bth_dynarray *da, void *e);
void bth_dynarray_pop(struct bth_dynarray *da, void *e);

#endif

#ifdef BTH_DYNARRAY_IMPLEMENTATION

#include <err.h>
#include <string.h>

struct bth_dynarray bth_dynarray_init(
    unsigned int isize,
    unsigned int prealloc
) {
    void *data = malloc(isize * prealloc);

    if (!data && prealloc)
        errx(1, "Cannot prealloc %d items for bth_dynarray", prealloc);

    struct bth_dynarray da = {
        .len = 0,
        .isize = isize,
        .cap = prealloc,
        .items = data,
    };

    return da;
}

void bth_dynarray_free(struct bth_dynarray *da)
{
    free(da->items);
    da->cap = 0;
    da->len = 0;
}

void bth_dynarray_resize(struct bth_dynarray *da, unsigned int n)
{
    void *data = realloc(da->items, da->isize * n);

    if (!data && n)
    {
        errx(
             1, 
             "Cannot realloc %d items of size %d for bth_dynarray",
             n,
             da->isize
        );
    }

    da->items = data;
    da->len = n > da->len ? da->len : n;
    da->cap = n;
}

void bth_dynarray_get(struct bth_dynarray *da, unsigned int index, void *e)
{
    if (index >= da->len)
        errx(1, "Index out of bound of bth_dynarray");

    memcpy(e, da->items + index * da->isize, da->isize);
}

void bth_dynarray_set(struct bth_dynarray *da, unsigned int index, void *e)
{
    if (index >= da->len)
        errx(1, "Index out of bound of bth_dynarray");

    memcpy(da->items + index * da->isize, e, da->isize);
}

void bth_dynarray_append(struct bth_dynarray *da, void *e)
{
    if (da->len >= da->cap)
        bth_dynarray_resize(da, da->cap + 1);

    da->len++;
    bth_dynarray_set(da, da->len - 1, e);
}

void bth_dynarray_pop(struct bth_dynarray *da, void *e)
{
    if (!da->len)
        errx(1, "Invalid pop on empty bth_dynarray");

    if (e)
        bth_dynarray_get(da, da->len - 1, e);
    
    da->len--;
}

#endif
