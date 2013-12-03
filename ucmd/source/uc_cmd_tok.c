#include <stdlib.h>
#include "uc_cmd_tok.h"

uc_arg_tok *uc_cmd_tok_get_arg(uc_cmd_tok *p) {
    return uc_arg_tok_owner_get_arg((uc_arg_tok_owner*)p);
}

uc_switch_tok *uc_cmd_tok_get_switch(uc_cmd_tok *p) {
    uc_tok *tok;
    if (NULL == p) return NULL;
    tok = uc_tok_get_next((uc_tok*)p);
    while (NULL != tok) {
        if (uc_tok_is_switch(tok)) {
            return (uc_switch_tok*)tok;
        }
        tok = uc_tok_get_next(tok);
    }
    return NULL;
}
