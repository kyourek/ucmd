#include <stdlib.h>
#include "ucmdtests.h"

static ucTestErr uc_true_causes_if_statement(ucTestGroup *p) {
    int entered = 0;
    if (ucBool_TRUE) {
        entered = 1;
    }
    ucTEST(entered == 1);

    if (!ucBool_TRUE) {
        entered = 2;
    }
    ucTEST(entered == 1);
    ucPASS();
}

static ucTestErr uc_false_avoids_if_statement(ucTestGroup *p) {
    int entered = 0;
    if (ucBool_FALSE) {
        entered = 1;
    }
    ucTEST(entered == 0);

    if (!ucBool_FALSE) {
        entered = 2;
    }
    ucTEST(entered == 2);
    ucPASS();
}

static ucTestErr uc_true_is_opposite_of_uc_false(ucTestGroup *p) {
    ucTEST(ucBool_TRUE == !ucBool_FALSE);
    ucPASS();
}

static ucTestErr uc_false_is_opposite_of_uc_true(ucTestGroup *p) {
    ucTEST(ucBool_FALSE == !ucBool_TRUE);
    ucPASS();
}

ucTestGroup *ucBool_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_TestFunc *tests[] = {
        uc_true_causes_if_statement,
        uc_false_avoids_if_statement,
        uc_true_is_opposite_of_uc_false,
        uc_false_is_opposite_of_uc_true,
        NULL
    };
    
    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
