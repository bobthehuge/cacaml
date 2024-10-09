#include <stdio.h>
#include <stdlib.h>

#include "cml_module_node.h"
#include "cml_expr_node.h"

void cml_parse_module(struct cml_parser *pa, struct cml_module_node *prog)
{
    prog->exprs = bth_dynarray_init(sizeof(struct cml_expr_node), 0);

    while (pa->curr.kind != END)
    {
        struct cml_expr_node expr;
        cml_parse_expr(pa, &expr);
        bth_dynarray_append(&prog->exprs, &expr);
    }
}

void cml_print_module(struct cml_module_node *prog)
{
    struct cml_expr_node expr;

    printf("-- PROGRAM BEGIN --\n");

    for (uint32_t i = 0; i < prog->exprs.len; i++)
    {
        bth_dynarray_get(&prog->exprs, i, &expr);
        cml_print_expr(&expr, 0);
    }

    printf("-- PROGRAM END --\n");
}

void cml_free_module(struct cml_module_node *mod)
{
    struct cml_expr_node expr;

    for (uint32_t i = 0; i < mod->exprs.len; i++)
    {
        bth_dynarray_get(&mod->exprs, i, &expr);
        cml_free_expr(&expr);
    }

    free(mod->exprs.items);
}
