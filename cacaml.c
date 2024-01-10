#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cml_parser/cml_parser.h"
#include "cml_parser/cml_serialize_ast.h"

#define BTH_CSTR_IMPLEMENTATION
#include "utils/bth_cstr.h"
#include "utils/cml_log.h"

void cml_eval_cstr(struct bth_cstr *cstr)
{
    char **args = malloc(3 * sizeof(char *));
    args[0] = "./eval/eval.py";
    args[1] = cstr->data;
    args[2] = NULL;

    printf("[LOG] eval.py: ");
    fflush(0);

    if (execvp(args[0], args) == -1)
    {
        ERR(1, "An error occured during CSTR evaluation");
    }
}

int main(void)
{
    struct cml_ast kids[] = {
        {
            .kind = CML_AST_OPERAND,
            .kids = NULL,
            .count = 0,
            .value = &(struct cmlenv_i32){.value=42}
        },
        {
            .kind = CML_AST_OPERAND,
            .kids = NULL,
            .count = 0,
            .value = &(struct cmlenv_i32){.value=-22}
        }
    };

    struct cml_ast ast = {
        .kind = CML_AST_FUNCTION,
        .kids = kids,
        .count = 2,
        .value = &(struct cmlenv_callable){.id=CML_ENV_ADD}
    };

    /* struct cml_ast *ast = cml_build_ast(NULL); */
    struct bth_cstr *sast = cml_ast2cstr(&ast);

    LOGX("%s", sast->data);
    LOGX("%zu", sast->len);

    cml_eval_cstr(sast);

    bth_cstr_free(sast);

    return 0;
}
