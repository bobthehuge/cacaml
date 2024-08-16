// #include <stdio.h>

#include <stdio.h>
#include "cml_lexer.h"
#include "cml_token.h"
#include "common.h"

#define BTH_CSTR_IMPLEMENTATION
#include "../utils/bth_log.h"
#define BTH_SALLOC_IMPLEMENTATION
#include "../utils/bth_salloc.h"


int main()
{
    struct cml_lexer lex;

    int err = cml_lexer_from_file(&lex, "sample.ml");
    if (err)
        ERRX(1, "Unsuccessful lexer initialisation");

    escprints(lex.buf);
    printf("\n");

    struct cml_token tok = {0};

    do
    {
        tok = cml_lexer_next_token(&lex);
        LOGX(
            "{ .kind: %s, .value: \"%s\" }",
            cml_tkind2str(tok.kind),
            tok.value
        );
    } while (tok.kind != END);
    
    cml_lexer_destroy(&lex);
    return 0;
}
