#include <stddef.h>
#include "ucArgTok.h"
#include "uc_tok_p.h"

ucArgTok *ucArgTok_get_next(ucArgTok* p) {
    uc_tok *tok;
    if (NULL == p) return NULL;
    tok = uc_tok_get_next((uc_tok*)p);
    if (NULL == tok) return NULL;
    if (uc_tok_is_switch(tok)) return NULL;
    return (ucArgTok*)tok;
}

int ucArgTok_count(ucArgTok* p) {
    int count = 0;
    while (NULL != p) {
        count++;
        p = ucArgTok_get_next(p);
    }
    return count;
}

ucArgTok *ucArgTok_find(ucArgTok *p, const char *arg_value) {
    while (NULL != p) {
        if (uc_tok_equals((uc_tok*)p, arg_value)) {
            return p;
        }
        p = ucArgTok_get_next(p);
    }
    return NULL;
}

ucBool ucArgTok_contains(ucArgTok* p, const char *arg_value) {
    return NULL == ucArgTok_find(p, arg_value) ? ucBool_false : ucBool_true;
}
