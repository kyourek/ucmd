#include <stddef.h>
#include "uc_arg_tok.h"
#include "uc_tok_p.h"

uc_arg_tok *uc_arg_tok_get_next(uc_arg_tok* p) {
    uc_tok *tok;
    if (NULL == p) return NULL;
    tok = uc_tok_get_next((uc_tok*)p);
    if (NULL == tok) return NULL;
    if (uc_tok_is_switch(tok)) return NULL;
    return (uc_arg_tok*)tok;
}

int uc_arg_tok_count(uc_arg_tok* p) {
    int count = 0;
    while (NULL != p) {
        count++;
        p = uc_arg_tok_get_next(p);
    }
    return count;
}

uc_arg_tok *uc_arg_tok_find(uc_arg_tok *p, const char *arg_value) {
    while (NULL != p) {
        if (uc_tok_equals((uc_tok*)p, arg_value)) {
            return p;
        }
        p = uc_arg_tok_get_next(p);
    }
    return NULL;
}

uc_bool uc_arg_tok_contains(uc_arg_tok* p, const char *arg_value) {
    return NULL == uc_arg_tok_find(p, arg_value) ? UC_FALSE : UC_TRUE;
}
