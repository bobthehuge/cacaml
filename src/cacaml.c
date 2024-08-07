// #include <stdio.h>
#include "cml_token.h"
#include "../utils/bth_log.h"

int main()
{
    LOGX("Hello, World!");

    struct token t = {
        .ttype = T_INT32,
        .tvalue = 42
    };

    return 0;
}
