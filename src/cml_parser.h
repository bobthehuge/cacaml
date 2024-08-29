#ifndef CML_PARSER_H
#define CML_PARSER_H

#include <stdint.h>

#include "../utils/bth_dynarray.h"
#include "cml_lexer.h"
#include "common.h"

#define ERR_UNEXP_TOKEN_FMT "Unexpected token of kind %s at (%u, %u)"
#define ERR_UNEXP_TOKEN(t) \
    ERROR(\
        1,\
        ERR_UNEXP_TOKEN_FMT,\
        cml_tkind2str((t)->kind),\
        (t)->row,\
        (t)->col\
    );

struct cml_parser
{
    struct bth_dynarray tokens;
    struct cml_token curr;
    uint32_t idx;
};

void cml_parser_init(struct cml_parser *pa);
void cml_parser_peek(struct cml_parser *pa, struct cml_token *p);
void cml_parser_next(struct cml_parser *pa);
void cml_free_parser(struct cml_parser *pa);

#endif
