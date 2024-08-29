// #include <stdio.h>

#include "cml_ast_module.h"
#include "cml_lexer.h"
#include "cml_parser.h"
#include "cml_token.h"
#include "common.h"

#define BTH_CSTR_IMPLEMENTATION
#include "../utils/bth_log.h"
#define BTH_SALLOC_IMPLEMENTATION
#include "../utils/bth_salloc.h"
#define BTH_DYNARRAY_IMPLEMENTATION
#include "../utils/bth_dynarray.h"

int main()
{
    struct cml_lexer lex;

    int code = cml_lexer_from_file(&lex, "../sample.ml");
    if (code)
        ERROR(1, "Unsuccessful lexer initialisation");

    // escprints(lex.buf);
    // printf("\n");

    struct cml_parser pa = {
        .tokens = cml_lexer_lexall(&lex),
        .curr = TOKEN_EMPTY,
        .idx = 0,
    };

    // for (uint32_t i = 0; i < pa.tokens.len; i++)
    // {
    //     struct cml_token tok;
    //     bth_dynarray_get(&pa.tokens, i, &tok);
    //     LOGX(
    //         "{ .kind: %s, .value: \"%s\" }",
    //         cml_tkind2str(tok.kind),
    //         tok.value
    //     );
    // }

    cml_parser_init(&pa);

    struct cml_module_node mod;
    cml_parse_module(&pa, &mod);
    
    cml_print_module(&mod);

    cml_lexer_destroy(&lex);
    cml_free_parser(&pa);
    cml_free_module(&mod);

    return 0;
}
