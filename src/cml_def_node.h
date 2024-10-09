#ifndef CML_AST_DEF_H
#define CML_AST_DEF_H

#include "cml_let_node.h"

enum cml_dkind
{
    DK_NONE,
    DK_BINDING,
};

struct cml_def_node
{
    enum cml_dkind kind;
    union {
        struct cml_let_binding_node *binding;
    };
};

struct cml_def_node *cml_new_def(void);
void cml_free_def(struct cml_def_node *root);

#endif
