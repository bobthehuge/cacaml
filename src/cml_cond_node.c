#include "cml_cond_node.h"
#include <cml_expr_node.h>
#include "../utils/bth_salloc.h"

struct cml_cond_node *cml_new_cond(void)
{
    return smalloc(sizeof(struct cml_cond_node));
}

void cml_parse_cond(struct cml_parser *pa, struct cml_expr_node *root);

void cml_emit_cond(struct cml_cond_node *cond, struct cml_ir_ctx *ctx);

void cml_print_cond(struct cml_cond_node *cond, uint32_t depth, FILE *file);
