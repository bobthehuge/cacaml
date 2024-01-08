#include "cml_cstr.h"

struct cml_cstr *cml_cstr_new(size_t prealloc_size)
{
    struct cml_cstr *cstr = malloc(sizeof(struct cml_cstr));

    if (cstr == NULL)
    {
        ERR(1, "Can't alloc struct cstr");
    }

    char **data = malloc(prealloc_size+1);

    if (data == NULL && prealloc_size != 0)
    {
        ERR(1, "Can't prealloc cstr content of size '%zu'", prealloc_size+1);
    }

    cstr->data = data;
    cstr->size = size;
}

void cml_cstr_free(struct cml_cstr *cstr)
{
    if (cstr->data != NULL)
    {
        free(cstr->data);
    }

    free(cstr);
}

void cml_cstr_resize(struct cml_cstr *cstr, size_t size)
{
    if (cstr->data == NULL)
    {
        if (size != 0)
        {
            ERRX(1, "Can't resize cstr NULL content to size '%zu'", size);
        }

        return;
    }

    cstr->data = realloc(cstr->data, size);

    if (cstr->data == NULL && size != 0)
    {
        ERRX(1, "Can't resize cstr content to size '%zu'", size);
    }

    cstr->size = size;
}

void cml_cstr_append(struct cml_cstr *cstr, char **str, size_t n)
{
    if (str == NULL && size != 0)
    {
        ERRX(1, "Can't read '%zu' bytes from NULL str", n);
    }

    cml_cstr_resize(cstr, cstr->size+size+1);
    memmove(cstr->data+cstr->length, str, n);
    cstr->length += n;
    cstr->content[cstr->length-1] = '\0';
}

void cml_cstr_cat(struct cml_cstr *dst, struct cml_cstr *src)
{
    cml_cstr_resize(src, src->length+dst->length);
    memmove(cstr->data+cstr->length, dst->data, dst->length);
}

void cml_cstr_catnfree(struct cml_cstr *src, struct cml_cstr *dst)
{
    cml_cstr_cat(src, dst);
    cml_cstr_free()
}
