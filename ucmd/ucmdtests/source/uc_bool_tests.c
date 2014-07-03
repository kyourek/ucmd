#include <stdlib.h>
#include "uc_bool_tests.h"
#include "uc_test.h"

static uc_test_err uc_true_causes_if_statement(uc_test_group *p) {
    int entered = 0;
    if (UC_TRUE) {
        entered = 1;
    }
    UC_TEST_ASSERT(entered == 1);

    if (!UC_TRUE) {
        entered = 2;
    }
    UC_TEST_ASSERT(entered == 1);
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_false_avoids_if_statement(uc_test_group *p) {
    int entered = 0;
    if (UC_FALSE) {
        entered = 1;
    }
    UC_TEST_ASSERT(entered == 0);

    if (!UC_FALSE) {
        entered = 2;
    }
    UC_TEST_ASSERT(entered == 2);
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_true_is_opposite_of_uc_false(uc_test_group *p) {
    UC_TEST_ASSERT(UC_TRUE == !UC_FALSE);
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_false_is_opposite_of_uc_true(uc_test_group *p) {
    UC_TEST_ASSERT(UC_FALSE == !UC_TRUE);
    return UC_TEST_ERR_NONE;
}

uc_test_group *uc_bool_tests_get_group(void) {
    static uc_test_group group;
    static uc_test_group_test_func *tests[] = {
        uc_true_causes_if_statement,
        uc_false_avoids_if_statement,
        uc_true_is_opposite_of_uc_false,
        uc_false_is_opposite_of_uc_true,
        NULL
    };
    
    return uc_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
