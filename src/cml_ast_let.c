#include "../utils/bth_salloc.h"
#include "cml_ast_let.h"

// struct cml_let_binding_node *cml_new_let_binding(void)
// {
//     return smalloc(sizeof(struct cml_let_binding_node));
// }

// void cml_print_let_binding(struct cml_let_binding_node *root, uint32_t padd)
// {
// }

// void cml_parse_let_binding(struct cml_parser *pa, struct cml_expr_node *root)
// {
// }

// void cml_free_let_binding(struct cml_let_binding_node *root)
// {
//     return;
// }

struct cml_let_node *cml_new_let(void)
{
    return smalloc(sizeof(struct cml_let_node));
}

void cml_parse_let(struct cml_parser *pa, struct cml_expr_node *root)
{
    struct cml_let_node *let = cml_new_let();
    // let->body = NULL;

    let->binding.name.row = pa->curr.row;
    let->binding.name.col = pa->curr.col;
    let->body.kind = EK_NONE;

    cml_parser_next(pa);
    if (pa->curr.kind != TK_IDENT)
        ERR_UNEXP_TOKEN(&pa->curr);

    let->binding.name.value = pa->curr.value;

    cml_parser_next(pa);
    if (pa->curr.kind != TK_EQ)
        ERR_UNEXP_TOKEN(&pa->curr);

    cml_parser_next(pa);

    cml_parse_expr(pa, &let->binding.body);

    root->type = let->binding.body.type;

    if (pa->curr.kind == TK_IN)
    {
        // let->body = cml_new_expr();
        cml_parser_next(pa);
        cml_parse_expr(pa, &let->body);
        root->type = let->body.type;
    }

    root->kind = EK_LET;
    root->let = let;
}

void cml_print_let(struct cml_let_node *r, uint32_t depth)
{
    nputchar(' ', depth * 2);
    
    printf(
        "EK_LET: (%u, %u) \"%s\" -> %s {\n",
        r->binding.name.row,
        r->binding.name.col,
        r->binding.name.value,
        r->binding.body.type
    );

    cml_print_expr(&r->binding.body, depth + 1);

    if (r->body.kind != EK_NONE)
        cml_print_expr(&r->body, depth + 1);

    nputchar(' ', depth * 2);
    printf("}\n");
}

void cml_free_let(struct cml_let_node *root)
{
    return;
}
