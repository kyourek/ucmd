#include <stdlib.h>
#include "cl_bool_tests.h"
#include "cl_test.h"

static cl_test_err cl_true_causes_if_statement(cl_test_group *p) {
    int entered = 0;
    if (CL_TRUE) {
        entered = 1;
    }
    CL_TEST_ASSERT(entered == 1);

    if (!CL_TRUE) {
        entered = 2;
    }
    CL_TEST_ASSERT(entered == 1);
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_false_avoids_if_statement(cl_test_group *p) {
    int entered = 0;
    if (CL_FALSE) {
        entered = 1;
    }
    CL_TEST_ASSERT(entered == 0);

    if (!CL_FALSE) {
        entered = 2;
    }
    CL_TEST_ASSERT(entered == 2);
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_true_is_opposite_of_cl_false(cl_test_group *p) {
    CL_TEST_ASSERT(CL_TRUE == !CL_FALSE);
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_false_is_opposite_of_cl_true(cl_test_group *p) {
    CL_TEST_ASSERT(CL_FALSE == !CL_TRUE);
    return CL_TEST_ERR_NONE;
}

cl_test_group *cl_bool_tests_get_group(void) {
    static cl_test_group group;
    static cl_test_group_test_func *tests[] = {
        cl_true_causes_if_statement,
        cl_false_avoids_if_statement,
        cl_true_is_opposite_of_cl_false,
        cl_false_is_opposite_of_cl_true,
        NULL
    };
    
    return cl_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
