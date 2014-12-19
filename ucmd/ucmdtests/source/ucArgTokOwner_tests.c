#include <stddef.h>
#include "ucArgTokOwner.h"
#include "ucArgTokOwner_tests.h"
#include "uc_test.h"
#include "uc_tok.h"

static uc_test_err ucArgTokOwner_get_arg_returns_first_arg(uc_test_group *p) {
    char cmd[] = "c\0a1\0a2\0\n";
    UC_TEST_ASSERT(ucBool_true == uc_tok_equals((uc_tok*)ucArgTokOwner_get_arg(cmd), "a1"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgTokOwner_get_arg_returns_null(uc_test_group *p) {
    char cmd[] = "cmd\0\n";
    UC_TEST_ASSERT(NULL == ucArgTokOwner_get_arg(cmd));
    return UC_TEST_ERR_NONE;
}

uc_test_group *ucArgTokOwner_tests_get_group(void) {
    static uc_test_group group;
    static uc_test_group_test_func *tests[] = {
        ucArgTokOwner_get_arg_returns_first_arg,
        ucArgTokOwner_get_arg_returns_null,
        NULL
    };

    return uc_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
