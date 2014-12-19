#include <string.h>
#include "uc_cmd_tok.h"
#include "uc_cmd_tok_tests.h"
#include "uc_test.h"
#include "uc_tok.h"

static uc_test_err uc_cmd_tok_get_arg_gets_arg(uc_test_group *p) {
    char cmd[] = "command\0arg\0\n";
    UC_TEST_ASSERT(ucBool_true == uc_tok_equals((uc_tok*)uc_cmd_tok_get_arg(cmd), "arg"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_tok_get_arg_returns_null_if_followed_by_switch(uc_test_group *p) {
    char cmd[] = "cmd\0-s\0a\0\n";
    UC_TEST_ASSERT(NULL == uc_cmd_tok_get_arg(cmd));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_tok_get_switch_gets_first_switch(uc_test_group *p) {
    char cmd[] = "c\0a\0a2\0-s1\0-s2\0\n";
    UC_TEST_ASSERT(uc_tok_equals((uc_tok*)uc_cmd_tok_get_switch(cmd), "-s1"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_tok_get_switch_returns_null_if_no_switch(uc_test_group *p) {
    char cmd[] = "name\0a1\0ssss\0\n";
    UC_TEST_ASSERT(NULL == uc_cmd_tok_get_switch(cmd));
    return UC_TEST_ERR_NONE;
}

uc_test_group *uc_cmd_tok_tests_get_group(void) {
    static uc_test_group group;
    static uc_test_group_test_func *tests[] = {
        uc_cmd_tok_get_arg_gets_arg,
        uc_cmd_tok_get_arg_returns_null_if_followed_by_switch,
        uc_cmd_tok_get_switch_gets_first_switch,
        uc_cmd_tok_get_switch_returns_null_if_no_switch,
        NULL
    };

    return uc_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
