#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add_char(char** buffer, size_t* buf_size, size_t* occ, char chr)
{
    char* tmp = *buffer;
    (*occ)++;

    if(*occ >= *buf_size)
    {
        *buffer = realloc(*buffer, *occ);
        *buf_size = *occ;
    }

    *(tmp + *occ - 1) = chr;
}


int main(void)
{
    char* test = malloc(2);
    size_t buf_size = 2;
    size_t occ = 0;

    add_char(&test, &buf_size, &occ, 'a');   //occ = 1
    add_char(&test, &buf_size, &occ, 'b');   //occ = 2
    add_char(&test, &buf_size, &occ, 'c');   //occ = 3

    printf("%s\n", test);

    return 0;
}
