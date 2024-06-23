#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CML_TOKEN_MACROS
#include "cml_lexer.h"
#include "cml_parser.h"
#include "cml_serialize_ast.h"

#define BTH_CSTR_IMPLEMENTATION
#include "../utils/bth_cstr.h"
#include "../utils/bth_log.h"

void cml_eval_cstr(struct bth_cstr *cstr)
{
    char **args = malloc(3 * sizeof(char *));
    args[0] = "../eval/eval.py";
    args[1] = cstr->data;
    args[2] = NULL;

    printf("[LOG] eval.py: ");
    fflush(0);

    if (execvp(args[0], args) == -1)
    {
        ERR(1, "An error occured during CSTR evaluation");
    }
}

void cml_eval_main(void)
{
    struct cml_token kids[] = {
        CML_TOKEN_I32(42),
        CML_TOKEN_I32(-22)
    };

    struct cml_token ast = {
        .kind = CML_TOK_BINOP,
        .kids = (struct cml_token **)&kids,
        .count = 2,
        .value = &(struct cml_func_t){.fid=0,.name="+"}
    };

    /* struct cml_ast *ast = cml_build_ast(NULL); */
    struct bth_cstr *sast = cml_ast2cstr(&ast);

    LOGX("%s", sast->data);
    LOGX("%zu", sast->len);

    cml_eval_cstr(sast);

    bth_cstr_free(sast);
}

void cml_parse_main(void)
{
    struct cml_token s1[] = {
        CML_TOKEN_I32(42),
        CML_TOKEN_BINOP("+"),
        CML_TOKEN_I32(-22),
        CML_TOKEN_NULL,
    };

    size_t count = sizeof(s1) / sizeof(struct cml_token) - 1;

    cml_build_ast(s1, count);

    struct cml_token *head = s1[count].kids[0];

    struct bth_cstr *sast = cml_ast2cstr(head);

    LOGX("%s", sast->data);
    LOGX("%zu", sast->len);

    // cml_eval_cstr(sast);

    bth_cstr_free(sast);
}

int main(void)
{
    // cml_eval_main();
    cml_parse_main();
    return 0;
}
