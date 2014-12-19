#include <stdarg.h>
#include <stdlib.h>
#include "ucArgOpt_tests.h"
#include "ucArgTokOwner_tests.h"
#include "ucArgTok_tests.h"
#include "ucBool_tests.h"
#include "ucCmdParser_tests.h"
#include "ucCmdTok_tests.h"
#include "ucOpt_tests.h"
#include "ucSwitchOpt_tests.h"
#include "ucSwitchTok_tests.h"
#include "ucTest.h"
#include "ucTok_tests.h"
#include "ucCmdLineApp_tests.h"
#include "ucCmdLineOpt_tests.h"
#include "ucCmdLineToks_tests.h"
#include "ucCmdLine_tests.h"

static ucTestGroup **ucTests_get_groups_va(ucTestGroup *group, ...) {
    static ucTestGroup *groups[20];
    int i, len;
    va_list arg_list;

    groups[0] = group;
    va_start(arg_list, group);

    len = sizeof(groups) / sizeof(groups[0]);
    group = va_arg(arg_list, ucTestGroup*);
    for (i = 1; ((len - 1) > i); i++) {
        if (NULL == group) break;
        groups[i] = group;
        group = va_arg(arg_list, ucTestGroup*);
    }

    groups[i] = NULL;

    va_end(arg_list);

    return groups;
}

static ucTestGroup **ucTests_get_groups(void) {
    return ucTests_get_groups_va(
        ucArgOpt_tests_get_group(),
        ucArgTokOwner_tests_get_group(),
        ucArgTok_tests_get_group(),
        ucBool_tests_get_group(),
        ucCmdParser_tests_get_group(),
        ucCmdTok_tests_get_group(),
        ucOpt_tests_get_group(),
        ucSwitchOpt_tests_get_group(),
        ucSwitchTok_tests_get_group(),
        ucTok_tests_get_group(),
        ucCmdLineApp_tests_get_group(),
        ucCmdLineOpt_tests_get_group(),
        ucCmdLine_tests_get_group(),
        ucCmdLineToks_tests_get_group(),
        NULL
    );
}

ucTest *ucTests_get_test(void) {
    static ucTest test;
    return ucTest_init(&test, "ucmd tests", ucTests_get_groups());
}
