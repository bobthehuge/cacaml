#include "../utils/bth_salloc.h"
#include "cml_info_node.h"
#include <cml_ast.h>
#include "common.h"

struct cml_info_node *cml_new_info(void)
{
    return smalloc(sizeof(struct cml_info_node));
}

// void cml_free_info(struct cml_info_node *root)
// {
//     return;
// }

void cml_print_info(struct cml_info_node *r, uint32_t padd)
{
    nputchar(' ', padd * 2);
    printf("EK_INFO: (%u, %u) \"%s\"\n", r->row, r->col, r->value);
}
