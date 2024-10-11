#ifndef CML_AST_MODULE_H
#define CML_AST_MODULE_H

#include <stdio.h>
#include "../utils/bth_dynarray.h"
#include "cml_parser.h"

struct cml_module_node
{
    const char *name;
    const char *path;
    struct bth_dynarray exprs;
};

struct cml_module_node *cml_new_module(void);
void cml_parse_module(struct cml_parser *pa, struct cml_module_node *mod);
void cml_emit_module(struct cml_module_node *mod, FILE *file);
void cml_print_module(struct cml_module_node *mod, FILE *file);
void cml_free_module(struct cml_module_node *mod);

#endif
