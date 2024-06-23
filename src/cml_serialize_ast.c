#include "cml_serialize_ast.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/bth_cstr.h"
#include "../utils/bth_htab.h"
#include "../utils/bth_log.h"
#include "cml_lexer.h"
#include "cml_parser.h"

#define MAGIC_SIZE 8

struct bth_cstr *__cml_ast2cstr(struct cml_token *ast)
{
    size_t size = MAGIC_SIZE * 2 + 1;

    struct bth_cstr *cstr = NULL;

    if (ast->kind == CML_TOK_NULL)
    {
        uint32_t value = 0xdeadbeef;

        cstr = bth_cstr_alloc(size);
        sprintf(cstr->data, "%08X%08X", ast->kind, value);
    }
    else if (ast->kind == CML_TOK_I32)
    {
        struct cml_i32_t *num = ast->value;
        uint32_t value = num->value;

        cstr = bth_cstr_alloc(size);
        sprintf(cstr->data, "%08X%08X", ast->kind, value);
    }
    else if (ast->kind == CML_TOK_BINOP)
    {
        struct cml_func_t *op = ast->value;
        uint32_t value = HASH(op->name);

        cstr = bth_cstr_alloc(size + MAGIC_SIZE);
        sprintf(
            cstr->data,
            "%08X%08X%08X",
            ast->kind,
            value,
            op->fid
        );
    }
    else
    {
        TODOX(1, "Unsupported TokenKind serialization");
    }

    return cstr;
}

struct bth_cstr *cml_ast2cstr(struct cml_token *ast)
{
    struct bth_cstr *root = __cml_ast2cstr(ast);

    for (size_t i = 0; i < ast->count; i++)
    {
        struct bth_cstr *child = cml_ast2cstr(ast->kids[i]);

        bth_cstr_cat(root, child);
        bth_cstr_free(child);
    }

    // bth_cstr_append(root, ")", 1);

    return root;
}
