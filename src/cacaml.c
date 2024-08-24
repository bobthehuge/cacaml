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

enum cml_type
{
    UNRESOLVED,
    INT32,
};

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

const char *cml_ekind2str(enum cml_ekind v)
{
    switch (v)
    {
    case EK_NONE: return STRINGIFY(EK_NONE);
    case EK_INT32: return STRINGIFY(EK_INT32);
    case EK_IDENT: return STRINGIFY(EK_IDENT);
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
        struct cml_info_node *ident;
        struct cml_info_node *int32;
        // struct cml_unop_node unop;
        struct cml_binop_node *binop;
        // struct cml_decl_node *decl;
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
        fact->int32 = smalloc(sizeof(struct cml_info_node));
        fact->type = cml_type2str(INT32);
        fact->kind = EK_INT32;
        fact->int32->value = pa->curr.value;
        fact->int32->row = 0;
        fact->int32->col = 0;
    }
    else if (pa->curr.kind == TK_IDENT)
    {
        fact->ident = smalloc(sizeof(struct cml_info_node));
        fact->type = "unresolved";
        fact->kind = EK_IDENT;
        fact->ident->value = pa->curr.value;
        fact->int32->row = 0;
        fact->int32->col = 0;
    }
    else
    {
        ERROR(1, "Unexpected token of kind %s", cml_tkind2str(pa->curr.kind));
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

void cml_parse_expr(struct cml_parser *pa, struct cml_expr_node *expr)
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
        expr->binop = smalloc(sizeof(struct cml_binop_node));
        *expr->binop = root;
    }
    else
    {
        *expr = *lhs;
        free(lhs);
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
        inode->kind = IK_EXPR;
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

void nprintc(char c, unsigned int n)
{
    for (uint32_t i = 0; i < n; i++)
        putchar(c);
}

void cml_print_expr(struct cml_expr_node *expr, uint32_t depth);

void cml_print_binop(struct cml_binop_node *binop, uint32_t depth)
{
    const char *v = binop->op.value;
    uint32_t l = binop->op.row;
    uint32_t c = binop->op.col;

    printf("(l: %u, c:%u) \"%s\" {\n", l, c, v);
    cml_print_expr(binop->lhs, depth + 1);
    cml_print_expr(binop->rhs, depth + 1);
    nprintc(' ', depth * 2);
    printf("}\n");
}

void cml_print_expr(struct cml_expr_node *expr, uint32_t depth)
{
    nprintc(' ', depth * 2);

    printf(
        "IK_EXPR -> %s {\n",
        expr->type
    );

    nprintc(' ', (depth + 1) * 2);
    
    printf(
        "%s: ",
        cml_ekind2str(expr->kind)
    );

    switch (expr->kind)
    {
    case EK_BINOP:
        cml_print_binop(expr->binop, depth+1);
        break;
    case EK_IDENT: case EK_INT32:
    {
        const char *v;
        uint32_t l;
        uint32_t c;
        
        if (expr->kind == EK_IDENT)
        {
            v = expr->ident->value;
            l = expr->ident->row;
            c = expr->ident->col;
        }
        else
        {
            v = expr->int32->value;
            l = expr->int32->row;
            c = expr->int32->col;
        }

        printf("(l: %u, c:%u) \"%s\"\n", l, c, v);
    }
        break;
    case EK_UNOP:
        printf("EK_UNOP\n");
        break;
    case EK_NONE:
        printf("EK_NONE\n");
        break;
    }

    nprintc(' ', depth * 2);
    printf("}\n");
}

void cml_print_program(struct cml_program_node *prog)
{
    struct cml_instr_node instr;

    printf("-- PROGRAM BEGIN --\n");

    for (uint32_t i = 0; i < prog->instrs.len; i++)
    {
        bth_dynarray_get(&prog->instrs, i, &instr);

        switch (instr.kind)
        {
        case IK_DECL: case IK_LOCAL_DECL:
            printf("IK_DECL -> %s {\n", instr.decl.body.type);
            cml_print_expr(&instr.decl.body, 1);
            printf("}\n");
            break;
        case IK_COND:
            printf("IK_COND\n");
            break;
        case IK_EXPR:
            cml_print_expr(&instr.expr, 0);
            break;
        }
    }

    printf("-- PROGRAM END --\n");
}

// kidS because expr can be parent of multiple instr
void cml_free_expr_kids(struct cml_expr_node *expr)
{
    switch (expr->kind)
    {
    case EK_NONE:
        break;
    case EK_INT32:
        free(expr->int32);
        break;
    case EK_IDENT:
        free(expr->ident);
        break;
    case EK_UNOP:
        break;
    case EK_BINOP:
        cml_free_expr_kids(expr->binop->lhs);
        cml_free_expr_kids(expr->binop->rhs);
        free(expr->binop->lhs);
        free(expr->binop->rhs);
        free(expr->binop);
        break;
    }
}

void cml_free_program(struct cml_program_node *prog)
{
    struct cml_instr_node instr;

    for (uint32_t i = 0; i < prog->instrs.len; i++)
    {
        bth_dynarray_get(&prog->instrs, i, &instr);

        switch (instr.kind)
        {
        case IK_DECL: case IK_LOCAL_DECL:
            cml_free_expr_kids(&instr.decl.body);
            break;
        case IK_EXPR:
            cml_free_expr_kids(&instr.expr);
            break;
        default:
            break;
        }
    }

    free(prog->instrs.items);
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
