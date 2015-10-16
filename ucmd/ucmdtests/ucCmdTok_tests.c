#include <string.h>
#include "ucmdtests.h"

static ucTestErr ucCmdTok_get_arg_gets_arg(ucTestGroup *p) {
    char cmd[] = "command\0arg\0\n";
    ucTest_ASSERT(ucBool_TRUE == ucTok_equals((ucTok*)ucCmdTok_get_arg(cmd), "arg"));
    return ucTestErr_NONE;
}

static ucTestErr ucCmdTok_get_arg_returns_null_if_followed_by_switch(ucTestGroup *p) {
    char cmd[] = "cmd\0-s\0a\0\n";
    ucTest_ASSERT(NULL == ucCmdTok_get_arg(cmd));
    return ucTestErr_NONE;
}

static ucTestErr ucCmdTok_get_switch_gets_first_switch(ucTestGroup *p) {
    char cmd[] = "c\0a\0a2\0-s1\0-s2\0\n";
    ucTest_ASSERT(ucTok_equals((ucTok*)ucCmdTok_get_switch(cmd), "-s1"));
    return ucTestErr_NONE;
}

static ucTestErr ucCmdTok_get_switch_returns_null_if_no_switch(ucTestGroup *p) {
    char cmd[] = "name\0a1\0ssss\0\n";
    ucTest_ASSERT(NULL == ucCmdTok_get_switch(cmd));
    return ucTestErr_NONE;
}

ucTestGroup *ucCmdTok_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_TestFunc *tests[] = {
        ucCmdTok_get_arg_gets_arg,
        ucCmdTok_get_arg_returns_null_if_followed_by_switch,
        ucCmdTok_get_switch_gets_first_switch,
        ucCmdTok_get_switch_returns_null_if_no_switch,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
