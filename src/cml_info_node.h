#ifndef CML_AST_INFO
#define CML_AST_INFO

#include <stdint.h>
#include "cml_parser.h"
#include "cml_expr_node.h"

struct cml_info_node
{
    char *value;
    uint32_t row;
    uint32_t col;
};

struct cml_info_node *cml_new_info(void);
void cml_parse_info(struct cml_parser *pa, struct cml_expr_node *root);
void cml_print_info(struct cml_info_node *info, uint32_t padd);
// void cml_free_info(struct cml_info_node *root);

#endif
