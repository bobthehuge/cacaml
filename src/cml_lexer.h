#ifndef CML_LEXER_H
#define CML_LEXER_H

#include <stdlib.h>
#include <stdint.h>

enum cml_token_kind
{
    CML_TOK_NULL,
    CML_TOK_I32,
    CML_TOK_UNOP,
    CML_TOK_BINOP,
    CML_TOK_CALL,
};

struct cml_i32_t
{
    int32_t value;
};

struct cml_func_t
{
    uint32_t fid;
    char *name;
    size_t argc;
};

struct cml_token
{
    uint32_t kind;
    struct cml_token *parent;
    struct cml_token **kids;
    size_t count;
    void *value;
};

#ifdef CML_TOKEN_MACROS
#define CML_TOKEN_NULL (struct cml_token){0}

#define CML_TOKEN_I32(n) (struct cml_token){\
    .kind = CML_TOK_I32,\
    .parent = NULL,\
    .kids = NULL,\
    .count = 0,\
    .value = &(struct cml_i32_t){\
        .value = n\
    }\
}

#define CML_TOKEN_BINOP(c) (struct cml_token){\
    .kind = CML_TOK_BINOP,\
    .parent = NULL,\
    .kids = NULL,\
    .count = 0,\
    .value = &(struct cml_func_t){\
        .fid = 0,\
        .name = c,\
        .argc = 2\
    }\
}
#endif

#endif
