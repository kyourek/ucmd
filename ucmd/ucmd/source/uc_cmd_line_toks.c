#include <stdlib.h>
#include "uc_cmd_line_toks.h"

uc_cmd_tok *uc_cmd_line_toks_get_cmd_tok(uc_cmd_line_toks *p) {
    if (NULL == p) return NULL;
    return p->cmd_tok;
}

ucArgTok *uc_cmd_line_toks_get_arg_tok(uc_cmd_line_toks *p) {
    if (NULL == p) return NULL;
    return p->arg_tok;
}

uc_switch_tok *uc_cmd_line_toks_get_switch_tok(uc_cmd_line_toks *p) {
    if (NULL == p) return NULL;
    return p->switch_tok;
}
