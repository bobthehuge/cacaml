#include <err.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "../include/bth_alloc.h"
#include "../include/types.h"
#include "../include/utils.h"

void *m_scalloc(size_t s)
{
    return scalloc(1, s);
}

void *m_memdup(const void *src, size_t len)
{
    void *dst = smalloc(len);
    memcpy(dst, src, len);

    return dst;
}

char *m_strndup(const char *str, size_t n)
{
    size_t len = strlen(str);
    len = len > n ? n : len;

    char *res = m_memdup(str, len + 1);
    res[len] = 0;

    return res;
}

char *m_strdup(const char *str)
{
    size_t len = strlen(str);
    char *res = m_memdup(str, len + 1);
    res[len] = 0;

    return res;
}

char *m_strapp(char *dst, const char *src)
{
    size_t dlen = strlen(dst);
    size_t slen = strlen(src);

    char *res = srealloc(dst, dlen + slen + 1);
    memcpy(res + dlen, src, slen);
    res[dlen + slen] = 0;

    return res;
}

char *m_strcat(const char *s1, const char *s2)
{
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    char *res = smalloc(len1 + len2 + 1);
    memcpy(res, s1, len1);
    memcpy(res + len1, s2, len2);
    res[len1 + len2] = 0;

    return res;
}

char *stresc(const char *str)
{
    const char table[7] = {
        ['\a'%7] = 'a',
        ['\b'%7] = 'b',
        ['\t'%7] = 't',
        ['\n'%7] = 'n',
        ['\v'%7] = 'v',
        ['\f'%7] = 'f',
        ['\r'%7] = 'r',
    };

    const char *iter;
    size_t flen = 0;

    for (iter = str; *iter; iter++)
    {
        if (*iter >= '\a' && *iter <= '\r')
            flen++;
        flen++;
    }

    char *res = malloc(flen + 1);
    char *ptr = res;
    
    for (iter = str; *iter; iter++)
    {
        if (*iter >= '\a' && *iter <= '\r')
        {
            *ptr++ = '\\';
            *ptr++ = table[(*iter) % 7];
            continue;
        }

        *ptr++ = *iter;
    }

    *ptr = 0;

    return res;
}

char *file_basename(const char *path)
{
    char *ext = strrchr(path, '.');

    if (!ext)
        perr("'%s' is not a valid filename", path);

    char *org = ext;
    while (org > path && *org != '/')
        org--;

    if (org > path)
        org++;

    return m_strndup(org, ext - org);
}

int is_valid_int(const char *s, long *res, int base)
{
    char *end = NULL;

    errno = 0;
    *res = strtol(s, &end, base);

    if (errno == EINVAL || (end && *end))
        return 0;

    if (errno == ERANGE)
        return 1;

    return 2;
}


char *varsig(struct VarDeclNode *vnode)
{
    char *res = smalloc(1);
    *res = 0;

    res = m_strapp(res, type2str(vnode->type));
    res = m_strapp(res, " ");
    res = m_strapp(res, vnode->name);

    return res;
}

char *funcsig(struct FunDeclNode *fnode)
{
    char *res = smalloc(1);
    *res = 0;

    res = m_strapp(res, type2str(fnode->ret));
    res = m_strapp(res, " ");
    res = m_strapp(res, fnode->name);
    res = m_strapp(res, "(");

    if (fnode->argc)
    {
        char *tmp = varsig(fnode->args[0]->as.vdecl);
        res = m_strapp(res, tmp);
        free(tmp);

        for (size_t i = 1; i < fnode->argc; i++)
        {
            tmp = varsig(fnode->args[0]->as.vdecl);
            res = m_strapp(res, ", ");
            res = m_strapp(res, tmp);
            free(tmp);
        }
    }

    res = m_strapp(res, ")");

    return res;
}

// get a valid cstr representation from delimited raw bytes
char *gethashkey(const char *raw, size_t len)
{
    char *hk = malloc(len + 1);

    for (size_t i = 0; i < len; i++)
        hk[i] = raw[i] % 255 + 1;

    hk[len] = 0;
    return hk;
}

void __perr(const char *fun, int l, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    fprintf(stderr, "at %s:%d: ", fun, l);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");

    va_end(ap);

    exit(1);
}
