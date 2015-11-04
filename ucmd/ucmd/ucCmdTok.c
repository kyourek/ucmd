#include "ucmd_internal.h"

ucArgTok *ucCmdTok_get_arg(ucCmdTok *p) {
    return ucArgTokOwner_get_arg(p);
}

ucSwitchTok *ucCmdTok_get_switch(ucCmdTok *p) {
    ucTok *tok;

    assert(p);

    tok = ucTok_get_next(p);
    while (tok) {
        if (ucTok_is_switch(tok)) {
            return tok;
        }
        tok = ucTok_get_next(tok);
    }
    return NULL;
}
