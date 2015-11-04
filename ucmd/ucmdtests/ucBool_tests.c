#include <stdlib.h>
#include "ucmdtests.h"

static ucTestErr uc_true_causes_if_statement(ucTestGroup *p) {
    int entered = 0;
    if (ucBool_true) {
        entered = 1;
    }
    ucTRUE(entered == 1);

    if (!ucBool_true) {
        entered = 2;
    }
    ucTRUE(entered == 1);
    ucPASS();
}

static ucTestErr uc_false_avoids_if_statement(ucTestGroup *p) {
    int entered = 0;
    if (ucBool_false) {
        entered = 1;
    }
    ucTRUE(entered == 0);

    if (!ucBool_false) {
        entered = 2;
    }
    ucTRUE(entered == 2);
    ucPASS();
}

static ucTestErr uc_true_is_opposite_of_uc_false(ucTestGroup *p) {
    ucTRUE(ucBool_true == !ucBool_false);
    ucPASS();
}

static ucTestErr uc_false_is_opposite_of_uc_true(ucTestGroup *p) {
    ucTRUE(ucBool_false == !ucBool_true);
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
