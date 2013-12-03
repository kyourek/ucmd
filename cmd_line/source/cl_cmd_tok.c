#include <stdlib.h>
#include "cl_cmd_tok.h"

cl_arg_tok *cl_cmd_tok_get_arg(cl_cmd_tok *p) {
    return cl_arg_tok_owner_get_arg((cl_arg_tok_owner*)p);
}

cl_switch_tok *cl_cmd_tok_get_switch(cl_cmd_tok *p) {
    cl_tok *tok;
    if (NULL == p) return NULL;
    tok = cl_tok_get_next((cl_tok*)p);
    while (NULL != tok) {
        if (cl_tok_is_switch(tok)) {
            return (cl_switch_tok*)tok;
        }
        tok = cl_tok_get_next(tok);
    }
    return NULL;
}
