#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cml_log.h"
#include "cml_parser.h"

size_t digit_count(int64_t value)
{
    int64_t l = !value;

    while(value)
    { 
        l++; 
        value/=10; 
    }

    return l;
}

size_t serialize_token(struct cml_token *tok, char **dst)
{
    size_t size = digit_count(tok->kind)+4;
    int32_t val;

    if (tok->kind == T_OPERAND)
    {
        struct cml_i32 *num = tok->value;
        val = num->value;
    }
    else if (tok->kind == T_FUNCTION)
    {
        struct cml_callable *call = tok->value;
        val = call->id;
    }
    else
    {
        ERRX(1, "TODO: unsupported TokenKind serialization");
    }

    size += digit_count(val);
    *dst = malloc(size);
    (*dst)[size-2] = '\0';
    sprintf(*dst, "(%d;%d", tok->kind, val);

    return size;
}

size_t serialize_ast(struct cmle_ast *ast, struct cml_cstr **dst)
{
    size_t size = serialize_key(&ast->key, dst);

    for (size_t i = 0; i < ast->count; i++)
    {
        char *child;
        size += serialize_ast(&ast->kids[i], &child);
        
        *dst = realloc(*dst, size);
        strcat(*dst, child);
        free(child);
    }

    (*dst)[size-2] = ')';
    (*dst)[size-1] = '\0';

    return size-1;
}
