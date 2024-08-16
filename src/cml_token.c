#include "common.h"
#include "../utils/bth_cstr.h"
#include "cml_token.h"

const char *cml_tkind2str(enum cml_tkind kind)
{
    switch (kind)
    {
    case END: return STRINGIFY(END);
    case TK_INVALID: return STRINGIFY(TK_INVALID);
    case TK_INT32: return STRINGIFY(TK_INT32);
    case TK_ADD: return STRINGIFY(TK_ADD);
    case TK_EQ: return STRINGIFY(TK_EQ);
    case TK_LET: return STRINGIFY(TK_LET);
    case TK_IF: return STRINGIFY(TK_IF);
    case TK_IDENT: return STRINGIFY(TK_IDENT);
    }
}
