#include <stdlib.h>
#include "ucBool_tests.h"
#include "ucTest.h"

static ucTestErr uc_true_causes_if_statement(ucTestGroup *p) {
    int entered = 0;
    if (ucBool_true) {
        entered = 1;
    }
    UC_TEST_ASSERT(entered == 1);

    if (!ucBool_true) {
        entered = 2;
    }
    UC_TEST_ASSERT(entered == 1);
    return ucTestErr_NONE;
}

static ucTestErr uc_false_avoids_if_statement(ucTestGroup *p) {
    int entered = 0;
    if (ucBool_false) {
        entered = 1;
    }
    UC_TEST_ASSERT(entered == 0);

    if (!ucBool_false) {
        entered = 2;
    }
    UC_TEST_ASSERT(entered == 2);
    return ucTestErr_NONE;
}

static ucTestErr uc_true_is_opposite_of_uc_false(ucTestGroup *p) {
    UC_TEST_ASSERT(ucBool_true == !ucBool_false);
    return ucTestErr_NONE;
}

static ucTestErr uc_false_is_opposite_of_uc_true(ucTestGroup *p) {
    UC_TEST_ASSERT(ucBool_false == !ucBool_true);
    return ucTestErr_NONE;
}

ucTestGroup *ucBool_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_test_func *tests[] = {
        uc_true_causes_if_statement,
        uc_false_avoids_if_statement,
        uc_true_is_opposite_of_uc_false,
        uc_false_is_opposite_of_uc_true,
        NULL
    };
    
    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
