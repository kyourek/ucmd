#include <stdlib.h>
#include "ucCmdLineToks.h"
#include "ucCmdLineToks_tests.h"
#include "ucTest.h"

static ucTestErr ucCmdLineToks_get_cmd_tok_returns_value(ucTestGroup *p) {
    ucCmdLineToks inst = { 0 };
    ucCmdLineToks *ptr = &inst;
    ucCmdTok *cmd_tok = "c";

    UC_TEST_ASSERT(NULL == ucCmdLineToks_get_cmd_tok(NULL));
    UC_TEST_ASSERT(NULL == ucCmdLineToks_get_cmd_tok(ptr));

    ptr->cmd_tok = cmd_tok;
    UC_TEST_ASSERT(cmd_tok == ucCmdLineToks_get_cmd_tok(ptr));

    return ucTestErr_NONE;
}

static ucTestErr ucCmdLineToks_get_arg_tok_returns_value(ucTestGroup *p) {
    ucCmdLineToks inst = { 0 };
    ucCmdLineToks *ptr = &inst;
    ucArgTok *arg_tok = "a";

    UC_TEST_ASSERT(NULL == ucCmdLineToks_get_arg_tok(NULL));
    UC_TEST_ASSERT(NULL == ucCmdLineToks_get_arg_tok(ptr));

    ptr->arg_tok = arg_tok;
    UC_TEST_ASSERT(arg_tok == ucCmdLineToks_get_arg_tok(ptr));

    return ucTestErr_NONE;
}

static ucTestErr ucCmdLineToks_get_switch_tok_returns_value(ucTestGroup *p) {
    ucCmdLineToks inst = { 0 };
    ucCmdLineToks *ptr = &inst;
    ucArgTok *switch_tok = "-s";

    UC_TEST_ASSERT(NULL == ucCmdLineToks_get_switch_tok(NULL));
    UC_TEST_ASSERT(NULL == ucCmdLineToks_get_switch_tok(ptr));

    ptr->switch_tok = switch_tok;
    UC_TEST_ASSERT(switch_tok == ucCmdLineToks_get_switch_tok(ptr));

    return ucTestErr_NONE;
}

ucTestGroup *ucCmdLineToks_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_test_func *tests[] = {
        ucCmdLineToks_get_cmd_tok_returns_value,
        ucCmdLineToks_get_arg_tok_returns_value,
        ucCmdLineToks_get_switch_tok_returns_value,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
