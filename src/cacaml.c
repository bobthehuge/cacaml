// #include <stdio.h>

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

enum cml_ikind
{
    IK_DECL,
    IK_LOCAL_DECL,
    IK_COND,
    IK_EXPR,
};

enum cml_ekind
{
    EK_NONE,
    EK_INT32,
    EK_IDENT,
    EK_UNOP,
    EK_BINOP,
};

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
        struct cml_info_node ident;
        struct cml_info_node int32;
        // struct cml_unop_node unop;
        struct cml_binop_node binop;
    };
};

struct cml_block_node
{
    const char *type;
    struct bth_dynarray exprs;
};

struct cml_decl_node
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

struct cml_instr_node
{
    enum cml_ikind kind;
    union {
        struct cml_decl_node decl;
        struct cml_cond_node cond;
        struct cml_expr_node expr;
    };
};

struct cml_program_node
{
    const char *filename;
    struct bth_dynarray instrs;
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

    bth_dynarray_get(&pa->tokens, pa->idx + 1, &p);
}

void cml_parser_init(struct cml_parser *pa)
{
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
        fact->kind = EK_INT32;
        fact->int32.value = pa->curr.value;
    }
    else if (pa->curr.kind == TK_IDENT)
    {
        fact->kind = EK_IDENT;
        fact->ident.value = pa->curr.value;
    }
    else
    {
        ERROR(1, "Unexpected token of kind %s", cml_tkind2str(pa->curr.kind));
    }

    cml_parser_next(pa);
}

void cml_parse_term(struct cml_parser *pa, struct cml_binop_node *term)
{
    // struct cml_token next;

    struct cml_expr_node *lhs = smalloc(sizeof(struct cml_expr_node));
    cml_parse_factor(pa, lhs);

    // cml_parser_peek(pa, &next);

    // Term = Factor | Factor (* | /) Expr
    if (pa->curr.kind == TK_MUL)
    {
        // cml_parser_next(pa); // TK_MUL | TK_DIV
        cml_parser_next(pa); // rhs Expr

        struct cml_expr_node *rhs = smalloc(sizeof(struct cml_expr_node));
        cml_parse_expr(pa, rhs);

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

void cml_parse_expr(struct cml_parser *pa, struct cml_expr_node *expr)
{
    // struct cml_token next;

    expr->type = NULL;
    expr->kind = EK_NONE;

    struct cml_expr_node *lhs = smalloc(sizeof(struct cml_expr_node));
    cml_parse_term(pa, &lhs->binop);

    // cml_parser_peek(pa, &next);

    // Expr = Term | Term (+ | -) Expr
    if (pa->curr.kind == TK_ADD)
    {
        // cml_parser_next(pa); // TK_ADD | TK_SUB
        cml_parser_next(pa); // rhs Expr

        struct cml_expr_node *rhs = smalloc(sizeof(struct cml_expr_node));
        cml_parse_expr(pa, rhs);

        struct cml_info_node op = {
            .value = "+",
        };
        
        struct cml_binop_node root = {
            .lhs = lhs,
            .op = op,
            .rhs = rhs
        };

        // TODO: expr->type = ??
        expr->kind = EK_BINOP;
        expr->binop = root;
    }
    else
    {
        if (lhs->binop.op.value != NULL)
            *expr = *lhs;
        else
            *expr = *lhs->binop.lhs;
    }
}

void cml_parse_decl(struct cml_parser *pa, struct cml_instr_node *inode)
{
    struct cml_decl_node node;

    cml_parser_next(pa);
    if (pa->curr.kind != TK_IDENT)
        ERROR(1, "Unexpected token of kind %s", cml_tkind2str(pa->curr.kind));

    node.name.value = pa->curr.value;
    node.name.row = 0;
    node.name.col = 0;

    cml_parser_next(pa);
    if (pa->curr.kind != TK_EQ)
        ERROR(1, "Unexpected token of kind %s", cml_tkind2str(pa->curr.kind));

    cml_parser_next(pa);
    cml_parse_expr(pa, &node.body);
    
    inode->kind = IK_DECL;

    if (pa->curr.kind == TK_IN)
    {
        cml_parser_next(pa);
        inode->kind = IK_LOCAL_DECL;
    }
    
    inode->decl = node;
}

void cml_parse_instr(struct cml_parser *pa, struct cml_instr_node *inode)
{
    switch (pa->curr.kind)
    {
    case TK_LET:
        // LET IDENT EQ <EXPR> IN?
        cml_parse_decl(pa, inode);
        break;
    case TK_IF:
        // IF <EXPR> THEN <INSTR> ELSE? <INSTR?>
        // cml_parse_cond(pa, inode);
        TODO();
        break;
    case TK_IDENT: case TK_INT32: case TK_ADD:
        // return associated expr
        cml_parse_expr(pa, &inode->expr);
        break;
    case END:
        // do nothing
        break;
    default:
        ERROR(1, "Unexpected token of kind %s", cml_tkind2str(pa->curr.kind));
        break;
    }
}

void cml_parse_program(struct cml_parser *pa, struct cml_program_node *prog)
{
    prog->instrs = bth_dynarray_init(sizeof(struct cml_instr_node), 0);

    while (pa->curr.kind != END)
    {
        struct cml_instr_node inode;
        cml_parse_instr(pa, &inode);
        bth_dynarray_append(&prog->instrs, &inode);
    }
}

void cml_print_expr(struct cml_expr_node *expr)
{
    fputs("", stdout);
    switch (expr->kind)
    {
    case EK_BINOP:
        break;
    case EK_IDENT:
        break;
    case EK_INT32:
        break;
    case EK_UNOP:
        break;
    case EK_NONE:
        break;
    }
}

void cml_print_program(struct cml_program_node *prog)
{
    struct cml_instr_node instr;

    for (uint32_t i = 0; i < prog->instrs.len; i++)
    {
        bth_dynarray_get(&prog->instrs, i, &instr);

        switch (instr.kind)
        {
        case IK_DECL: case IK_LOCAL_DECL:
            break;
        case IK_COND:
            break;
        case IK_EXPR:
            cml_print_expr(&instr.expr);
            break;
        }
    }
}

int main()
{
    struct cml_lexer lex;

    int code = cml_lexer_from_file(&lex, "sample.ml");
    if (code)
        ERROR(1, "Unsuccessful lexer initialisation");

    escprints(lex.buf);
    printf("\n");

    struct cml_parser pa = {
        .tokens = cml_lexer_lexall(&lex),
        // .curr = TOKEN_EMPTY,
        .idx = 0,
    };

    for (uint32_t i = 0; i < pa.tokens.len; i++)
    {
        struct cml_token tok;
        bth_dynarray_get(&pa.tokens, i, &tok);
        LOGX(
            "{ .kind: %s, .value: \"%s\" }",
            cml_tkind2str(tok.kind),
            tok.value
        );
    }

    cml_parser_init(&pa);

    struct cml_program_node prog;
    cml_parse_program(&pa, &prog);
    
    cml_lexer_destroy(&lex);
    return 0;
}
