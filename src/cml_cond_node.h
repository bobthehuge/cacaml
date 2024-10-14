#ifndef CML_COND_NODE_H
#define CML_COND_NODE_H

#include "cml_expr_node.h"
#include "cml_parser.h"

struct cml_cond_node
{
    struct cml_expr_node pred;
    struct cml_expr_node b_then;
    struct cml_expr_node b_else;
};

struct cml_cond_node *cml_new_cond(void);
void cml_parse_cond(struct cml_parser *pa, struct cml_expr_node *root);
void cml_emit_cond(struct cml_cond_node *cond, struct cml_ir_ctx *ctx);
void cml_print_cond(struct cml_cond_node *cond, uint32_t depth, FILE *file);
// void cml_free_cond(struct cml_cond_node *cond);

#endif
