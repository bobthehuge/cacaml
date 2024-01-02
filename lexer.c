#include <stdio.h>
#include "lexer.h"

Token EMPTY_TOKEN = {TOKEN_END, NULL, -1, (Vec2i){-1,-1}};

char* string_of_kind(Token_Kind tokkind)
{/*{{{*/
    switch (tokkind)
    {
        case TOKEN_IDENT:
            return "ident";
        case TOKEN_TYPE:
            return "type";
        case TOKEN_LET:
            return "let";
        case TOKEN_EQUAL:
            return "=";
        case TOKEN_PLUS:
            return "+";
        case TOKEN_DASH:
            return "-";
        case TOKEN_STAR:
            return "*";
        case TOKEN_SLASH:
            return "/";
        case TOKEN_PERCENT:
            return "%";
        case TOKEN_PIPE:
            return "|";
        case TOKEN_UPPERSAND:
            return "&";
        case TOKEN_SEMICOLON:
            return ":";
        case TOKEN_LPAREN:
            return "(";
        case TOKEN_RPAREN:
            return ")";
        case TOKEN_LANGLE:
            return "<";
        case TOKEN_RANGLE:
            return ">";
        default:
            printf("unknown token_kind\n");
            exit(-1);
    }
}/*}}}*/

void print_token(Token tok)
{
    printf("{kind: %s; text: '%s'; len: %ld; pos: (%d,%d)}\n",
            string_of_kind(tok.kind), tok.text, tok.text_len, 
            tok.pos.x, tok.pos.y);
}

void add_char(char** buffer, size_t* buf_size, size_t* occup, char chr)
{
    char* tmp = *buffer;
    (*occup)++;

    if(*occup >= *buf_size)
    {
        *buffer = realloc(*buffer, *occup);
        *buf_size = *occup;
    }

    *(tmp + *occup - 1) = chr;
}

char get_char(Lexer* lexer)
{/*{{{*/
    return *(lexer->content + lexer->cursor);
}/*}}}*/

int is_separator(char c)
{/*{{{*/
    return (c == ' ' || c == '\n');
}/*}}}*/

Token_Kind get_kind(char* buffer)
{/*{{{*/
    if (!strcmp(buffer, "int"))
        return TOKEN_TYPE;
    else if (!strcmp(buffer, "let"))
        return TOKEN_LET;
    else if (!strcmp(buffer, "="))
        return TOKEN_EQUAL;
    else if (!strcmp(buffer, "+"))
        return TOKEN_PLUS;
    else if (!strcmp(buffer, "-"))
        return TOKEN_DASH;
    else if (!strcmp(buffer, "*"))
        return TOKEN_STAR;
    else if (!strcmp(buffer, "/"))
        return TOKEN_SLASH;
    else if (!strcmp(buffer, "%"))
        return TOKEN_PERCENT;
    else if (!strcmp(buffer, "|"))
        return TOKEN_PIPE;
    else if (!strcmp(buffer, "&"))
        return TOKEN_UPPERSAND;
    else if (!strcmp(buffer, ":"))
        return TOKEN_SEMICOLON;
    else if (!strcmp(buffer, "("))
        return TOKEN_LPAREN;
    else if (!strcmp(buffer, ")"))
        return TOKEN_RPAREN;
    else if (!strcmp(buffer, "<"))
        return TOKEN_LANGLE;
    else if (!strcmp(buffer, ">"))
        return TOKEN_RANGLE;
    else
        return TOKEN_IDENT;
}/*}}}*/

Token token_next(Lexer* lexer)
{
    char* buffer = calloc(8, 1);
    size_t buf_size = 8;
    size_t occup = 0;

    Token token = {0};

    size_t bow = lexer->cursor;

    for(;lexer->cursor < lexer->content_len && 
            !is_separator(get_char(lexer)) &&
            (token.kind == TOKEN_END || 
             token.kind == TOKEN_IDENT)
        ;lexer->cursor++)
    {
        add_char(&buffer, &buf_size, &occup, get_char(lexer));
        
        if(occup == 0)
            continue;

        token.kind = get_kind(buffer);
    }

    for(;lexer->cursor < lexer->content_len &&
            is_separator(get_char(lexer))
        ;lexer->cursor++)
    {
        if(get_char(lexer) == '\n')
        {
            (lexer->line)++;
            (lexer->bol) = lexer->cursor;
        }
    }

    Vec2i pos = {
        lexer->line,
        (bow - lexer->bol)
    };

    if(occup < (buf_size-1))
        buffer = realloc(buffer, occup);

    token.pos = pos;
    token.text = buffer;
    token.text_len = occup;

    return token;
}

Token* tokenize(Lexer* lexer)
{
    Token token = token_next(lexer);
    Token* lexed = malloc(sizeof(Token));
    *lexed = token;
    size_t size = 1;

    while(token.kind != TOKEN_END)
    {
        token = token_next(lexer);
        lexed = realloc(lexed, (size+1)*sizeof(Token));

        *(lexed + size) = token;
        size++;
    }

    return lexed;
}

Lexer lexer_new(char* content, size_t content_len)
{
    Lexer lexer = {
        content,
        content_len,
        0, 
        0, 
        0
    };

    return lexer;
}
