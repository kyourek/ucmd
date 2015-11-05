#include "ucmdtests.h"

static ucTestGroup **ucTestRunner_group_va(ucTestGroup *group, ...) {
    static ucTestGroup *groups[20];
    int i, len;
    va_list arg_list;

    groups[0] = group;
    va_start(arg_list, group);

    len = sizeof(groups) / sizeof(groups[0]);
    group = va_arg(arg_list, ucTestGroup*);
    for (i = 1; ((len - 1) > i); i++) {
        if (!group) break;
        groups[i] = group;
        group = va_arg(arg_list, ucTestGroup*);
    }
    groups[i] = NULL;

    va_end(arg_list);

    return groups;
}
