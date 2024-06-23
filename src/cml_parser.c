#include <stdlib.h>
#include "../utils/bth_log.h"

#include "cml_lexer.h"
#include "cml_operator.h"
#include "cml_parser.h"

//    n   =>   n
//   /        /
//  ..      .., {0}
//
static inline struct cml_token *cml_ast_shift(struct cml_token *root)
{
    root->count++;
    struct cml_token **tmp = 
        realloc(root->kids, root->count * sizeof(struct cml_token *));

    if (!tmp) ERR(1, "ERROR");

    root->kids = tmp;

    return root;
}


//    n   =>   n
//   /        /
//  ..       o 
//          /
//         ..
//
static inline void cml_ast_reduce(
    struct cml_token *root,
    struct cml_token *new_node
){
    cml_ast_shift(root);
    root->kids[root->count - 1] = new_node;
    new_node->parent = root;
}

static inline void cml_ast_adopt(
    struct cml_token *new_parent,
    struct cml_token *old_parent
){
        new_parent->kids = old_parent->kids;
        new_parent->count = old_parent->count;
        old_parent->kids = NULL;
        old_parent->count = 0;

        for (size_t i = 0; i < new_parent->count; i++)
            new_parent->kids[i]->parent = old_parent;
}

void cml_build_ast(
    struct cml_token *tokens,
    size_t count
){
    struct cml_token *root = tokens + count;
    struct cml_token *lroot = root;

    size_t i = 0;
    
    while (1)
    {
        switch (tokens[i].kind) 
        {
        case CML_TOK_NULL:
            return;
        case CML_TOK_I32:
            if (lroot->kind == CML_TOK_I32)
            {
                ERRX(1, "Missing operator");
            }
            else if (
                lroot->kind &&
                lroot->count >= 
                ((struct cml_func_t *)lroot->value)->argc
            ){
                ERRX(1, "Too much args");
            }
            
            cml_ast_shift(lroot);
            lroot->kids[lroot->count - 1] = &tokens[i];
            tokens[i].parent = lroot;
            break;
        case CML_TOK_BINOP:
            if (lroot->kind)
                lroot = lroot->parent;

            cml_ast_adopt(&tokens[i], lroot);
            cml_ast_reduce(lroot, &tokens[i]);
            lroot = &tokens[i];
            break;
        case CML_TOK_CALL:
            TODO();
            break;
        default:
            ERRX(1, "unknown token type");
        }

        i++;
    }
}
