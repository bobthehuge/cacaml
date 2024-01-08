#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/bth_cstr.h"
#include "../utils/cml_log.h"
#include "../utils/cml_utils.h"

#include "cml_parser.h"
#include "cml_serialize_ast.h"

struct bth_cstr *__cml_ast2cstr(struct cml_ast *ast)
{
    size_t size = getlongdigits(ast->kind)+2;
    int32_t val;

    if (ast->kind == AST_OPERAND)
    {
        struct cmlenv_i32 *num = ast->value;
        val = num->value;
    }
    else if (ast->kind == AST_FUNCTION)
    {
        struct cmlenv_callable *call = ast->value;
        val = call->id;
    }
    else
    {
        ERRX(1, "TODO: unsupported TokenKind serialization");
    }

    size += getlongdigits(val);

    struct bth_cstr *cstr = bth_cstr_alloc(size);
    /* *dst = malloc(size); */
    /* (*dst)[size-2] = '\0'; */
    /* BTH_CSTR_TA(cstr,1) = '\0'; */
    sprintf(cstr->data, "(%u;%u", ast->kind, val);

    return cstr;
}

struct bth_cstr *cml_ast2cstr(struct cml_ast *ast)
{
    struct bth_cstr *root = __cml_ast2cstr(ast);

    for (size_t i = 0; i < ast->count; i++)
    {
        struct bth_cstr *child = cml_ast2cstr(&ast->kids[i]);
        
        /* *dst = realloc(*dst, size); */
        /* strcat(*dst, child); */
        /* free(child); */
        bth_cstr_cat(root, child);
        bth_cstr_free(child);
    }

    /* (*dst)[size-2] = ')'; */
    /* (*dst)[size-1] = '\0'; */
    /* BTH_CSTR_TA(root, 1) = ')'; */
    /* BTH_CSTR_TA(root, 0) = '\0'; */
    /* bth_cstr_append(root, ")", 1); */

    return root;
}
