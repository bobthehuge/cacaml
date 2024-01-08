#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils/cml_log.h"
#include "cml_parser/cml_parser.h"
#include "cml_parser/cml_serialize_ast.h"

#define BTH_CSTR_IMPLEMENTATION
#include "utils/bth_cstr.h"

int main(void)
{
    struct cml_ast kids[] = {
        {
            .kind = CML_AST_OPERAND,
            .kids = NULL,
            .count = 0,
            .value = &(struct cmlenv_i32){.value=-42}
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

    struct bth_cstr *sast = cml_ast2cstr(&ast);
    LOGX("%s", sast->data);
    LOGX("%zu", sast->len);
    bth_cstr_free(sast);

    return 0;
}
