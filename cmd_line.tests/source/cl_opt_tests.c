#include <string.h>
#include "cl_opt.h"
#include "cl_opt_p.h"
#include "cl_opt_tests.h"
#include "cl_tests.h"

static char *cl_opt_get_name_returns_name(void) {
    cl_opt o;

    o.name = "name 1";
    cl_assert("cl_opt_get_name returned incorrect name.", 0 == strcmp("name 1", cl_opt_get_name(&o)));

    o.name = "two NAME";
    cl_assert("cl_opt_get_name returned incorrect name.", 0 == strcmp("two NAME", cl_opt_get_name(&o)));

    return 0;
}

static char *cl_opt_get_desc_returns_description(void) {
    cl_opt o;

    o.desc = "description one";
    cl_assert("cl_opt_get_desc returned incorrect description.", 0 == strcmp("description one", cl_opt_get_desc(&o)));

    o.desc = "2 desc";
    cl_assert("cl_opt_get_desc returned incorrect description.", 0 == strcmp("2 desc", cl_opt_get_desc(&o)));

    return 0;
}

static char *cl_opt_init_initializes_structure(void) {
    cl_opt o;
    cl_opt *p = cl_opt_init(&o, "my_command_name", "Description of command.", CL_TRUE);
    cl_assert("cl_opt_init did not return correct pointer.", p == (&o));
    cl_assert("cl_opt_init did not set name.", 0 == strcmp("my_command_name", cl_opt_get_name(p)));
    cl_assert("cl_opt_init did not set desc.", 0 == strcmp("Description of command.", cl_opt_get_desc(p)));
    cl_assert("cl_opt_init did not set is_required.", CL_TRUE == cl_opt_is_required(p));
    return 0;
}

static char *cl_opt_is_required_returns_is_required(void) {
    cl_opt o;
    char *message = "cl_opt_is_required did not return value.";
    o.is_required = CL_TRUE;
    cl_assert(message, CL_TRUE == cl_opt_is_required(&o));
    o.is_required = CL_FALSE;
    cl_assert(message, CL_FALSE == cl_opt_is_required(&o));
    return 0;
}

char *cl_opt_tests(void) {
    cl_run_test(cl_opt_get_name_returns_name);
    cl_run_test(cl_opt_get_desc_returns_description);
    cl_run_test(cl_opt_init_initializes_structure);
    cl_run_test(cl_opt_is_required_returns_is_required);
    return 0;
}
