#include "ucmd_internal.h"

ucArgTok *ucArgTok_get_next(ucArgTok* p) {
    ucTok *tok;
    if (NULL == p) return NULL;
    tok = ucTok_get_next((ucTok*)p);
    if (NULL == tok) return NULL;
    if (ucTok_is_switch(tok)) return NULL;
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
        if (ucTok_equals((ucTok*)p, arg_value)) {
            return p;
        }
        p = ucArgTok_get_next(p);
    }
    return NULL;
}

ucBool ucArgTok_contains(ucArgTok* p, const char *arg_value) {
    return NULL == ucArgTok_find(p, arg_value) ? ucBool_false : ucBool_true;
}
