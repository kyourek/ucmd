#include <stddef.h>
#include "cl_arg_tok_owner.h"
#include "cl_arg_tok_owner_tests.h"
#include "cl_test.h"
#include "cl_tok.h"

static cl_test_err cl_arg_tok_owner_get_arg_returns_first_arg(cl_test_group *p) {
    char cmd[] = "c\0a1\0a2\0\n";
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_arg_tok_owner_get_arg(cmd), "a1"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_tok_owner_get_arg_returns_null(cl_test_group *p) {
    char cmd[] = "cmd\0\n";
    CL_TEST_ASSERT(NULL == cl_arg_tok_owner_get_arg(cmd));
    return CL_TEST_ERR_NONE;
}

cl_test_group *cl_arg_tok_owner_tests_get_group(void) {
    static cl_test_group group;
    static cl_test_group_test_func *tests[] = {
        cl_arg_tok_owner_get_arg_returns_first_arg,
        cl_arg_tok_owner_get_arg_returns_null,
        NULL
    };

    return cl_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
