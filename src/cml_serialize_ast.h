#ifndef CML_SERIALIZE_AST_H
#define CML_SERIALIZE_AST_H

#include "../utils/bth_cstr.h"
#include "cml_lexer.h"

struct bth_cstr *cml_ast2cstr(struct cml_token *ast);

#endif
