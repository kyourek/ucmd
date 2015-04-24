#include "ucmd_internal.h"

ucArgTok *ucSwitchTok_get_arg(ucSwitchTok *p) {
    return ucArgTokOwner_get_arg((ucArgTokOwner*)p);
}

ucSwitchTok *ucSwitchTok_get_next(ucSwitchTok *p) {
    ucTok *tok;
    if (NULL == p) return NULL;
    tok = ucTok_get_next((ucTok*)p);
    while (NULL != tok) {
        if (ucTok_is_switch(tok)) return (ucSwitchTok*)tok;
        tok = ucTok_get_next((ucTok*)tok);
    }
    return NULL;
}

int ucSwitchTok_count(ucSwitchTok* p) {
    int count = 0;
    while (NULL != p) {
        count++;
        p = ucSwitchTok_get_next(p);
    }
    return count;
}

ucSwitchTok *ucSwitchTok_find(ucSwitchTok *p, const char *switch_value) {
    while (NULL != p) {
        if (ucTok_equals((ucTok*)p, switch_value)) {
            return p;
        }
        p = ucSwitchTok_get_next(p);
    }
    return NULL;
}

ucBool ucSwitchTok_contains(ucSwitchTok* p, const char *switch_value) {
    return NULL == ucSwitchTok_find(p, switch_value) ? ucBool_FALSE : ucBool_TRUE;
}
