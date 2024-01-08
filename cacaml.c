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
    /* struct cml_int32 = {} */
    struct cml_ast left = {
        .kind = AST_FUNCTION,
        .kids = NULL,
        .count = 0,
        .value = &(struct cmlenv_callable){.id=42},
    };

    struct cml_ast ast = {
        .kind = AST_OPERAND,
        .kids = &left,
        .count = 1,
        .value = &(struct cmlenv_i32){.value=42069},
    };

    struct bth_cstr *sast = cml_ast2cstr(&ast);
    LOGX("%s\n", sast->data);
    LOGX("%zu\n", sast->len);
    bth_cstr_free(sast);

    return 0;
}
