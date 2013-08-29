#include <stdlib.h>
#include "cl_arg_tok.h"
#include "cl_arg_tok_owner.h"
#include "cl_common.h"
#include "cl_tok.h"

cl_arg_tok *cl_arg_tok_owner_get_arg(cl_arg_tok_owner *p) {
    cl_tok* tok;
    if (NULL == p) return NULL;

    tok = cl_tok_get_next((cl_tok*)p);
    if (cl_tok_is_switch(tok)) return NULL;

    return (cl_arg_tok*)tok;
}
