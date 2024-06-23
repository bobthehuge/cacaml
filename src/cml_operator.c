#include "../utils/bth_htab.h"
#include "cml_lexer.h"

static struct pair OPTABLE_CONTENT[32] = {
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {
        .hkey = 16,
        .key = "+",
        .value = &(struct cml_func_t){
            .fid = 0,
            .name = "+",
            .argc = 2
        },
        .next = NULL
    },
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
};

static struct htab OPTABLE = {
    .capacity = 32,
    .size = 1,
    .data = OPTABLE_CONTENT
};

struct cml_func_t *cml_get_opinfos(char *ident)
{
    return htab_get(&OPTABLE, ident)->value;
}
