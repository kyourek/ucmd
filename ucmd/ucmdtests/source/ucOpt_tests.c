#include <string.h>
#include "ucOpt_p.h"
#include "ucOpt_tests.h"
#include "ucTest.h"

static ucTestErr ucOpt_get_name_returns_name(ucTestGroup *p) {
    ucOpt o;

    o.name = "name 1";
    ucTest_ASSERT(ucOpt_get_name(&o));

    o.name = "two NAME";
    ucTest_ASSERT(ucOpt_get_name(&o));

    return ucTestErr_NONE;
}

static ucTestErr ucOpt_get_desc_returns_description(ucTestGroup *p) {
    ucOpt o;

    o.desc = "description one";
    ucTest_ASSERT(ucOpt_get_desc(&o));

    o.desc = "2 desc";
    ucTest_ASSERT(ucOpt_get_desc(&o));

    return ucTestErr_NONE;
}

static ucTestErr ucOpt_init_initializes_structure(ucTestGroup *p) {
    ucOpt o;
    ucOpt *ptr = ucOpt_init(&o, "my_command_name", "Description of command.", ucBool_true);
    ucTest_ASSERT(ptr == (&o));
    ucTest_ASSERT(ucOpt_get_name(ptr));
    ucTest_ASSERT(ucOpt_get_desc(ptr));
    ucTest_ASSERT(ucBool_true == ucOpt_is_required(ptr));
    return ucTestErr_NONE;
}

static ucTestErr ucOpt_is_required_returns_is_required(ucTestGroup *p) {
    ucOpt o;
    o.is_required = ucBool_true;
    ucTest_ASSERT(ucBool_true == ucOpt_is_required(&o));
    o.is_required = ucBool_false;
    ucTest_ASSERT(ucBool_false == ucOpt_is_required(&o));
    return ucTestErr_NONE;
}

ucTestGroup *ucOpt_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_test_func *tests[] = {
        ucOpt_get_name_returns_name,
        ucOpt_get_desc_returns_description,
        ucOpt_init_initializes_structure,
        ucOpt_is_required_returns_is_required,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
