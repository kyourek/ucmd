#include <string.h>
#include "ucmdtests.h"

static ucTestErr ucOpt_get_name_returns_name(ucTestGroup *p) {
    ucOpt o;

    o.name = "name 1";
    ucTRUE(ucOpt_get_name(&o));

    o.name = "two NAME";
    ucTRUE(ucOpt_get_name(&o));

    ucPASS();
}

static ucTestErr ucOpt_get_desc_returns_description(ucTestGroup *p) {
    ucOpt o;

    o.desc = "description one";
    ucTRUE(ucOpt_get_desc(&o));

    o.desc = "2 desc";
    ucTRUE(ucOpt_get_desc(&o));

    ucPASS();
}

static ucTestErr ucOpt_init_initializes_structure(ucTestGroup *p) {
    ucOpt o;
    ucOpt *ptr = ucOpt_init(&o, "my_command_name", "Description of command.", ucBool_TRUE);
    ucTRUE(ptr == (&o));
    ucTRUE(ucOpt_get_name(ptr));
    ucTRUE(ucOpt_get_desc(ptr));
    ucTRUE(ucBool_TRUE == ucOpt_is_required(ptr));
    ucPASS();
}

static ucTestErr ucOpt_is_required_returns_is_required(ucTestGroup *p) {
    ucOpt o;
    o.is_required = ucBool_TRUE;
    ucTRUE(ucBool_TRUE == ucOpt_is_required(&o));
    o.is_required = ucBool_FALSE;
    ucTRUE(ucBool_FALSE == ucOpt_is_required(&o));
    ucPASS();
}

ucTestGroup *ucOpt_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_TestFunc *tests[] = {
        ucOpt_get_name_returns_name,
        ucOpt_get_desc_returns_description,
        ucOpt_init_initializes_structure,
        ucOpt_is_required_returns_is_required,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
