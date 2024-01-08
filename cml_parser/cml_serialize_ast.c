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
    char value[9] = {0};

    if (ast->kind == CML_AST_OPERAND)
    {
        struct cmlenv_i32 *num = ast->value;

        int32_t val = num->value;
        sprintf(value, "%08X", val);
    }
    else if (ast->kind == CML_AST_FUNCTION)
    {
        struct cmlenv_callable *call = ast->value;

        uint32_t val = call->id;
        sprintf(value, "%08X", val);
    }
    else
    {
        ERRX(1, "TODO: unsupported TokenKind serialization");
    }

    struct bth_cstr *cstr = bth_cstr_alloc(size);
    sprintf(cstr->data, "(%08X;%s", ast->kind, value);

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
