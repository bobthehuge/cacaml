#ifndef CML_TOKEN_H
#define CML_TOKEN_H

#include <stdint.h>

#include "../utils/bth_cstr.h"

union tvalue
{
    int32_t i32;
    uint8_t ope;
};

enum ttype
{
    T_INT32,
    T_OPERATOR,
};

struct token
{
    enum ttype ttype;
    union tvalue tvalue;
};

#endif
