#include <stdarg.h>
#include <stdlib.h>
#include "uc_arg_opt_tests.h"
#include "uc_arg_tok_owner_tests.h"
#include "uc_arg_tok_tests.h"
#include "uc_bool_tests.h"
#include "uc_cmd_parser_tests.h"
#include "uc_cmd_tok_tests.h"
#include "uc_opt_tests.h"
#include "uc_switch_opt_tests.h"
#include "uc_switch_tok_tests.h"
#include "uc_test.h"
#include "uc_tok_tests.h"
#include "uc_cmd_line_app_tests.h"
#include "uc_cmd_line_opt_tests.h"
#include "uc_cmd_line_toks_tests.h"
#include "uc_cmd_line_tests.h"

static uc_test_group **uc_tests_get_groups_va(uc_test_group *group, ...) {
    static uc_test_group *groups[20];
    int i, len;
    va_list arg_list;

    groups[0] = group;
    va_start(arg_list, group);

    len = sizeof(groups) / sizeof(groups[0]);
    group = va_arg(arg_list, uc_test_group*);
    for (i = 1; ((len - 1) > i); i++) {
        if (NULL == group) break;
        groups[i] = group;
        group = va_arg(arg_list, uc_test_group*);
    }

    groups[i] = NULL;

    va_end(arg_list);

    return groups;
}

static uc_test_group **uc_tests_get_groups(void) {
    return uc_tests_get_groups_va(
        uc_arg_opt_tests_get_group(),
        uc_arg_tok_owner_tests_get_group(),
        uc_arg_tok_tests_get_group(),
        uc_bool_tests_get_group(),
        uc_cmd_parser_tests_get_group(),
        uc_cmd_tok_tests_get_group(),
        uc_opt_tests_get_group(),
        uc_switch_opt_tests_get_group(),
        uc_switch_tok_tests_get_group(),
        uc_tok_tests_get_group(),
        uc_cmd_line_app_tests_get_group(),
        uc_cmd_line_opt_tests_get_group(),
        uc_cmd_line_tests_get_group(),
        uc_cmd_line_toks_tests_get_group(),
        NULL
    );
}

uc_test *uc_tests_get_test(void) {
    static uc_test test;
    return uc_test_init(&test, "ucmd tests", uc_tests_get_groups());
}
