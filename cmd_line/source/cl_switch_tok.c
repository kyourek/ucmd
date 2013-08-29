#include <stdlib.h>
#include "cl_common.h"
#include "cl_arg_tok.h"
#include "cl_arg_tok_owner.h"
#include "cl_switch_tok.h"
#include "cl_tok.h"

cl_arg_tok *cl_switch_tok_get_arg(cl_switch_tok *p) {
    return cl_arg_tok_owner_get_arg((cl_arg_tok_owner*)p);
}

cl_switch_tok *cl_switch_tok_get_next(cl_switch_tok *p) {
    cl_tok *tok;
    if (NULL == p) return NULL;
    tok = cl_tok_get_next((cl_tok*)p);
    while (NULL != tok) {
        if (cl_tok_is_switch(tok)) return (cl_switch_tok*)tok;
        tok = cl_tok_get_next((cl_tok*)tok);
    }
    return NULL;
}

int cl_switch_tok_count(cl_switch_tok* p) {
    int count = 0;
    while (NULL != p) {
        count++;
        p = cl_switch_tok_get_next(p);
    }
    return count;
}

cl_switch_tok *cl_switch_tok_find(cl_switch_tok *p, const char *switch_value) {
    while (NULL != p) {
        if (cl_tok_equals((cl_tok*)p, switch_value)) {
            return p;
        }
        p = cl_switch_tok_get_next(p);
    }
    return NULL;
}

CL_BOOL cl_switch_tok_contains(cl_switch_tok* p, const char *switch_value) {
    return NULL == cl_switch_tok_find(p, switch_value) ? CL_FALSE : CL_TRUE;
}
