#include <stdio.h>
#include <stdlib.h>

#include "../utils/bth_salloc.h"
#include "cml_binop_node.h"
#include "cml_let_node.h"
#include "cml_expr_node.h"
#include "cml_ast.h"
#include "cml_info_node.h"

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

void cml_emit_expr(struct cml_expr_node *expr, struct cml_ir_ctx *ctx)
{
    switch (expr->kind)
    {
    case EK_NONE:
        break;
    case EK_VALUE_NAME: {
            struct htab_pair *pair = 
                htab_get(ctx->vtable, expr->value->value);

            if (pair == NULL)
                ERRX(1, "Unbound variable '%s'", expr->value->value);

            fprintf(
                ctx->file,
                "    %%s%zu =w copy %%%s\n",
                ctx->vcount,
                (char *)pair->value
            );
            ctx->vcount++;
        } break;
    case EK_CONSTANT: {
            fprintf(
                ctx->file,
                "    %%s%zu =w copy %s\n",
                ctx->vcount,
                expr->int32->value
            );
            ctx->vcount++;
        } break;
    case EK_EXPR:
        cml_emit_expr(expr, ctx);
        break;
    case EK_LET: {
            // let {binding.name} = s_{ctx->vcount - 1}
            cml_emit_expr(&expr->let->binding.body, ctx);

            struct htab_pair *pair = 
                htab_get(ctx->vtable, expr->let->binding.name.value);

            char *res;
            myasprintf(&res, "s%zu", ctx->vcount-1);

            // if {binding.name} is already bound...
            if (pair != NULL)
            {
                void *old_val = pair->value;
                pair->value = res;

                cml_emit_expr(&expr->let->body, ctx);

                // and assignment is local
                if (expr->let->body.kind != EK_NONE)
                {
                    free(res);
                    pair->value = old_val;
                }
                else
                {
                    free(old_val);
                }
            }
            else
            {
                htab_insert(ctx->vtable, expr->value->value, res);
                cml_emit_expr(&expr->let->body, ctx);
            }
        } break;
    case EK_UNOP:
        TODO();
        break;
    case EK_BINOP: {
            cml_emit_expr(expr->binop->lhs, ctx);
            cml_emit_expr(expr->binop->rhs, ctx);
            fprintf(
                ctx->file,
                "    %%s%zu =w %s %%s%zu, %%s%zu\n",
                ctx->vcount,
                expr->binop->op.value,
                ctx->vcount - 2,
                ctx->vcount - 1
            );
            ctx->vcount++;
        } break;
    }
}

void cml_print_expr(struct cml_expr_node *expr, uint32_t depth, FILE *file)
{
    switch (expr->kind)
    {
    case EK_NONE:
        nputchar(file, ' ', depth * 2);
        printf("EK_NONE\n");
        break;
    case EK_VALUE_NAME:
        cml_print_info(expr->value, depth, file);
        break;
    case EK_CONSTANT:
        cml_print_info(expr->int32, depth, file);
        break;
    case EK_EXPR:
        cml_print_expr(expr->expr, depth + 1, file);
        break;
    case EK_LET:
        cml_print_let(expr->let, depth, file);
        break;
    case EK_UNOP:
        nputchar(file, ' ', depth * 2);
        printf("EK_UNOP\n");
        break;
    case EK_BINOP:
        cml_print_binop(expr->binop, depth, file);
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
