#ifndef CML_AST_BINOP
#define CML_AST_BINOP

#include <stdint.h>
#include "cml_info_node.h"
#include "cml_parser.h"

struct cml_binop_node
{
    struct cml_info_node op;
    struct cml_expr_node *lhs;
    struct cml_expr_node *rhs;
};

struct cml_binop_node *cml_new_binop(void);
void cml_parse_factor(struct cml_parser *pa, struct cml_expr_node *fact);
void cml_parse_term(struct cml_parser *pa, struct cml_binop_node *term);
void cml_parse_binop(struct cml_parser *pa, struct cml_expr_node *expr);
void cml_print_binop(struct cml_binop_node *root, uint32_t padd);
void cml_free_binop(struct cml_binop_node *root);

#endif
