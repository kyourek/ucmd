#include <stddef.h>
#include "uc_switch_tok.h"

ucArgTok *uc_switch_tok_get_arg(uc_switch_tok *p) {
    return ucArgTokOwner_get_arg((ucArgTokOwner*)p);
}

uc_switch_tok *uc_switch_tok_get_next(uc_switch_tok *p) {
    uc_tok *tok;
    if (NULL == p) return NULL;
    tok = uc_tok_get_next((uc_tok*)p);
    while (NULL != tok) {
        if (uc_tok_is_switch(tok)) return (uc_switch_tok*)tok;
        tok = uc_tok_get_next((uc_tok*)tok);
    }
    return NULL;
}

int uc_switch_tok_count(uc_switch_tok* p) {
    int count = 0;
    while (NULL != p) {
        count++;
        p = uc_switch_tok_get_next(p);
    }
    return count;
}

uc_switch_tok *uc_switch_tok_find(uc_switch_tok *p, const char *switch_value) {
    while (NULL != p) {
        if (uc_tok_equals((uc_tok*)p, switch_value)) {
            return p;
        }
        p = uc_switch_tok_get_next(p);
    }
    return NULL;
}

ucBool uc_switch_tok_contains(uc_switch_tok* p, const char *switch_value) {
    return NULL == uc_switch_tok_find(p, switch_value) ? ucBool_false : ucBool_true;
}
