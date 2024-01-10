#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/bth_cstr.h"
#include "../utils/cml_log.h"

#include "cml_parser.h"
#include "cml_serialize_ast.h"

#define MAGIC_SIZE 8

struct bth_cstr *__cml_ast2cstr(struct cml_ast *ast)
{
    size_t size = MAGIC_SIZE * 2 + 3;
    uint32_t value;

    if (ast->kind == CML_AST_NULL)
    {
        value = 0xdeadbeef;
    }
    else if (ast->kind == CML_AST_OPERAND)
    {
        struct cmlenv_i32 *num = ast->value;
        value = num->value;
    }
    else if (ast->kind == CML_AST_FUNCTION)
    {
        struct cmlenv_callable *call = ast->value;
        value = call->id;
    }
    else
    {
        ERRX(1, "TODO: unsupported TokenKind serialization");
    }

    struct bth_cstr *cstr = bth_cstr_alloc(size);
    sprintf(cstr->data, "(%08X;%08X", ast->kind, value);

    return cstr;
}

struct bth_cstr *cml_ast2cstr(struct cml_ast *ast)
{
    struct bth_cstr *root = __cml_ast2cstr(ast);

    for (size_t i = 0; i < ast->count; i++)
    {
        struct bth_cstr *child = cml_ast2cstr(&ast->kids[i]);
        
        bth_cstr_cat(root, child);
        bth_cstr_free(child);
    }

    bth_cstr_append(root, ")", 1);

    return root;
}
