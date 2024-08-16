// #include <stdio.h>

#include <stdio.h>
#include "cml_lexer.h"
#include "cml_token.h"
#include "common.h"

#define BTH_CSTR_IMPLEMENTATION
#include "../utils/bth_log.h"
#define BTH_SALLOC_IMPLEMENTATION
#include "../utils/bth_salloc.h"
#define BTH_DYNARRAY_IMPLEMENTATION
#include "../utils/bth_dynarray.h"

struct cml_program_node
{
    const char *filename;
    struct bth_dynarray nodes;
};

int main()
{
    struct cml_lexer lex;

    int err = cml_lexer_from_file(&lex, "sample.ml");
    if (err)
        ERRX(1, "Unsuccessful lexer initialisation");

    escprints(lex.buf);
    printf("\n");

    struct bth_dynarray tokens = cml_lexer_lexall(&lex);

    for (uint32_t i = 0; i < tokens.len; i++)
    {
        struct cml_token tok;
        bth_dynarray_get(&tokens, i, &tok);
        LOGX(
            "{ .kind: %s, .value: \"%s\" }",
            cml_tkind2str(tok.kind),
            tok.value
        );
    }
    
    cml_lexer_destroy(&lex);
    return 0;
}
