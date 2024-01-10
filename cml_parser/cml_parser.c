#include <stdlib.h>

#include "cml_parser.h"

struct cml_ast *cml_ast_alloc(void)
{
    struct cml_ast *ast = malloc(sizeof(struct cml_ast));
    return ast;
}

struct cml_ast *cml_build_ast(struct cml_token **tokens)
{
    struct cml_ast *ast = cml_ast_alloc();
    ast->kind = CML_AST_NULL;

    (void)tokens;

    return ast;
}
