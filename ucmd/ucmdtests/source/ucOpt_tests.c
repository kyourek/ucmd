#include <string.h>
#include "uc_opt_p.h"
#include "uc_opt_tests.h"
#include "uc_test.h"

static uc_test_err uc_opt_get_name_returns_name(uc_test_group *p) {
    uc_opt o;

    o.name = "name 1";
    UC_TEST_ASSERT(uc_opt_get_name(&o));

    o.name = "two NAME";
    UC_TEST_ASSERT(uc_opt_get_name(&o));

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_opt_get_desc_returns_description(uc_test_group *p) {
    uc_opt o;

    o.desc = "description one";
    UC_TEST_ASSERT(uc_opt_get_desc(&o));

    o.desc = "2 desc";
    UC_TEST_ASSERT(uc_opt_get_desc(&o));

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_opt_init_initializes_structure(uc_test_group *p) {
    uc_opt o;
    uc_opt *ptr = uc_opt_init(&o, "my_command_name", "Description of command.", ucBool_true);
    UC_TEST_ASSERT(ptr == (&o));
    UC_TEST_ASSERT(uc_opt_get_name(ptr));
    UC_TEST_ASSERT(uc_opt_get_desc(ptr));
    UC_TEST_ASSERT(ucBool_true == uc_opt_is_required(ptr));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_opt_is_required_returns_is_required(uc_test_group *p) {
    uc_opt o;
    o.is_required = ucBool_true;
    UC_TEST_ASSERT(ucBool_true == uc_opt_is_required(&o));
    o.is_required = ucBool_false;
    UC_TEST_ASSERT(ucBool_false == uc_opt_is_required(&o));
    return UC_TEST_ERR_NONE;
}

uc_test_group *uc_opt_tests_get_group(void) {
    static uc_test_group group;
    static uc_test_group_test_func *tests[] = {
        uc_opt_get_name_returns_name,
        uc_opt_get_desc_returns_description,
        uc_opt_init_initializes_structure,
        uc_opt_is_required_returns_is_required,
        NULL
    };

    return uc_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
