#ifndef CML_TAC_H
#define CML_TAC_H

enum CML_TAKIND
{
    TAK_ASSIGN_ADD,
    TAK_ASSIGN_MUL,
    TAK_ASSIGN_VAL,
};

struct cml_tac_value
{
    char *name;
    char *expr;
};

struct cml_tac_binop
{
    char *ident;
    char *lhs;
    char *rhs;
};

struct cml_tac_instr
{
    enum CML_TAKIND kind;
    union {
        cml_tac_value value;
        cml_tac_binop binop;
    };
};

#endif
