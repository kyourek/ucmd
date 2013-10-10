#include <string.h>
#include "cl_cmd_tok.h"
#include "cl_cmd_tok_tests.h"
#include "cl_test.h"
#include "cl_tok.h"

static cl_test_err cl_cmd_tok_get_arg_gets_arg(cl_test_group *p) {
    char cmd[] = "command\0arg\0\n";
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_cmd_tok_get_arg(cmd), "arg"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_tok_get_arg_returns_null_if_followed_by_switch(cl_test_group *p) {
    char cmd[] = "cmd\0-s\0a\0\n";
    CL_TEST_ASSERT(NULL == cl_cmd_tok_get_arg(cmd));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_tok_get_switch_gets_first_switch(cl_test_group *p) {
    char cmd[] = "c\0a\0a2\0-s1\0-s2\0\n";
    CL_TEST_ASSERT(cl_tok_equals((cl_tok*)cl_cmd_tok_get_switch(cmd), "-s1"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_tok_get_switch_returns_null_if_no_switch(cl_test_group *p) {
    char cmd[] = "name\0a1\0ssss\0\n";
    CL_TEST_ASSERT(NULL == cl_cmd_tok_get_switch(cmd));
    return CL_TEST_ERR_NONE;
}

cl_test_group *cl_cmd_tok_tests_get_group(void) {
    static cl_test_group group;
    static cl_test_group_test_func *tests[] = {
        cl_cmd_tok_get_arg_gets_arg,
        cl_cmd_tok_get_arg_returns_null_if_followed_by_switch,
        cl_cmd_tok_get_switch_gets_first_switch,
        cl_cmd_tok_get_switch_returns_null_if_no_switch,
        NULL
    };

    return cl_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
