#include <string.h>
#include "ucmdtests.h"

static ucTestErr ucCmdTok_get_arg_gets_arg(ucTestGroup *p) {
    char cmd[] = "command\0arg\0\n";
    ucTRUE(ucBool_true == ucTok_equals((ucTok*)ucCmdTok_get_arg(cmd), "arg"));
    ucPASS();
}

static ucTestErr ucCmdTok_get_arg_returns_null_if_followed_by_switch(ucTestGroup *p) {
    char cmd[] = "cmd\0-s\0a\0\n";
    ucTRUE(NULL == ucCmdTok_get_arg(cmd));
    ucPASS();
}

static ucTestErr ucCmdTok_get_switch_gets_first_switch(ucTestGroup *p) {
    char cmd[] = "c\0a\0a2\0-s1\0-s2\0\n";
    ucTRUE(ucTok_equals((ucTok*)ucCmdTok_get_switch(cmd), "-s1"));
    ucPASS();
}

static ucTestErr ucCmdTok_get_switch_returns_null_if_no_switch(ucTestGroup *p) {
    char cmd[] = "name\0a1\0ssss\0\n";
    ucTRUE(NULL == ucCmdTok_get_switch(cmd));
    ucPASS();
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
