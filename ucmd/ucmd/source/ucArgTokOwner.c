#include <stddef.h>
#include "ucArgTokOwner.h"

ucArgTok *ucArgTokOwner_get_arg(ucArgTokOwner *p) {
    uc_tok* tok;
    if (NULL == p) return NULL;

    tok = uc_tok_get_next((uc_tok*)p);
    if (uc_tok_is_switch(tok)) return NULL;

    return (ucArgTok*)tok;
}
