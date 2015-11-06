#include <stdlib.h>
#include "ucmdtests.h"

uc_TEST(ucCmdLineToks_get_cmd_tok_returns_value)
    ucCmdLineToks inst = { 0 };
    ucCmdLineToks *ptr = &inst;
    ucCmdTok *cmd_tok = "c";

    uc_TRUE(NULL == ucCmdLineToks_get_cmd_tok(ptr));

    ptr->cmd_tok = cmd_tok;
    uc_TRUE(cmd_tok == ucCmdLineToks_get_cmd_tok(ptr));
uc_PASS

uc_TEST(ucCmdLineToks_get_arg_tok_returns_value)
    ucCmdLineToks inst = { 0 };
    ucCmdLineToks *ptr = &inst;
    ucArgTok *arg_tok = "a";

    uc_TRUE(NULL == ucCmdLineToks_get_arg_tok(ptr));

    ptr->arg_tok = arg_tok;
    uc_TRUE(arg_tok == ucCmdLineToks_get_arg_tok(ptr));
uc_PASS

uc_TEST(ucCmdLineToks_get_switch_tok_returns_value)
    ucCmdLineToks inst = { 0 };
    ucCmdLineToks *ptr = &inst;
    ucArgTok *switch_tok = "-s";

    uc_TRUE(NULL == ucCmdLineToks_get_switch_tok(ptr));

    ptr->switch_tok = switch_tok;
    uc_TRUE(switch_tok == ucCmdLineToks_get_switch_tok(ptr));
uc_PASS

uc_TEST_GROUP(ucCmdLineToks, NULL,
    ucCmdLineToks_get_cmd_tok_returns_value,
    ucCmdLineToks_get_arg_tok_returns_value,
    ucCmdLineToks_get_switch_tok_returns_value)
