#ifndef CML_PARSER_H
#define CML_PARSER_H

#include <stdlib.h>
#include <stdint.h>

#include "cml_lexer.h"

// #define CMLENV_IDS_COUNT 1
// enum cmlenv_callable_ids
// {
//     CML_ENV_ADD,
// };

// // typedef struct cml_ast*(*cmlenv_callable_fun)(struct cml_ast *argv);
// typedef struct cml_token*(*cmlenv_callable_fun)(struct cml_token *argv);

struct cmlenv_i32
{
    int32_t value;
};

// struct cmlenv_callable
// {
//     uint32_t id;
//     size_t argc;
//     cmlenv_callable_fun call;
// };

// struct cml_ast
// {
//     uint32_t kind;
//     struct cml_ast *kids;
//     size_t count;
//     void *value;
// };

void cml_build_ast(struct cml_token *tokens, size_t count);

#endif
