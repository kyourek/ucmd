#include <stdarg.h>
#include <stdlib.h>
#include "cl_arg_opt_tests.h"
#include "cl_arg_tok_owner_tests.h"
#include "cl_arg_tok_tests.h"
#include "cl_bool_tests.h"
#include "cl_cmd_parser_tests.h"
#include "cl_cmd_tok_tests.h"
#include "cl_opt_tests.h"
#include "cl_switch_opt_tests.h"
#include "cl_switch_tok_tests.h"
#include "cl_test.h"
#include "cl_tok_tests.h"
#include "cl_cmd_line_app_tests.h"
#include "cl_cmd_line_opt_tests.h"
#include "cl_cmd_line_toks_tests.h"
#include "cl_cmd_line_tests.h"

static cl_test_group **cl_tests_get_groups_va(cl_test_group *group, ...) {
    static cl_test_group *groups[20];
    int i, len;
    va_list arg_list;

    groups[0] = group;
    va_start(arg_list, group);

    len = sizeof(groups) / sizeof(groups[0]);
    group = va_arg(arg_list, cl_test_group*);
    for (i = 1; ((len - 1) > i); i++) {
        if (NULL == group) break;
        groups[i] = group;
        group = va_arg(arg_list, cl_test_group*);
    }

    groups[i] = NULL;

    va_end(arg_list);

    return groups;
}

static cl_test_group **cl_tests_get_groups(void) {
    return cl_tests_get_groups_va(
        cl_arg_opt_tests_get_group(),
        cl_arg_tok_owner_tests_get_group(),
        cl_arg_tok_tests_get_group(),
        cl_bool_tests_get_group(),
        cl_cmd_parser_tests_get_group(),
        cl_cmd_tok_tests_get_group(),
        cl_opt_tests_get_group(),
        cl_switch_opt_tests_get_group(),
        cl_switch_tok_tests_get_group(),
        cl_tok_tests_get_group(),
        cl_cmd_line_app_tests_get_group(),
        cl_cmd_line_opt_tests_get_group(),
        cl_cmd_line_tests_get_group(),
        cl_cmd_line_toks_tests_get_group(),
        NULL
    );
}

cl_test *cl_tests_get_test(void) {
    static cl_test test;
    return cl_test_init(&test, "cmd_line tests", cl_tests_get_groups());
}
