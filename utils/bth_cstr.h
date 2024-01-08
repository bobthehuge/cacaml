#ifndef BTH_CSTR_H
#define BTH_CSTR_H

#include <stddef.h>

struct bth_cstr
{
    size_t len;
    char *data;
};

#define BTH_CSTR_AT(cstr, i) (cstr)->data[(i)]
#define BTH_CSTR_TA(cstr, i) (cstr)->data[(cstr)->len-(i)]

struct bth_cstr *bth_cstr_new(void);
struct bth_cstr *bth_cstr_alloc(size_t size);
struct bth_cstr *bth_cstr_from(char *src);
void bth_cstr_free(struct bth_cstr *cstr);
void bth_cstr_resize(struct bth_cstr *cstr, size_t size);
void bth_cstr_append(struct bth_cstr *cstr, char *src, size_t n);
void bth_cstr_cat(struct bth_cstr *dst, struct bth_cstr *src);

#endif

#ifdef BTH_CSTR_IMPLEMENTATION

#include <err.h>
#include <stdlib.h>
#include <string.h>

struct bth_cstr *bth_cstr_new(void)
{
    struct bth_cstr *cstr = malloc(sizeof(struct bth_cstr));

    if (cstr == NULL)
    {
        err(1, "Can't allocate cstr");
    }

    cstr->len = 0;
    cstr->data = malloc(0);

    return cstr;
}

struct bth_cstr *bth_cstr_alloc(size_t size)
{
    struct bth_cstr *cstr = bth_cstr_new();

    cstr->data = malloc(size*sizeof(char));

    if (cstr->data == NULL && size != 0)
    {
        err(1, "Can't allocate cstr data of size '%zu'", size);
    }
    else if (size != 0)
    {
        cstr->len = size-1;
    }

    return cstr;
}

void bth_cstr_free(struct bth_cstr *cstr)
{
    free(cstr->data);
    free(cstr);
}

void bth_cstr_resize(struct bth_cstr *cstr, size_t size)
{
    char *data = realloc(cstr->data, size);

    if (data == NULL && size != 0)
    {
        err(1, "Can't resize cstr to size '%zu'", size);
    }

    cstr->data = data;
}

void bth_cstr_append(struct bth_cstr *cstr, char *src, size_t n)
{
    bth_cstr_resize(cstr, cstr->len + 1 + n);
    char *org = cstr->data + cstr->len;
    memcpy(org, src, n);
    cstr->len += n;
}

struct bth_cstr *bth_cstr_from(char *src)
{
    struct bth_cstr *cstr = bth_cstr_new();

    size_t len = strlen(src);
    bth_cstr_append(cstr, src, len);
    cstr->len = len;

    return cstr;
}


void bth_cstr_cat(struct bth_cstr *dst, struct bth_cstr *src)
{
    bth_cstr_append(dst, src->data, src->len);
}

#endif
