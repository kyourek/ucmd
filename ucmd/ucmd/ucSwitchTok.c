#include "ucmd_internal.h"

ucArgTok *ucSwitchTok_get_arg(ucSwitchTok *p) {
    return ucArgTokOwner_get_arg(p);
}

ucSwitchTok *ucSwitchTok_get_next(ucSwitchTok *p) {
    ucTok *tok = ucTok_get_next(p);
    while (tok) {
        if (ucTok_is_switch(tok)) {
            return tok;
        }
        tok = ucTok_get_next(tok);
    }
    return NULL;
}

ucSwitchTok *ucSwitchTok_get_index(ucSwitchTok *p, int index) {
    assert(p);
    for (; p && index; p = ucSwitchTok_get_next(p), index--);
    return p;
}

int ucSwitchTok_count(ucSwitchTok* p) {
    int count;
    assert(p);
    for (count = 0; p; count++, p = ucSwitchTok_get_next(p));
    return count;
}

ucSwitchTok *ucSwitchTok_find(ucSwitchTok *p, const char *switch_name) {
    assert(p);
    while (p) {
        if (ucTok_equals(p, switch_name)) {
            return p;
        }
        p = ucSwitchTok_get_next(p);
    }
    return NULL;
}

ucBool ucSwitchTok_contains(ucSwitchTok* p, const char *switch_name) {
    return NULL == ucSwitchTok_find(p, switch_name) ? ucBool_false : ucBool_true;
}
