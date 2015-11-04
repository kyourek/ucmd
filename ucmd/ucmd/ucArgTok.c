#include "ucmd_internal.h"

ucArgTok *ucArgTok_get_next(ucArgTok* p) {
    ucTok *tok = ucTok_get_next(p);
    if (!tok) return NULL;
    if (ucTok_is_switch(tok)) return NULL;
    return tok;
}

int ucArgTok_count(ucArgTok* p) {
    int count;
    assert(p);
    for (count = 0; p; count++, p = ucArgTok_get_next(p));
    return count;
}

ucArgTok *ucArgTok_find(ucArgTok *p, const char *arg_value) {
    assert(p);
    while (p) {
        if (ucTok_equals(p, arg_value)) {
            return p;
        }
        p = ucArgTok_get_next(p);
    }
    return NULL;
}

ucBool ucArgTok_contains(ucArgTok* p, const char *arg_value) {
    return ucArgTok_find(p, arg_value) 
        ? ucBool_true
        : ucBool_false;
}
