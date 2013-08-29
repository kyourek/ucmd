#include <string.h>
#include "cl_arg_opt.h"
#include "cl_arg_opt_p.h"
#include "cl_memory_manager_p.h"
#include "cl_tests.h"

static char *cl_arg_opt_is_numeric_returns_is_numeric(void) {
    cl_arg_opt o;
    
    o.is_numeric = CL_FALSE;
    cl_assert("cl_arg_opt_is_numeric should have returned CL_FALSE.", CL_FALSE == cl_arg_opt_is_numeric(&o));

    o.is_numeric = CL_TRUE;
    cl_assert("cl_arg_opt_is_numeric should have returned CL_TRUE.", CL_TRUE == cl_arg_opt_is_numeric(&o));

    return 0;
}

static char *cl_arg_opt_get_numeric_min_returns_value(void) {
    cl_arg_opt o;
    o.numeric_min = -765.432;
    cl_assert("cl_arg_opt_get_numeric_min did not return value.", -765.432 == cl_arg_opt_get_numeric_min(&o));
    return 0;
}

static char *cl_arg_opt_get_numeric_max_returns_value(void) {
    cl_arg_opt o;
    o.numeric_max = 0.123456789;
    cl_assert("cl_arg_opt_get_numeric_max did not return value.", 0.123456789 == cl_arg_opt_get_numeric_max(&o));
    return 0;
}

static char *cl_arg_opt_create_creates_arg_opt(void) {
    cl_arg_opt *p;
    cl_arg_opt a;
    
    p = cl_arg_opt_create("a-name", "a-desc", &a);
    
    cl_assert("cl_arg_opt_create did not set name.", 0 == strcmp("a-name", cl_opt_get_name((cl_opt*)p)));
    cl_assert("cl_arg_opt_create did not set desc.", 0 == strcmp("a-desc", cl_opt_get_desc((cl_opt*)p)));
    cl_assert("cl_arg_opt_create did not set next.", &a == cl_arg_opt_get_next(p));
    cl_assert("cl_arg_opt_create did not set is_required.", CL_FALSE == cl_opt_is_required((cl_opt*)p));
    cl_assert("cl_arg_opt_create did not set is_numeric.", CL_FALSE == cl_arg_opt_is_numeric(p));

    cl_arg_opt_destroy(p);

    return 0;
}

static char *cl_arg_opt_create_required_creates_arg_opt(void) {
    cl_arg_opt *p;
    cl_arg_opt a;

    p = cl_arg_opt_create_required("aname", "adesc", &a);

    cl_assert("cl_arg_opt_create_required did not set name.", 0 == strcmp("aname", cl_opt_get_name((cl_opt*)p)));
    cl_assert("cl_arg_opt_create_required did not set desc.", 0 == strcmp("adesc", cl_opt_get_desc((cl_opt*)p)));
    cl_assert("cl_arg_opt_create_required did not set next.", &a == cl_arg_opt_get_next(p));
    cl_assert("cl_arg_opt_create_required did not set is_required.", CL_TRUE == cl_opt_is_required((cl_opt*)p));
    cl_assert("cl_arg_opt_create_required did not set is_numeric.", CL_FALSE == cl_arg_opt_is_numeric(p));

    cl_arg_opt_destroy(p);

    return 0;
}

static char *cl_arg_opt_create_numeric_creates_arg_opt(void) {
    cl_arg_opt *p;
    cl_arg_opt a;
    
    p = cl_arg_opt_create_numeric("ds", -5.678, 12.34, &a);
    
    cl_assert("cl_arg_opt_create_numeric did not set name.", 0 == strcmp("<number>", cl_opt_get_name((cl_opt*)p)));
    cl_assert("cl_arg_opt_create_numeric did not set desc.", 0 == strcmp("ds", cl_opt_get_desc((cl_opt*)p)));
    cl_assert("cl_arg_opt_create_numeric did not set next.", &a == cl_arg_opt_get_next(p));
    cl_assert("cl_arg_opt_create_numeric did not set is_required.", CL_FALSE == cl_opt_is_required((cl_opt*)p));
    cl_assert("cl_arg_opt_create_numeric did not set is_numeric.", CL_TRUE == cl_arg_opt_is_numeric(p));
    cl_assert("cl_arg_opt_create_numeric did not set numeric_min.", -5.678 == cl_arg_opt_get_numeric_min(p));
    cl_assert("cl_arg_opt_create_numeric did not set numeric_max.", 12.34 == cl_arg_opt_get_numeric_max(p));

    cl_arg_opt_destroy(p);

    return 0;
}

static char *cl_arg_opt_create_required_numeric_creates_arg_opt(void) {
    cl_arg_opt *p;
    cl_arg_opt a;
    
    p = cl_arg_opt_create_required_numeric("REQ", 100.436, 567.890, &a);
    
    cl_assert("cl_arg_opt_create_required_numeric did not set name.", 0 == strcmp("<number>", cl_opt_get_name((cl_opt*)p)));
    cl_assert("cl_arg_opt_create_required_numeric did not set desc.", 0 == strcmp("REQ", cl_opt_get_desc((cl_opt*)p)));
    cl_assert("cl_arg_opt_create_required_numeric did not set next.", &a == cl_arg_opt_get_next(p));
    cl_assert("cl_arg_opt_create_required_numeric did not set is_required.", CL_TRUE == cl_opt_is_required((cl_opt*)p));
    cl_assert("cl_arg_opt_create_required_numeric did not set is_numeric.", CL_TRUE == cl_arg_opt_is_numeric(p));
    cl_assert("cl_arg_opt_create_required_numeric did not set numeric_min.", 100.436 == cl_arg_opt_get_numeric_min(p));
    cl_assert("cl_arg_opt_create_required_numeric did not set numeric_max.", 567.890 == cl_arg_opt_get_numeric_max(p));

    cl_arg_opt_destroy(p);

    return 0;
}

static char *cl_arg_opt_create_creates_different_instances(void) {
    cl_arg_opt *o1, *o2, *o3;
    char *message = "cl_arg_opt_create did not create different instances.";

    o1 = cl_arg_opt_create("o1", "o1", NULL);
    o2 = cl_arg_opt_create("o2", "o2", NULL);
    o3 = cl_arg_opt_create("o3", "o3", NULL);

    cl_assert(message, o1 != o2);
    cl_assert(message, o2 != o3);

    cl_arg_opt_destroy(o1);
    cl_arg_opt_destroy(o2);
    cl_arg_opt_destroy(o3);

    return 0;
}

static char *cl_arg_opt_destroy_releases_instance(void) {
    cl_arg_opt *o1, *o2, *o3;

    o1 = cl_arg_opt_create("o1", "o1", NULL);
    o2 = cl_arg_opt_create("o2", "o2", NULL);

    cl_arg_opt_destroy(o2);

    o3 = cl_arg_opt_create("o3", "o3", NULL);

    cl_assert("cl_arg_opt_destroy did not release an instance.", o3 == o2);

    cl_arg_opt_destroy(o1);
    cl_arg_opt_destroy(o3);

    return 0;
}

static char *cl_arg_opt_destroy_chain_releases_all_instances(void) {
    cl_arg_opt *o1, *o2, *o3, *o1_2, *o2_2, *o3_2;
    char *message = "cl_arg_opt_destroy_chain did not release all instances.";

    o3 = cl_arg_opt_create("o3", "o3", NULL);
    o2 = cl_arg_opt_create("o2", "o2", o3);
    o1 = cl_arg_opt_create("o1", "o1", o2);

    cl_arg_opt_destroy_chain(o1);

    o3_2 = cl_arg_opt_create("o3_2", "o3_2", NULL);
    o2_2 = cl_arg_opt_create("o2_2", "o2_2", NULL);
    o1_2 = cl_arg_opt_create("o1_2", "o1_2", NULL);

    cl_assert(message, o3 == o3_2);
    cl_assert(message, o2 == o2_2);
    cl_assert(message, o1 == o1_2);

    cl_arg_opt_destroy(o3_2);
    cl_arg_opt_destroy(o2_2);
    cl_arg_opt_destroy(o1_2);

    return 0;
}

char *cl_arg_opt_tests(void) {
    cl_run_test(cl_arg_opt_is_numeric_returns_is_numeric);
    cl_run_test(cl_arg_opt_get_numeric_min_returns_value);
    cl_run_test(cl_arg_opt_get_numeric_max_returns_value);
    cl_run_test(cl_arg_opt_create_creates_arg_opt);
    cl_run_test(cl_arg_opt_create_required_creates_arg_opt);
    cl_run_test(cl_arg_opt_create_numeric_creates_arg_opt);
    cl_run_test(cl_arg_opt_create_required_numeric_creates_arg_opt);
    cl_run_test(cl_arg_opt_create_creates_different_instances);
    cl_run_test(cl_arg_opt_destroy_releases_instance);
    cl_run_test(cl_arg_opt_destroy_chain_releases_all_instances);
    return 0;
}
