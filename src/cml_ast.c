#include "../utils/bth_salloc.h"
#include "cml_ast.h"
#include "common.h"

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
    case UNRESOLVED: return STRINGIFY(UNRESOLVED); // NULL string
    case UNIT: return STRINGIFY(UNIT);
    case INT32: return STRINGIFY(INT32);
    }

    return "";
}
