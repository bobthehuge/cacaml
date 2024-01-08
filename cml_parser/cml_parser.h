#ifndef CML_LEXER_H
#define CML_LEXER_H

#include <stdint.h>

enum cml_ast_kind
{
    AST_NULL,
    AST_OPERAND,
    AST_FUNCTION,
};

enum cmlenv_callable_ids
{
    ADD,
};

typedef struct cml_ast*(*cmlenv_callable_fun)(struct cml_ast *argv);

struct cmlenv_i32
{
    int32_t value;
};

struct cmlenv_callable
{
    uint8_t id;
    size_t argc;
    cmlenv_callable_fun call;
};

struct cml_ast
{
    enum cml_ast_kind kind;
    struct cml_ast *kids;
    size_t count;
    void *value;
};

#endif
