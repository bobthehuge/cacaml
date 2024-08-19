#ifndef CML_TOKEN_H
#define CML_TOKEN_H

#include <stdint.h>
#include <stdio.h>

#include "../utils/bth_cstr.h"

#define CML_TKIND_KW_OFFSET TK_LET
#define CML_KW_COUNT TK_IDENT - TK_LET

enum cml_tkind
{
    END = 0, // should ALWAYS be the first kind
    TK_INVALID,
    TK_INT32,
    TK_ADD,
    TK_MUL,
    TK_EQ,
    TK_LET, // should ALWAYS be the FIRST keyword kind
    TK_IN,
    TK_IF,
    TK_THEN,
    TK_ELSE,
    TK_IDENT, // should ALWAYS be AFTER the last keyword kind
};

struct cml_token
{
    enum cml_tkind kind;
    char *value;
};

struct cml_lexer
{
    char *buf_path;
    char *buf;
    uint32_t buf_len;
    uint32_t pos;
    uint32_t read_pos;
    char ch;
};

const char *cml_tkind2str(enum cml_tkind kind);

#endif
