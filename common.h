#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int x;
    int y;
} Vec2i;

char* get_file_content(char* path, size_t* flen);

#endif /* COMMON_H_ */
