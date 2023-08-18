#ifndef LEXER_H_
#define LEXER_H_

#include "common.h"

enum {
    TOKEN_END = 0,          // shouldn't be used as data holder
    TOKEN_IDENT,            // identifier
    TOKEN_TYPE,             // type declarator
    TOKEN_I32,              // int
    TOKEN_F32,              // float
    TOKEN_LET,              // let
    TOKEN_EQUAL,            // =
    TOKEN_PLUS,             // +
    TOKEN_DASH,             // -
    TOKEN_STAR,             // *
    TOKEN_SLASH,            // /
    TOKEN_PERCENT,          // %
    TOKEN_PIPE,             // |
    TOKEN_UPPERSAND,        // &
    TOKEN_SEMICOLON,        // :
    TOKEN_LPAREN,           // (
    TOKEN_RPAREN,           // )
    TOKEN_LANGLE,           // <
    TOKEN_RANGLE,           // >
};

typedef unsigned char Token_Kind;

typedef struct {
    Token_Kind kind;
    char* text;
    size_t text_len;
    Vec2i pos;              // 0: line ; 1: col
} Token;

typedef struct {
    char* content;
    size_t content_len;
    size_t cursor;
    size_t line;
    size_t bol;             // beginning of line (col = cursor - bol)
} Lexer;

void print_token(Token token);

Lexer lexer_new(char* content, size_t content_len);
Token token_next(Lexer* lexer);
Token* tokenize(Lexer* lexer);

#endif /* LEXER_H_ */
