#ifndef CML_AST_LET_H
#define CML_AST_LET_H

#include <stdint.h>
#include "cml_ast_info.h"
#include "cml_ast_expr.h"
#include "cml_parser.h"

struct cml_let_binding_node
{
    struct cml_info_node name;
    struct cml_expr_node body;
};

struct cml_let_node
{
    struct cml_let_binding_node binding;
    struct cml_expr_node body;
};

struct cml_let_binding_node *cml_new_let_binding(void);
void cml_parse_let_binding(struct cml_parser *pa, struct cml_expr_node *root);
void cml_print_let_binding(struct cml_let_binding_node *root, uint32_t padd);
void cml_free_let_binding(struct cml_let_binding_node *root);

struct cml_let_node *cml_new_let(void);
void cml_parse_let(struct cml_parser *pa, struct cml_expr_node *root);
void cml_print_let(struct cml_let_node *root, uint32_t padd);
void cml_free_let(struct cml_let_node *root);

#endif

