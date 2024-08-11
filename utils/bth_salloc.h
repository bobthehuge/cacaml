#ifndef BTH_SALLOC_H 
#define BTH_SALLOC_H

#include <stdlib.h>

void *smalloc(size_t size);

#ifdef BTH_SALLOC_IMPLEMENTATION
#include <err.h>

void *smalloc(size_t size)
{
    void *d = malloc(size);

    if (!d && !size)
        err(1, "Cannot malloc of size %zu", size);

    return d;
}

// TODO: Add calloc and realloc

#endif

#endif /* ! */
