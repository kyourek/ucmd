#include "ucmdtests.h"

uc_TEST(ucCmdTok_get_arg_gets_arg)
    char cmd[] = "command\0arg\0\n";
    uc_TRUE(ucBool_true == ucTok_equals((ucTok*)ucCmdTok_get_arg(cmd), "arg"));
uc_PASS

uc_TEST(ucCmdTok_get_arg_returns_null_if_followed_by_switch)
    char cmd[] = "cmd\0-s\0a\0\n";
    uc_TRUE(NULL == ucCmdTok_get_arg(cmd));
uc_PASS

uc_TEST(ucCmdTok_get_switch_gets_first_switch)
    char cmd[] = "c\0a\0a2\0-s1\0-s2\0\n";
    uc_TRUE(ucTok_equals((ucTok*)ucCmdTok_get_switch(cmd), "-s1"));
uc_PASS

uc_TEST(ucCmdTok_get_switch_returns_null_if_no_switch)
    char cmd[] = "name\0a1\0ssss\0\n";
    uc_TRUE(NULL == ucCmdTok_get_switch(cmd));
uc_PASS

uc_TEST_GROUP(ucCmdTok, NULL,
    ucCmdTok_get_arg_gets_arg,
    ucCmdTok_get_arg_returns_null_if_followed_by_switch,
    ucCmdTok_get_switch_gets_first_switch,
    ucCmdTok_get_switch_returns_null_if_no_switch)
