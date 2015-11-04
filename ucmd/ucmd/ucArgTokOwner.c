#include "ucmd_internal.h"

ucArgTok *ucArgTokOwner_get_arg(ucArgTokOwner *p) {
    ucTok* tok;
    tok = ucTok_get_next(p);
    if (!tok) return NULL;
    if (ucTok_is_switch(tok)) return NULL;
    return tok;
}
