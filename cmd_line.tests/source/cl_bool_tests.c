#include "cl_bool_tests.h"
#include "cl_tests_p.h"

static cl_tests_err cl_true_causes_if_statement(void) {
    int entered = 0;
    if (CL_TRUE) {
        entered = 1;
    }
    CL_TESTS_ASSERT(entered == 1);

    if (!CL_TRUE) {
        entered = 2;
    }
    CL_TESTS_ASSERT(entered == 1);
    return CL_TESTS_ERR_NONE;
}

static cl_tests_err cl_false_avoids_if_statement(void) {
    int entered = 0;
    if (CL_FALSE) {
        entered = 1;
    }
    CL_TESTS_ASSERT(entered == 0);

    if (!CL_FALSE) {
        entered = 2;
    }
    CL_TESTS_ASSERT(entered == 2);
    return CL_TESTS_ERR_NONE;
}

static cl_tests_err cl_true_is_opposite_of_cl_false(void) {
    CL_TESTS_ASSERT(CL_TRUE == !CL_FALSE);
    return CL_TESTS_ERR_NONE;
}

static cl_tests_err cl_false_is_opposite_of_cl_true(void) {
    CL_TESTS_ASSERT(CL_FALSE == !CL_TRUE);
    return CL_TESTS_ERR_NONE;
}

CL_EXPORTED cl_tests_err cl_bool_tests(void) {
    CL_TESTS_RUN(cl_true_causes_if_statement);
    CL_TESTS_RUN(cl_false_avoids_if_statement);
    CL_TESTS_RUN(cl_true_is_opposite_of_cl_false);
    CL_TESTS_RUN(cl_false_is_opposite_of_cl_true);
    return CL_TESTS_ERR_NONE;
}
