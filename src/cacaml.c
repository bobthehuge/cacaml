// #include <stdio.h>

#include <stdint.h>
#include <stdio.h>
#include "cml_lexer.h"
#include "cml_token.h"
#include "common.h"

#define BTH_CSTR_IMPLEMENTATION
#include "../utils/bth_log.h"
#define BTH_SALLOC_IMPLEMENTATION
#include "../utils/bth_salloc.h"
#define BTH_DYNARRAY_IMPLEMENTATION
#include "../utils/bth_dynarray.h"

#define ERR_UNEXP_TOKEN_FMT "Unexpected token of kind %s at (%u, %u)"
#define ERR_UNEXP_TOKEN(t) \
    ERROR(\
        1,\
        ERR_UNEXP_TOKEN_FMT,\
        cml_tkind2str((t)->kind),\
        (t)->row,\
        (t)->col\
    );

enum cml_type
{
    UNRESOLVED,
    INT32,
};

enum cml_ekind
{
    EK_NONE,
    EK_VALUE_NAME,
    EK_CONSTANT,
    EK_EXPR,
    EK_LET,
    EK_UNOP,
    EK_BINOP,
};

const char *cml_ekind2str(enum cml_ekind v)
{
    switch (v)
    {
    case EK_NONE: return STRINGIFY(EK_NONE);
    case EK_VALUE_NAME: return STRINGIFY(EK_VALUE_NAME);
    case EK_CONSTANT: return STRINGIFY(EK_CONSTANT);
    case EK_EXPR: return STRINGIFY(EK_EXPR);
    case EK_LET: return STRINGIFY(EK_LET);
    case EK_UNOP: return STRINGIFY(EK_UNOP);
    case EK_BINOP: return STRINGIFY(EK_BINOP);
    }

    return "";
}

const char *cml_type2str(enum cml_type t)
{
    switch (t)
    {
    case UNRESOLVED: return STRINGIFY(UNRESOLVED);
    case INT32: return STRINGIFY(INT32);
    }

    return "";
}

struct cml_info_node
{
    char *value;
    uint32_t row;
    uint32_t col;
};

// struct cml_unop_node
// {
//     struct cml_info_node op;
//     struct cml_expr_node *expr;
// };

struct cml_binop_node
{
    struct cml_info_node op;
    struct cml_expr_node *lhs;
    struct cml_expr_node *rhs;
};

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

struct cml_block_node
{
    const char *type;
    struct bth_dynarray exprs;
};

struct cml_let_node
{
    struct cml_info_node name;
    struct cml_expr_node body;
};

struct cml_cond_node
{
    struct cml_info_node node;
    struct cml_expr_node pred;
    struct cml_expr_node b_then;
    struct cml_expr_node b_else;
};

struct cml_program_node
{
    const char *filename;
    struct bth_dynarray exprs;
};

struct cml_parser
{
    struct bth_dynarray tokens;
    struct cml_token curr;
    uint32_t idx;
};

void cml_parser_current(struct cml_parser *pa, struct cml_token *p)
{
    if (pa->idx >= pa->tokens.len - 1)
    {
        p->kind = END;
        p->value = NULL;
        return;
    }

    bth_dynarray_get(&pa->tokens, pa->idx, p);
}

void cml_parser_peek(struct cml_parser *pa, struct cml_token *p)
{
    if (pa->idx >= pa->tokens.len - 2)
    {
        p->kind = END;
        p->value = NULL;
        return;
    }

    bth_dynarray_get(&pa->tokens, pa->idx + 1, p);
}

void cml_parser_init(struct cml_parser *pa)
{
    pa->idx = 0;
    cml_parser_current(pa, &pa->curr);
}

void cml_parser_next(struct cml_parser *pa)
{
    // out of bound is already handled;
    pa->idx++;
    cml_parser_current(pa, &pa->curr);
}

void cml_parse_expr(struct cml_parser *pa, struct cml_expr_node *expr);

void cml_parse_factor(struct cml_parser *pa, struct cml_expr_node *fact)
{
    if (pa->curr.kind == TK_INT32)
    {
        fact->int32 = smalloc(sizeof(struct cml_info_node));
        fact->type = cml_type2str(INT32);
        fact->kind = EK_CONSTANT;
        fact->int32->value = pa->curr.value;
        fact->int32->row = pa->curr.row;
        fact->int32->col = pa->curr.col;
    }
    else if (pa->curr.kind == TK_IDENT)
    {
        fact->value = smalloc(sizeof(struct cml_info_node));
        fact->type = "unresolved";
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
    struct cml_expr_node *lhs = smalloc(sizeof(struct cml_expr_node));
    cml_parse_factor(pa, lhs);

    // Term = Factor | Factor (* | /) Expr
    if (pa->curr.kind == TK_MUL)
    {
        cml_parser_next(pa); // rhs Expr

        struct cml_expr_node *rhs = smalloc(sizeof(struct cml_expr_node));
        rhs->binop = smalloc(sizeof(struct cml_binop_node));
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
        
        // expr->type = TODO
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

    struct cml_expr_node *lhs = smalloc(sizeof(struct cml_expr_node));
    lhs->type = cml_type2str(UNRESOLVED);
    lhs->binop = smalloc(sizeof(struct cml_binop_node));
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

        struct cml_expr_node *rhs = smalloc(sizeof(struct cml_expr_node));
        cml_parse_binop(pa, rhs);

        
        struct cml_binop_node root = {
            .lhs = lhs,
            .op = op,
            .rhs = rhs
        };

        // TODO: expr->type = ??
        expr->kind = EK_BINOP;
        expr->binop = smalloc(sizeof(struct cml_binop_node));
        *expr->binop = root;
    }
    else
    {
        *expr = *lhs;
        free(lhs);
    }
}

void cml_parse_let(struct cml_parser *pa, struct cml_expr_node *root)
{
    struct cml_let_node *let = smalloc(sizeof(struct cml_let_node));

    let->name.row = pa->curr.row;
    let->name.col = pa->curr.col;

    cml_parser_next(pa);
    if (pa->curr.kind != TK_IDENT)
        ERR_UNEXP_TOKEN(&pa->curr);

    let->name.value = pa->curr.value;

    cml_parser_next(pa);
    if (pa->curr.kind != TK_EQ)
        ERR_UNEXP_TOKEN(&pa->curr);

    cml_parser_next(pa);
    if (pa->curr.kind == TK_LET)
    {
        // struct cml_let_node *let = smalloc(sizeof(struct cml_let_node));
    }
    else
    {
        cml_parse_expr(pa, &let->body);
        root->kind = EK_LET;
        root->let = let;
    }
   
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

void cml_parse_program(struct cml_parser *pa, struct cml_program_node *prog)
{
    prog->exprs = bth_dynarray_init(sizeof(struct cml_expr_node), 0);

    while (pa->curr.kind != END)
    {
        struct cml_expr_node expr;
        cml_parse_expr(pa, &expr);
        bth_dynarray_append(&prog->exprs, &expr);
    }
}

void nputchar(char c, unsigned int n)
{
    for (uint32_t i = 0; i < n; i++)
        putchar(c);
}

void cml_print_expr(struct cml_expr_node *expr, uint32_t depth);

void __print_binop(struct cml_binop_node *binop, uint32_t depth)
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

void __print_info(struct cml_info_node *r, uint32_t depth)
{
    nputchar(' ', depth * 2);
    printf("EK_INFO: (%u, %u) \"%s\"\n", r->row, r->col, r->value);
}

void __print_let(struct cml_let_node *r, uint32_t depth)
{
    printf(
        "EK_LET: (%u, %u) \"%s\" -> %s {\n",
        r->name.row,
        r->name.col,
        r->name.value,
        r->body.type
    );

    cml_print_expr(&r->body, depth + 1);
    printf("}\n");
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
        __print_info(expr->value, depth);
        break;
    case EK_CONSTANT:
        __print_info(expr->int32, depth);
        break;
    case EK_EXPR:
        cml_print_expr(expr->expr, depth + 1);
        break;
    case EK_LET:
        __print_let(expr->let, depth);
        break;
    case EK_UNOP:
        nputchar(' ', depth * 2);
        printf("EK_UNOP\n");
        break;
    case EK_BINOP:
        __print_binop(expr->binop, depth);
        break;
    }
}

void cml_print_program(struct cml_program_node *prog)
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

void cml_free_program(struct cml_program_node *prog)
{
    struct cml_expr_node expr;

    for (uint32_t i = 0; i < prog->exprs.len; i++)
    {
        bth_dynarray_get(&prog->exprs, i, &expr);
        cml_free_expr(&expr);
    }

    free(prog->exprs.items);
}

void cml_free_parser(struct cml_parser *pa)
{
    struct cml_token tok;

    for (uint32_t i = 0; i < pa->tokens.len; i++)
    {
        bth_dynarray_get(&pa->tokens, i, &tok);

        switch (tok.kind)
        {
        case TK_INT32: case TK_IDENT: case TK_INVALID:
            free(tok.value);
            break;
        default:
            break;
        }
    }

    free(pa->tokens.items);
}

int main()
{
    struct cml_lexer lex;

    int code = cml_lexer_from_file(&lex, "sample.ml");
    if (code)
        ERROR(1, "Unsuccessful lexer initialisation");

    // escprints(lex.buf);
    // printf("\n");

    struct cml_parser pa = {
        .tokens = cml_lexer_lexall(&lex),
        .curr = TOKEN_EMPTY,
        .idx = 0,
    };

    // for (uint32_t i = 0; i < pa.tokens.len; i++)
    // {
    //     struct cml_token tok;
    //     bth_dynarray_get(&pa.tokens, i, &tok);
    //     LOGX(
    //         "{ .kind: %s, .value: \"%s\" }",
    //         cml_tkind2str(tok.kind),
    //         tok.value
    //     );
    // }

    cml_parser_init(&pa);

    struct cml_program_node prog;
    cml_parse_program(&pa, &prog);
    
    cml_print_program(&prog);

    cml_lexer_destroy(&lex);
    cml_free_parser(&pa);
    cml_free_program(&prog);

    return 0;
}
