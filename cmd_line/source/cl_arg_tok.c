#include <stddef.h>
#include "cl_arg_tok.h"
#include "cl_tok_p.h"

cl_arg_tok *cl_arg_tok_get_next(cl_arg_tok* p) {
    cl_tok *tok;
    if (NULL == p) return NULL;
    tok = cl_tok_get_next((cl_tok*)p);
    if (NULL == tok) return NULL;
    if (cl_tok_is_switch(tok)) return NULL;
    return (cl_arg_tok*)tok;
}

int cl_arg_tok_count(cl_arg_tok* p) {
    int count = 0;
    while (NULL != p) {
        count++;
        p = cl_arg_tok_get_next(p);
    }
    return count;
}

cl_arg_tok *cl_arg_tok_find(cl_arg_tok *p, const char *arg_value) {
    while (NULL != p) {
        if (cl_tok_equals((cl_tok*)p, arg_value)) {
            return p;
        }
        p = cl_arg_tok_get_next(p);
    }
    return NULL;
}

cl_bool cl_arg_tok_contains(cl_arg_tok* p, const char *arg_value) {
    return NULL == cl_arg_tok_find(p, arg_value) ? CL_FALSE : CL_TRUE;
}
