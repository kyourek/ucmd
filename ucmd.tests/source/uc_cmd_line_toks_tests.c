#include <stdlib.h>
#include "uc_cmd_line_toks.h"
#include "uc_cmd_line_toks_tests.h"
#include "uc_test.h"

static uc_test_err uc_cmd_line_toks_get_cmd_tok_returns_value(uc_test_group *p) {
    uc_cmd_line_toks inst = { 0 };
    uc_cmd_line_toks *ptr = &inst;
    uc_cmd_tok *cmd_tok = "c";

    UC_TEST_ASSERT(NULL == uc_cmd_line_toks_get_cmd_tok(NULL));
    UC_TEST_ASSERT(NULL == uc_cmd_line_toks_get_cmd_tok(ptr));

    ptr->cmd_tok = cmd_tok;
    UC_TEST_ASSERT(cmd_tok == uc_cmd_line_toks_get_cmd_tok(ptr));

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_toks_get_arg_tok_returns_value(uc_test_group *p) {
    uc_cmd_line_toks inst = { 0 };
    uc_cmd_line_toks *ptr = &inst;
    uc_arg_tok *arg_tok = "a";

    UC_TEST_ASSERT(NULL == uc_cmd_line_toks_get_arg_tok(NULL));
    UC_TEST_ASSERT(NULL == uc_cmd_line_toks_get_arg_tok(ptr));

    ptr->arg_tok = arg_tok;
    UC_TEST_ASSERT(arg_tok == uc_cmd_line_toks_get_arg_tok(ptr));

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_toks_get_switch_tok_returns_value(uc_test_group *p) {
    uc_cmd_line_toks inst = { 0 };
    uc_cmd_line_toks *ptr = &inst;
    uc_arg_tok *switch_tok = "-s";

    UC_TEST_ASSERT(NULL == uc_cmd_line_toks_get_switch_tok(NULL));
    UC_TEST_ASSERT(NULL == uc_cmd_line_toks_get_switch_tok(ptr));

    ptr->switch_tok = switch_tok;
    UC_TEST_ASSERT(switch_tok == uc_cmd_line_toks_get_switch_tok(ptr));

    return UC_TEST_ERR_NONE;
}

uc_test_group *uc_cmd_line_toks_tests_get_group(void) {
    static uc_test_group group;
    static uc_test_group_test_func *tests[] = {
        uc_cmd_line_toks_get_cmd_tok_returns_value,
        uc_cmd_line_toks_get_arg_tok_returns_value,
        uc_cmd_line_toks_get_switch_tok_returns_value,
        NULL
    };

    return uc_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
