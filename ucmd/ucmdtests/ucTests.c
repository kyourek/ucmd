#include <stdarg.h>
#include <stdlib.h>
#include "ucmdtests.h"

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
        ucTestGroup_ucArgOpt(),
        ucTestGroup_ucArgOptOwner(),
        ucTestGroup_ucArgTokOwner(),
        ucTestGroup_ucArgTok(),
        ucTestGroup_ucBool(),
        ucTestGroup_ucCmdParser(),
        ucTestGroup_ucCmdTok(),
        ucTestGroup_ucOpt(),
        ucTestGroup_ucSwitchOpt(),
        ucTestGroup_ucSwitchTok(),
        ucTestGroup_ucTok(),
        ucTestGroup_ucCmdLineApp(),
        ucTestGroup_ucCmdLineOpt(),
        ucTestGroup_ucCmdLine(),
        ucTestGroup_ucCmdLineToks(),
        NULL
    );
}

ucTest *ucTests_get_test(void) {
    static ucTest test;
    return ucTest_init(&test, "ucmd tests", ucTests_get_groups());
}
