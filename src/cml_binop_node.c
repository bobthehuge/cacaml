#include <stdlib.h>

#include "../utils/bth_salloc.h"

#include "cml_ast.h"
#include "cml_expr_node.h"
#include "cml_binop_node.h"
#include "common.h"

struct cml_binop_node *cml_new_binop(void)
{
    return smalloc(sizeof(struct cml_binop_node));
}

void cml_parse_factor(struct cml_parser *pa, struct cml_expr_node *fact)
{
    if (pa->curr.kind == TK_INT32)
    {
        fact->int32 = cml_new_info();
        fact->type = cml_type2str(INT32);
        fact->kind = EK_CONSTANT;
        fact->int32->value = pa->curr.value;
        fact->int32->row = pa->curr.row;
        fact->int32->col = pa->curr.col;
    }
    else if (pa->curr.kind == TK_IDENT)
    {
        fact->value = cml_new_info();
        fact->type = NULL;
        fact->kind = EK_VALUE_NAME;
        fact->value->value = pa->curr.value;
        fact->value->row = pa->curr.row;
        fact->value->col = pa->curr.col;
    }
    else
    {
        ERR_UNEXP_TOKEN(&pa->curr);
    }

    cml_parser_next(pa);
}

void cml_parse_term(struct cml_parser *pa, struct cml_binop_node *term)
{
    struct cml_expr_node *lhs = cml_new_expr();
    cml_parse_factor(pa, lhs);

    // Term = Factor | Factor (* | /) Expr
    if (pa->curr.kind == TK_MUL)
    {
        cml_parser_next(pa); // rhs Expr

        struct cml_expr_node *rhs = cml_new_expr();
        rhs->binop = cml_new_binop();
        cml_parse_term(pa, rhs->binop);

        if (rhs->binop->op.value != NULL)
            rhs->kind = EK_BINOP;
        else
        {
            struct cml_expr_node *t = rhs->binop->lhs;
            free(rhs->binop);
            free(rhs);
            rhs = t;
        }

        struct cml_info_node op = {
            .value = "*",
        };
        
        term->op = op;
        term->lhs = lhs;
        term->rhs = rhs;
    }
    else
    {
        term->op.value = NULL;
        term->lhs = lhs;
    }
}

// Expr = Term (+- Expr)*
// Term = Factor (*/% Term)*
// Factor = Literal | (Expr)

void cml_parse_binop(struct cml_parser *pa, struct cml_expr_node *expr)
{
    expr->type = cml_type2str(UNRESOLVED);

    struct cml_expr_node *lhs = cml_new_expr();
    lhs->type = cml_type2str(UNRESOLVED);
    lhs->binop = cml_new_binop();
    cml_parse_term(pa, lhs->binop);

    if (lhs->binop->op.value != NULL)
        lhs->kind = EK_BINOP;
    else
    {
        struct cml_expr_node *t = lhs->binop->lhs;
        free(lhs->binop);
        free(lhs);
        lhs = t;
    }

    // Expr = Term | Term (+ | -) Expr
    if (pa->curr.kind == TK_ADD)
    {
        struct cml_info_node op = {
            .value = "+",
            .row = pa->curr.row,
            .col = pa->curr.col
        };

        cml_parser_next(pa); // rhs Expr

        struct cml_expr_node *rhs = cml_new_expr();
        cml_parse_binop(pa, rhs);

        
        struct cml_binop_node root = {
            .lhs = lhs,
            .op = op,
            .rhs = rhs
        };

        expr->type = lhs->type != NULL ? lhs->type : rhs->type;
        expr->kind = EK_BINOP;
        expr->binop = cml_new_binop();
        *expr->binop = root;
    }
    else
    {
        *expr = *lhs;
        free(lhs);
    }
}

void cml_print_binop(struct cml_binop_node *binop, uint32_t depth)
{
    const char *v = binop->op.value;
    uint32_t l = binop->op.row;
    uint32_t c = binop->op.col;
    
    nputchar(' ', depth * 2);
    printf("EK_BINOP: (%u, %u) \"%s\" {\n", l, c, v);

    cml_print_expr(binop->lhs, depth + 1);
    cml_print_expr(binop->rhs, depth + 1);

    nputchar(' ', depth * 2);
    printf("}\n");
}

void cml_free_binop(struct cml_binop_node *root)
{
    cml_free_expr(root->lhs);
    free(root->lhs);
    cml_free_expr(root->rhs);
    free(root->rhs);
}
