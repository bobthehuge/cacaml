#ifndef CML_LEXER_H
#define CML_LEXER_H

#include <stdint.h>

enum cml_ast_kind
{
    CML_AST_NULL,
    CML_AST_OPERAND,
    CML_AST_FUNCTION,
};

enum cmlenv_callable_ids
{
    CML_ENV_ADD,
};

typedef struct cml_ast*(*cmlenv_callable_fun)(struct cml_ast *argv);

struct cmlenv_i32
{
    int32_t value;
};

struct cmlenv_callable
{
    uint32_t id;
    size_t argc;
    cmlenv_callable_fun call;
};

struct cml_ast
{
    uint8_t kind;
    struct cml_ast *kids;
    size_t count;
    void *value;
};

#endif
