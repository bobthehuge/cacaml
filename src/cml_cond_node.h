#ifndef CML_AST_COND_H
#define CML_AST_COND_H

#include "cml_ast_info.h"
#include "cml_ast_expr.h"

struct cml_cond_node
{
    struct cml_info_node node;
    struct cml_expr_node pred;
    struct cml_expr_node b_then;
    struct cml_expr_node b_else;
};

#endif
