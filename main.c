#include "lexer.h"
#include "common.h"

int main(void)
{
    size_t cont_len;
    char* cont = get_file_content("test.cml", &cont_len);

    //printf("%s\n", cont);

    Lexer lexer = lexer_new(cont, cont_len);
    Token* lexed = tokenize(&lexer);
    
    printf("hi\n");

    for(size_t i = 0; (lexed+i)->kind != TOKEN_END; i++)
    {
        print_token(*(lexed+i));
        free((lexed+i)->text);
    }

    free(cont);
    free(lexed);

    return 0;
}
