#include "ucmd_internal.h"

ucArgTok *ucArgTokOwner_get_arg(ucArgTokOwner *p) {
    ucTok* tok;
    if (NULL == p) return NULL;

    tok = ucTok_get_next((ucTok*)p);
    if (ucTok_is_switch(tok)) return NULL;

    return (ucArgTok*)tok;
}
