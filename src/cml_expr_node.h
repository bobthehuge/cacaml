#ifndef CML_AST_EXPR_H
#define CML_AST_EXPR_H

#include <stdint.h>
#define _GNU_SOURCE
#include <stdio.h>
#include "cml_ast.h"
#include "cml_parser.h"

struct cml_expr_node
{
    const char *type;
    enum cml_ekind kind;
    union {
        struct cml_info_node *value;
        struct cml_info_node *int32;
        struct cml_expr_node *expr;
        struct cml_let_node *let;
        // struct cml_unop_node unop;
        struct cml_binop_node *binop;
    };
};

struct cml_expr_node *cml_new_expr(void);
void cml_parse_expr(struct cml_parser *pa, struct cml_expr_node *expr);
void cml_emit_expr(struct cml_expr_node *expr, struct cml_ir_ctx *ctx);
void cml_print_expr(struct cml_expr_node *expr, uint32_t depth);
void cml_free_expr(struct cml_expr_node *expr);

#endif
