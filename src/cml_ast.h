#ifndef CML_AST_H
#define CML_AST_H

// #include "cml_parser.h"

enum cml_type
{
    UNRESOLVED,
    UNIT,
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

// struct cml_unop_node
// {
//     struct cml_info_node op;
//     struct cml_expr_node *expr;
// };

// struct cml_block_node
// {
//     const char *type;
//     struct bth_dynarray exprs;
// };

const char *cml_ekind2str(enum cml_ekind v);
const char *cml_type2str(enum cml_type t);

#endif