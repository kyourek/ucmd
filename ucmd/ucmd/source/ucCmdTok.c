#include "ucmd_internal.h"

ucArgTok *ucCmdTok_get_arg(ucCmdTok *p) {
    return ucArgTokOwner_get_arg((ucArgTokOwner*)p);
}

ucSwitchTok *ucCmdTok_get_switch(ucCmdTok *p) {
    ucTok *tok;
    if (NULL == p) return NULL;
    tok = ucTok_get_next((ucTok*)p);
    while (NULL != tok) {
        if (ucTok_is_switch(tok)) {
            return (ucSwitchTok*)tok;
        }
        tok = ucTok_get_next(tok);
    }
    return NULL;
}
