#ifndef COMMON_H
#define COMMON_H

#include "../utils/bth_log.h"

#define ERROR(code, ...) ERR(code, __VA_ARGS__)
#define STRINGIFY(s) #s
#define CSTRNCMP(s1, s2) strncmp((s1), (s2).data, s2.len)

void escprints(char *s);
void nputchar(char c, unsigned int n);

#endif
