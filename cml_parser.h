#ifndef CML_LEXER_H
#define CML_LEXER_H

#include <stdint.h>

enum cmle_ast_kind
{
    AST_NULL,
    AST_OPERAND,
    AST_FUNCTION,
};

enum cmle_callable_ids
{
    ADD,
};

typedef struct cmle_ast*(*cmle_callable_fun)(struct cmle_ast *argv);

struct cmle_i32
{
    int32_t value;
};

struct cmle_callable
{
    uint8_t id;
    size_t argc;
    cmle_callable_fun call;
};

struct cmle_ast
{
    enum cmle_tkind kind;
    struct cmle_ast *kids;
    size_t count;
    void *value;
};

size_t serialize_ast(struct cmle_ast *ast, char **dst);

#endif
