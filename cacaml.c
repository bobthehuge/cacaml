#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cml_log.h"
#include "cml_parser.h"

int main(void)
{
    /* struct cml_int32 = {} */
    struct cml_ast left = {
        .key = {
            .kind = T_FUNCTION,
            .value = &(struct cml_callable){.id=42},
        },
        .kids = NULL,
        .count = 0,
    };

    struct cml_ast ast = {
        .key = {
            .kind = T_OPERAND,
            .value = &(struct cml_i32){.value=42069},
        },
        .kids = &left,
        .count = 1,
    };

    char *sast;
    serialize_ast(&ast, &sast);
    LOGX("%s\n", sast);
    free(sast);

    return 0;
}
