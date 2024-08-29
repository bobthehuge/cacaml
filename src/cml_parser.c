#include <stdlib.h>

#include "cml_parser.h"
#include "common.h"

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

void cml_parser_init(struct cml_parser *pa)
{
    pa->idx = 0;
    cml_parser_current(pa, &pa->curr);
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

void cml_parser_next(struct cml_parser *pa)
{
    // out of bound is already handled;
    pa->idx++;
    cml_parser_current(pa, &pa->curr);
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
