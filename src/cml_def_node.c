#include "../utils/bth_salloc.h"
#include "cml_def_node.h"

struct cml_def_node *cml_new_def(void)
{
    return smalloc(sizeof(struct cml_def_node));
}

void cml_free_def(struct cml_def_node *root)
{
    if (root->kind == DK_BINDING)
    {
        cml_free_binding(root->binding);
        free(root->binding);
    }
}
