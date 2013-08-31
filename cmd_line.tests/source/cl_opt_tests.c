#include <string.h>
#include "cl_opt.h"
#include "cl_opt_p.h"
#include "cl_opt_tests.h"
#include "cl_tests.h"

static cl_tests_err cl_opt_get_name_returns_name(void) {
    cl_opt o;

    o.name = "name 1";
    CL_TESTS_ASSERT(cl_opt_get_name(&o));

    o.name = "two NAME";
    CL_TESTS_ASSERT(cl_opt_get_name(&o));

    return CL_TESTS_NO_ERR;
}

static cl_tests_err cl_opt_get_desc_returns_description(void) {
    cl_opt o;

    o.desc = "description one";
    CL_TESTS_ASSERT(cl_opt_get_desc(&o));

    o.desc = "2 desc";
    CL_TESTS_ASSERT(cl_opt_get_desc(&o));

    return CL_TESTS_NO_ERR;
}

static cl_tests_err cl_opt_init_initializes_structure(void) {
    cl_opt o;
    cl_opt *p = cl_opt_init(&o, "my_command_name", "Description of command.", CL_TRUE);
    CL_TESTS_ASSERT(p == (&o));
    CL_TESTS_ASSERT(cl_opt_get_name(p));
    CL_TESTS_ASSERT(cl_opt_get_desc(p));
    CL_TESTS_ASSERT(CL_TRUE == cl_opt_is_required(p));
    return CL_TESTS_NO_ERR;
}

static cl_tests_err cl_opt_is_required_returns_is_required(void) {
    cl_opt o;
    char *message = "cl_opt_is_required did not return value.";
    o.is_required = CL_TRUE;
    CL_TESTS_ASSERT(CL_TRUE == cl_opt_is_required(&o));
    o.is_required = CL_FALSE;
    CL_TESTS_ASSERT(CL_FALSE == cl_opt_is_required(&o));
    return CL_TESTS_NO_ERR;
}

cl_tests_err cl_opt_tests(void) {
    CL_TESTS_RUN(cl_opt_get_name_returns_name);
    CL_TESTS_RUN(cl_opt_get_desc_returns_description);
    CL_TESTS_RUN(cl_opt_init_initializes_structure);
    CL_TESTS_RUN(cl_opt_is_required_returns_is_required);
    return CL_TESTS_NO_ERR;
}
