#include <stdlib.h>
#include "cl_cmd_line_toks.h"
#include "cl_cmd_line_toks_tests.h"
#include "cl_test.h"

static cl_test_err cl_cmd_line_toks_get_cmd_tok_returns_value(cl_test_group *p) {
    cl_cmd_line_toks inst = { 0 };
    cl_cmd_line_toks *ptr = &inst;
    cl_cmd_tok *cmd_tok = "c";

    CL_TEST_ASSERT(NULL == cl_cmd_line_toks_get_cmd_tok(NULL));
    CL_TEST_ASSERT(NULL == cl_cmd_line_toks_get_cmd_tok(ptr));

    ptr->cmd_tok = cmd_tok;
    CL_TEST_ASSERT(cmd_tok == cl_cmd_line_toks_get_cmd_tok(ptr));

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_toks_get_arg_tok_returns_value(cl_test_group *p) {
    cl_cmd_line_toks inst = { 0 };
    cl_cmd_line_toks *ptr = &inst;
    cl_arg_tok *arg_tok = "a";

    CL_TEST_ASSERT(NULL == cl_cmd_line_toks_get_arg_tok(NULL));
    CL_TEST_ASSERT(NULL == cl_cmd_line_toks_get_arg_tok(ptr));

    ptr->arg_tok = arg_tok;
    CL_TEST_ASSERT(arg_tok == cl_cmd_line_toks_get_arg_tok(ptr));

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_toks_get_switch_tok_returns_value(cl_test_group *p) {
    cl_cmd_line_toks inst = { 0 };
    cl_cmd_line_toks *ptr = &inst;
    cl_arg_tok *switch_tok = "-s";

    CL_TEST_ASSERT(NULL == cl_cmd_line_toks_get_switch_tok(NULL));
    CL_TEST_ASSERT(NULL == cl_cmd_line_toks_get_switch_tok(ptr));

    ptr->switch_tok = switch_tok;
    CL_TEST_ASSERT(switch_tok == cl_cmd_line_toks_get_switch_tok(ptr));

    return CL_TEST_ERR_NONE;
}

cl_test_group *cl_cmd_line_toks_tests_get_group(void) {
    static cl_test_group group;
    static cl_test_group_test_func *tests[] = {
        cl_cmd_line_toks_get_cmd_tok_returns_value,
        cl_cmd_line_toks_get_arg_tok_returns_value,
        cl_cmd_line_toks_get_switch_tok_returns_value,
        NULL
    };

    return cl_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
