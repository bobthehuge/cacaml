#include "../utils/bth_salloc.h"
#include "cml_let_node.h"
#include <cml_ast.h>
#include <cml_expr_node.h>
#include <cml_info_node.h>

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

void cml_free_binding(struct cml_let_binding_node *root)
{
    cml_free_expr(&root->body);
}

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

void cml_print_let(struct cml_let_node *r, uint32_t padd, FILE *file)
{
    nputchar(file, ' ', padd * 2);
    
    fprintf(
        file,
        "EK_LET: (%u, %u) \"%s\" -> %s {\n",
        r->binding.name.row,
        r->binding.name.col,
        r->binding.name.value,
        r->binding.body.type
    );

    cml_print_expr(&r->binding.body, padd + 1, file);

    if (r->body.kind != EK_NONE)
        cml_print_expr(&r->body, padd + 1, file);

    nputchar(file, ' ', padd * 2);
    fprintf(file, "}\n");
}

void cml_free_let(struct cml_let_node *root)
{
    cml_free_binding(&root->binding);
    cml_free_expr(&root->body);
}
