#include <stdlib.h>
#include "ucmdtests.h"

uc_TEST(uc_true_causes_if_statement)
    int entered = 0;
    if (ucBool_true) {
        entered = 1;
    }
    uc_TRUE(entered == 1);

    if (!ucBool_true) {
        entered = 2;
    }
    uc_TRUE(entered == 1);
uc_PASS

uc_TEST(uc_false_avoids_if_statement)
    int entered = 0;
    if (ucBool_false) {
        entered = 1;
    }
    uc_TRUE(entered == 0);

    if (!ucBool_false) {
        entered = 2;
    }
    uc_TRUE(entered == 2);
uc_PASS

uc_TEST(uc_true_is_opposite_of_uc_false)
    uc_TRUE(ucBool_true == !ucBool_false);
uc_PASS

uc_TEST(uc_false_is_opposite_of_uc_true)
    uc_TRUE(ucBool_false == !ucBool_true);
uc_PASS

uc_TEST_GROUP(ucBool, NULL,
    uc_true_causes_if_statement,
    uc_false_avoids_if_statement,
    uc_true_is_opposite_of_uc_false,
    uc_false_is_opposite_of_uc_true)
