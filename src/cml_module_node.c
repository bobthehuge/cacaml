#include <stdio.h>
#include <stdlib.h>

#include "../utils/bth_htab.h"
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

void cml_emit_module(struct cml_module_node *mod, FILE *file)
{
    struct cml_expr_node expr;
    struct cml_ir_ctx ctx = {
        .file = file,
        .vtable = htab_new(),
        .vcount = 0
    };

    fprintf(file, "function $%s() {\n@start\n", mod->name);

    for (uint32_t i = 0; i < mod->exprs.len; i++)
    {
        bth_dynarray_get(&mod->exprs, i, &expr);
        cml_emit_expr(&expr, &ctx);
    }

    fprintf(file, "    ret\n}\n\n");
}

void cml_print_module(struct cml_module_node *mod, FILE *file)
{
    struct cml_expr_node expr;

    fprintf(file, "MODULE %s {\n", mod->name);
    for (uint32_t i = 0; i < mod->exprs.len; i++)
    {
        bth_dynarray_get(&mod->exprs, i, &expr);
        cml_print_expr(&expr, 1, file);
    }
    fprintf(file, "}\n");
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
