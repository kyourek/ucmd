#include <stddef.h>
#include "ucmdtests.h"

static ucTestErr ucArgTokOwner_get_arg_returns_first_arg(ucTestGroup *p) {
    char cmd[] = "c\0a1\0a2\0\n";
    ucTRUE(ucBool_true == ucTok_equals((ucTok*)ucArgTokOwner_get_arg(cmd), "a1"));
    ucPASS();
}

static ucTestErr ucArgTokOwner_get_arg_returns_null(ucTestGroup *p) {
    char cmd[] = "cmd\0\n";
    ucTRUE(NULL == ucArgTokOwner_get_arg(cmd));
    ucPASS();
}

ucTestGroup *ucArgTokOwner_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_TestFunc *tests[] = {
        ucArgTokOwner_get_arg_returns_first_arg,
        ucArgTokOwner_get_arg_returns_null,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
