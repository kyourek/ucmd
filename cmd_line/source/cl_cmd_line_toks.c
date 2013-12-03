#include <stdlib.h>
#include "cl_cmd_line_toks.h"

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
