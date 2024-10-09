#include "../utils/bth_salloc.h"
#include "cml_binop_node.h"
#include "cml_let_node.h"
#include "cml_expr_node.h"
#include <cml_info_node.h>
#include <stdio.h>
#include "cml_parser.h"

struct cml_expr_node *cml_new_expr(void)
{
    return smalloc(sizeof(struct cml_expr_node));
}

void cml_parse_expr(struct cml_parser *pa, struct cml_expr_node *r)
{
    struct cml_token next;
    cml_parser_peek(pa, &next);

    if (next.kind == TK_ADD || next.kind == TK_MUL)
    {
        cml_parse_binop(pa, r);
    }
    else
    {
        if (pa->curr.kind == TK_IDENT || pa->curr.kind == TK_INT32)
        {
            cml_parse_factor(pa, r);
        }
        else if (pa->curr.kind == TK_LET)
        {
            cml_parse_let(pa, r);
        }
        else
        {
            ERR_UNEXP_TOKEN(&pa->curr);
        }
    }
}

void cml_print_expr(struct cml_expr_node *expr, uint32_t depth)
{
    switch (expr->kind)
    {
    case EK_NONE:
        nputchar(' ', depth * 2);
        printf("EK_NONE\n");
        break;
    case EK_VALUE_NAME:
        cml_print_info(expr->value, depth);
        break;
    case EK_CONSTANT:
        cml_print_info(expr->int32, depth);
        break;
    case EK_EXPR:
        cml_print_expr(expr->expr, depth + 1);
        break;
    case EK_LET:
        cml_print_let(expr->let, depth);
        break;
    case EK_UNOP:
        nputchar(' ', depth * 2);
        printf("EK_UNOP\n");
        break;
    case EK_BINOP:
        cml_print_binop(expr->binop, depth);
        break;
    }
}

void cml_free_expr(struct cml_expr_node *expr)
{
    switch (expr->kind)
    {
    case EK_NONE:
        break;
    case EK_VALUE_NAME:
        free(expr->value);
        break;
    case EK_CONSTANT:
        free(expr->int32);
        break;
    case EK_EXPR:
        cml_free_expr(expr->expr);
        free(expr->expr);
        break;
    case EK_LET:
        cml_free_expr(&expr->let->binding.body);

        cml_free_expr(&expr->let->body);
        
        free(expr->let);
        break;
    case EK_UNOP:
        TODO();
        break;
    case EK_BINOP:
        cml_free_expr(expr->binop->lhs);
        cml_free_expr(expr->binop->rhs);
        free(expr->binop->lhs);
        free(expr->binop->rhs);
        free(expr->binop);
        break;
    }
}
