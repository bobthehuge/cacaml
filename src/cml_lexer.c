#include <cml_token.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "../utils/bth_dynarray.h"
#include "../utils/bth_log.h"
#include "../utils/bth_salloc.h"

#include "cml_lexer.h"

#define CML_KEYWORD(s) { sizeof(s) - 1, s }

static const struct bth_cstr CML_KW_LIST[CML_KW_COUNT] = {
    CML_KEYWORD("let"),
    CML_KEYWORD("if"),
};

int cml_lexer_from_file(struct cml_lexer *lex, char *path)
{
    FILE *f = fopen(path, "r");

    if (!f)
    {
        ERROR(1, "Cannot fopen '%s'", path);
        return 1;
    }

    fseek(f, 0, SEEK_END);
    uint32_t len = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *d = smalloc(len);
    size_t count = fread(d, 1, len, f);

    if (count != len)
    {
        ERROR(1, "Cannot fread %u bytes from '%s'", len, path);
        return 1;
    }

    fclose(f);

    lex->buf_path = path;
    lex->buf = d;
    lex->buf_len = len;
    lex->pos = 0;
    lex->read_pos = 0;
    lex->ch = d[0];

    return 0;
}

inline void cml_lexer_destroy(struct cml_lexer *lex)
{
    free(lex->buf);
}

static inline char cml_lexer_peek(struct cml_lexer *lex)
{
    if (lex->read_pos >= lex->buf_len - 1)
    {
        return EOF;
    }

    return lex->buf[lex->read_pos + 1];
}

static inline void cml_lexer_next_char(struct cml_lexer *lex)
{
    char c = cml_lexer_peek(lex);

    if (c)
    {
        lex->pos = lex->read_pos;
        lex->read_pos++;
    }

    lex->ch = c;
}

static inline void cml_lexer_skip_spaces(struct cml_lexer *lex)
{
    char c;

    while (lex->ch != EOF && isspace(lex->ch))
    {
        cml_lexer_next_char(lex);   
    }
}


// returns if a keyword has successfully been found
// if so, res is a valid index inside CML_KW_LIST
// if not, res is equal to CML_KW_COUNT
static inline int cml_get_keyword(char *pos, uint32_t *res)
{
    uint32_t i = 0;

    while (i < CML_KW_COUNT && CSTRNCMP(pos, CML_KW_LIST[i]))
        i++;

    *res = i;
    return i < CML_KW_COUNT;
}

static inline void cml_lexer_skip(struct cml_lexer *lex, uint32_t n)
{
    lex->read_pos += n;
    lex->pos = lex->read_pos - 1;
    lex->ch = lex->buf[lex->read_pos];
}

struct cml_token cml_lexer_next_token(struct cml_lexer *lex)
{
    uint32_t kw_idx = 0;
    cml_lexer_skip_spaces(lex);

    if (lex->ch == EOF)
    {
        cml_lexer_next_char(lex);
        return (struct cml_token){.kind = END, .value = NULL};
    }
    else if (lex->ch == '+')
    {
        cml_lexer_next_char(lex);
        return (struct cml_token){.kind = TK_ADD, .value = NULL};
    }
    else if (lex->ch == '=')
    {
        cml_lexer_next_char(lex);
        return (struct cml_token){.kind = TK_EQ, .value = NULL};
    }
    else if (cml_get_keyword(lex->buf + lex->read_pos, &kw_idx))
    {
        cml_lexer_skip(lex, CML_KW_LIST[kw_idx].len);

        return (struct cml_token) {
            .kind = CML_TKIND_KW_OFFSET + kw_idx,
            .value = NULL
        };
    }
    else if (isdigit(lex->ch))
    {
        uint32_t start = lex->read_pos;

        do 
        {
            cml_lexer_next_char(lex);
        } while (isdigit(lex->ch));

        uint32_t count = lex->read_pos - start;
        char *s = smalloc(count + 1);
        memcpy(s, lex->buf + start, count);
        s[count] = 0;

        return (struct cml_token){.kind = TK_INT32, .value = s};
    }
    else if (isalnum(lex->ch) || lex->ch == '_')
    {
        uint32_t start = lex->read_pos;

        do 
        {
            cml_lexer_next_char(lex);
        } while (isalnum(lex->ch) || lex->ch == '_');

        uint32_t count = lex->read_pos - start;
        char *s = smalloc(count + 1);
        memcpy(s, lex->buf + start, count);
        s[count] = 0;

        return (struct cml_token){.kind = TK_IDENT, .value = s};
    }
    else
    {
        char *s = smalloc(2);

        s[0] = lex->ch;
        s[1] = 0;
        cml_lexer_next_char(lex);

        return (struct cml_token){.kind = TK_INVALID, .value = s};
    }
}

struct bth_dynarray cml_lexer_lexall(struct cml_lexer *lex)
{
    struct bth_dynarray da = bth_dynarray_init(sizeof(struct cml_token), 0);
    
    struct cml_token tok = {0};
    int i = 0;

    do
    {
        tok = cml_lexer_next_token(lex);
        bth_dynarray_append(&da, &tok);
    } while (tok.kind != END);
    
    return da;
}
