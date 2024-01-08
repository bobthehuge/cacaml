#ifndef CML_CSTR_H
#define CML_CSTR_H

struct cml_cstr
{
    size_t length;
    char **content;
};

struct cml_cstr *cml_cstr_new(size_t prealloc_size);
void cml_cstr_free(struct cml_cstr *cstr);
void cml_cstr_resize(struct cml_cstr *cstr, size_t size);
void cml_cstr_append(struct cml_cstr *cstr, char **str, size_t size);
void cml_cstr_cat(struct cml_cstr *src, struct cml_cstr *dst);

#endif
