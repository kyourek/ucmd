#include <stddef.h>
#include "uc_arg_tok_owner.h"

uc_arg_tok *uc_arg_tok_owner_get_arg(uc_arg_tok_owner *p) {
    uc_tok* tok;
    if (NULL == p) return NULL;

    tok = uc_tok_get_next((uc_tok*)p);
    if (uc_tok_is_switch(tok)) return NULL;

    return (uc_arg_tok*)tok;
}
