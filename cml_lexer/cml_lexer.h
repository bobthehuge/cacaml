#ifndef CML_LEXER_H
#define CML_LEXER_H

#include <stdint.h>

enum cml_token_kind
{
    CML_TOK_NULL,
    CML_TOK_I32,
    CML_TOK_CALL,
};

struct cml_token
{
    uint32_t kind;
    char **value;
};

#endif
