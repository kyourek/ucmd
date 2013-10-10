#include <string.h>
#include "cl_opt_p.h"
#include "cl_opt_tests.h"
#include "cl_test.h"

static cl_test_err cl_opt_get_name_returns_name(cl_test_group *p) {
    cl_opt o;

    o.name = "name 1";
    CL_TEST_ASSERT(cl_opt_get_name(&o));

    o.name = "two NAME";
    CL_TEST_ASSERT(cl_opt_get_name(&o));

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_opt_get_desc_returns_description(cl_test_group *p) {
    cl_opt o;

    o.desc = "description one";
    CL_TEST_ASSERT(cl_opt_get_desc(&o));

    o.desc = "2 desc";
    CL_TEST_ASSERT(cl_opt_get_desc(&o));

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_opt_init_initializes_structure(cl_test_group *p) {
    cl_opt o;
    cl_opt *ptr = cl_opt_init(&o, "my_command_name", "Description of command.", CL_TRUE);
    CL_TEST_ASSERT(ptr == (&o));
    CL_TEST_ASSERT(cl_opt_get_name(ptr));
    CL_TEST_ASSERT(cl_opt_get_desc(ptr));
    CL_TEST_ASSERT(CL_TRUE == cl_opt_is_required(ptr));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_opt_is_required_returns_is_required(cl_test_group *p) {
    cl_opt o;
    o.is_required = CL_TRUE;
    CL_TEST_ASSERT(CL_TRUE == cl_opt_is_required(&o));
    o.is_required = CL_FALSE;
    CL_TEST_ASSERT(CL_FALSE == cl_opt_is_required(&o));
    return CL_TEST_ERR_NONE;
}

cl_test_group *cl_opt_tests_get_group(void) {
    static cl_test_group group;
    static cl_test_group_test_func *tests[] = {
        cl_opt_get_name_returns_name,
        cl_opt_get_desc_returns_description,
        cl_opt_init_initializes_structure,
        cl_opt_is_required_returns_is_required,
        NULL
    };

    return cl_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
