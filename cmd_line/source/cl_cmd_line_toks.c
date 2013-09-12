#include <stdlib.h>
#include "cl_arg_tok.h"
#include "cl_cmd_line_toks.h"
#include "cl_cmd_tok.h"
#include "cl_switch_tok.h"

cl_cmd_tok *cl_cmd_line_toks_get_cmd_tok(cl_cmd_line_toks *p) {
    if (NULL == p) return NULL;
    return p->cmd_tok;
}

cl_arg_tok *cl_cmd_line_toks_get_arg_tok(cl_cmd_line_toks *p) {
    if (NULL == p) return NULL;
    return p->arg_tok;
}

cl_switch_tok *cl_cmd_line_toks_get_switch_tok(cl_cmd_line_toks *p) {
    if (NULL == p) return NULL;
    return p->switch_tok;
}
