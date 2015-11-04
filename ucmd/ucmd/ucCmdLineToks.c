#include "ucmd_internal.h"

ucCmdTok *ucCmdLineToks_get_cmd_tok(ucCmdLineToks *p) {
    assert(p);
    return p->cmd_tok;
}

ucArgTok *ucCmdLineToks_get_arg_tok(ucCmdLineToks *p) {
    assert(p);
    return p->arg_tok;
}

ucSwitchTok *ucCmdLineToks_get_switch_tok(ucCmdLineToks *p) {
    assert(p);
    return p->switch_tok;
}
