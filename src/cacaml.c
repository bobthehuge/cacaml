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
    EK_VALUE,
    EK_UNOP,
    EK_BINOP,
};

struct cml_info_node
{
    char *value;
    uint32_t line;
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

struct cml_decl_node
{
    struct cml_info_node name;
    struct cml_expr_node value;
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

    bth_dynarray_get(&pa->tokens, pa->idx, &p);
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

void cml_parse_binop_sides(struct cml_parser *pa, struct cml_binop_node *bnode)
{
    struct cml_token tok = {0};

    if (pa->idx == 0)
        ERROR(1, "Missing operand for %s", bnode->op.value);

    cml_parser_peek(pa, &tok);
}

void cml_parse_factor(struct cml_parser *pa, struct cml_info_node *fact)
{
}

void cml_parse_term(struct cml_parser *pa, struct cml_binop_node *term)
{
}

void cml_parse_expr(struct cml_parser *pa, struct cml_expr_node *expr)
{
    struct cml_token next = {0};
    cml_parser_peek(pa, &next);

    if (next.kind == TK_ADD)
}

void cml_parse_decl(struct cml_parser *pa, struct cml_instr_node *inode)
{
    struct cml_decl_node node = {0};

    cml_parser_next(pa);
    if (pa->curr.kind != TK_IDENT)
        ERROR(1, "Unexpected token of kind %s", cml_tkind2str(pa->curr.kind));

    node.name.value = pa->curr.value;
    node.name.line = 0;
    node.name.col = 0;

    cml_parser_next(pa);
    if (pa->curr.kind != TK_EQ)
        ERROR(1, "Unexpected token of kind %s", cml_tkind2str(pa->curr.kind));

    cml_parser_next(pa);
    cml_parse_expr(pa, &node.value);
    
    inode->kind = pa->curr.kind == TK_IN ? IK_LOCAL_DECL : IK_DECL;
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
        cml_parse_cond(pa, inode);
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
        struct cml_instr_node inode = {0};
        cml_parse_instr(pa, &inode);
        bth_dynarray_append(&prog->instrs, &inode);
    }
}

int main()
{
    struct cml_lexer lex;

    int err = cml_lexer_from_file(&lex, "sample.ml");
    if (err)
        ERRX(1, "Unsuccessful lexer initialisation");

    escprints(lex.buf);
    printf("\n");

    struct cml_parser pa = {
        .tokens = cml_lexer_lexall(&lex),
        .curr = {0},
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

    struct cml_program_node prog = {0};
    cml_parse_program(&pa, &prog);
    
    cml_lexer_destroy(&lex);
    return 0;
}
