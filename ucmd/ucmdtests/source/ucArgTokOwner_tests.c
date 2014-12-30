#include <stddef.h>
#include "ucArgTokOwner.h"
#include "ucTest.h"
#include "ucTok.h"

static ucTestErr ucArgTokOwner_get_arg_returns_first_arg(ucTestGroup *p) {
    char cmd[] = "c\0a1\0a2\0\n";
    ucTest_ASSERT(ucBool_TRUE == ucTok_equals((ucTok*)ucArgTokOwner_get_arg(cmd), "a1"));
    return ucTestErr_NONE;
}

static ucTestErr ucArgTokOwner_get_arg_returns_null(ucTestGroup *p) {
    char cmd[] = "cmd\0\n";
    ucTest_ASSERT(NULL == ucArgTokOwner_get_arg(cmd));
    return ucTestErr_NONE;
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
