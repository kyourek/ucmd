#include <stdlib.h>
#include "ucmdtests.h"

static ucTestErr ucCmdLineToks_get_cmd_tok_returns_value(ucTestGroup *p) {
    ucCmdLineToks inst = { 0 };
    ucCmdLineToks *ptr = &inst;
    ucCmdTok *cmd_tok = "c";

    ucTRUE(NULL == ucCmdLineToks_get_cmd_tok(NULL));
    ucTRUE(NULL == ucCmdLineToks_get_cmd_tok(ptr));

    ptr->cmd_tok = cmd_tok;
    ucTRUE(cmd_tok == ucCmdLineToks_get_cmd_tok(ptr));

    ucPASS();
}

static ucTestErr ucCmdLineToks_get_arg_tok_returns_value(ucTestGroup *p) {
    ucCmdLineToks inst = { 0 };
    ucCmdLineToks *ptr = &inst;
    ucArgTok *arg_tok = "a";

    ucTRUE(NULL == ucCmdLineToks_get_arg_tok(NULL));
    ucTRUE(NULL == ucCmdLineToks_get_arg_tok(ptr));

    ptr->arg_tok = arg_tok;
    ucTRUE(arg_tok == ucCmdLineToks_get_arg_tok(ptr));

    ucPASS();
}

static ucTestErr ucCmdLineToks_get_switch_tok_returns_value(ucTestGroup *p) {
    ucCmdLineToks inst = { 0 };
    ucCmdLineToks *ptr = &inst;
    ucArgTok *switch_tok = "-s";

    ucTRUE(NULL == ucCmdLineToks_get_switch_tok(NULL));
    ucTRUE(NULL == ucCmdLineToks_get_switch_tok(ptr));

    ptr->switch_tok = switch_tok;
    ucTRUE(switch_tok == ucCmdLineToks_get_switch_tok(ptr));

    ucPASS();
}

ucTestGroup *ucCmdLineToks_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_TestFunc *tests[] = {
        ucCmdLineToks_get_cmd_tok_returns_value,
        ucCmdLineToks_get_arg_tok_returns_value,
        ucCmdLineToks_get_switch_tok_returns_value,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
