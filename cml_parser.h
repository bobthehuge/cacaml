#ifndef CML_LEXER_H
#define CML_LEXER_H

#include <stdint.h>

enum cml_tkind
{
    T_NULL,
    T_OPERAND,
    T_FUNCTION,
};

enum cml_callable_ids
{
    ADD,
};

typedef struct cml_token*(*cml_callable_fun)(struct cml_token *argv);

struct cml_i32
{
    uint32_t value;
};

struct cml_callable
{
    uint8_t id;
    size_t argc;
    cml_callable_fun call;
};

struct cml_token
{
    enum cml_tkind kind;
    void *value;
};

struct cml_ast
{
    struct cml_token key;
    struct cml_ast *kids;
    size_t count;
};

size_t serialize_ast(struct cml_ast *ast, char **dst);

#endif
