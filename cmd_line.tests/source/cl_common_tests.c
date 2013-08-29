#include "cl_common.h"
#include "cl_common_tests.h"
#include "cl_tests.h"

static char *cl_true_causes_if_statement(void) {
    int entered = 0;
    if (CL_TRUE) {
        entered = 1;
    }
    cl_assert("CL_TRUE did not enter if statement.", entered == 1);

    if (!CL_TRUE) {
        entered = 2;
    }
    cl_assert("!CL_TRUE entered if statement.", entered == 1);
    return 0;
}

static char *cl_false_avoids_if_statement(void) {
    int entered = 0;
    if (CL_FALSE) {
        entered = 1;
    }
    cl_assert("CL_FALSE entered if statement.", entered == 0);

    if (!CL_FALSE) {
        entered = 2;
    }
    cl_assert("!CL_FALSE did not enter if statement.", entered == 2);
    return 0;
}

static char *cl_true_is_opposite_of_cl_false(void) {
    cl_assert("CL_TRUE is not opposite of CL_FALSE.", CL_TRUE == !CL_FALSE);
    return 0;
}

static char *cl_false_is_opposite_of_cl_true(void) {
    cl_assert("CL_FALSE is not opposite of CL_TRUE.", CL_FALSE == !CL_TRUE);
    return 0;
}

char *cl_common_tests(void) {
    cl_run_test(cl_true_causes_if_statement);
    cl_run_test(cl_false_avoids_if_statement);
    cl_run_test(cl_true_is_opposite_of_cl_false);
    cl_run_test(cl_false_is_opposite_of_cl_true);
    return 0;
}
