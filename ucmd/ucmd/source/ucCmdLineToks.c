#include <stdlib.h>
#include "ucCmdLineToks.h"

ucCmdTok *ucCmdLineToks_get_cmd_tok(ucCmdLineToks *p) {
    if (NULL == p) return NULL;
    return p->cmd_tok;
}

ucArgTok *ucCmdLineToks_get_arg_tok(ucCmdLineToks *p) {
    if (NULL == p) return NULL;
    return p->arg_tok;
}

ucSwitchTok *ucCmdLineToks_get_switch_tok(ucCmdLineToks *p) {
    if (NULL == p) return NULL;
    return p->switch_tok;
}
