#ifndef CML_LEXER_H
#define CML_LEXER_H

#include "cml_token.h"

int cml_lexer_from_file(struct cml_lexer *lex, char *path);
void cml_lexer_destroy(struct cml_lexer *lex);
struct cml_token cml_lexer_next_token(struct cml_lexer *lex);

#endif
