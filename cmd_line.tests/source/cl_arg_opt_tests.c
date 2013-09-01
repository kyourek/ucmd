#include <string.h>
#include "cl_arg_opt.h"
#include "cl_arg_opt_p.h"
#include "cl_tests.h"
#include "cl_tests_p.h"

static CL_TESTS_ERR cl_arg_opt_is_numeric_returns_is_numeric(void) {
    cl_arg_opt o;
    
    o.is_numeric = CL_FALSE;
    CL_TESTS_ASSERT(CL_FALSE == cl_arg_opt_is_numeric(&o));

    o.is_numeric = CL_TRUE;
    CL_TESTS_ASSERT(CL_TRUE == cl_arg_opt_is_numeric(&o));

    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_arg_opt_get_numeric_min_returns_value(void) {
    cl_arg_opt o;
    o.numeric_min = -765.432;
    CL_TESTS_ASSERT(-765.432 == cl_arg_opt_get_numeric_min(&o));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_arg_opt_get_numeric_max_returns_value(void) {
    cl_arg_opt o;
    o.numeric_max = 0.123456789;
    CL_TESTS_ASSERT(0.123456789 == cl_arg_opt_get_numeric_max(&o));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_arg_opt_create_creates_arg_opt(void) {
    cl_arg_opt *p;
    cl_arg_opt a;
    
    p = cl_arg_opt_create("a-name", "a-desc", &a);
    
    CL_TESTS_ASSERT(cl_opt_get_name((cl_opt*)p));
    CL_TESTS_ASSERT(cl_opt_get_desc((cl_opt*)p));
    CL_TESTS_ASSERT(&a == cl_arg_opt_get_next(p));
    CL_TESTS_ASSERT(CL_FALSE == cl_opt_is_required((cl_opt*)p));
    CL_TESTS_ASSERT(CL_FALSE == cl_arg_opt_is_numeric(p));

    cl_arg_opt_destroy(p);

    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_arg_opt_create_required_creates_arg_opt(void) {
    cl_arg_opt *p;
    cl_arg_opt a;

    p = cl_arg_opt_create_required("aname", "adesc", &a);

    CL_TESTS_ASSERT(cl_opt_get_name((cl_opt*)p));
    CL_TESTS_ASSERT(cl_opt_get_desc((cl_opt*)p));
    CL_TESTS_ASSERT(&a == cl_arg_opt_get_next(p));
    CL_TESTS_ASSERT(CL_TRUE == cl_opt_is_required((cl_opt*)p));
    CL_TESTS_ASSERT(CL_FALSE == cl_arg_opt_is_numeric(p));

    cl_arg_opt_destroy(p);

    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_arg_opt_create_numeric_creates_arg_opt(void) {
    cl_arg_opt *p;
    cl_arg_opt a;
    
    p = cl_arg_opt_create_numeric("ds", -5.678, 12.34, &a);
    
    CL_TESTS_ASSERT(cl_opt_get_name((cl_opt*)p));
    CL_TESTS_ASSERT(cl_opt_get_desc((cl_opt*)p));
    CL_TESTS_ASSERT(&a == cl_arg_opt_get_next(p));
    CL_TESTS_ASSERT(CL_FALSE == cl_opt_is_required((cl_opt*)p));
    CL_TESTS_ASSERT(CL_TRUE == cl_arg_opt_is_numeric(p));
    CL_TESTS_ASSERT(-5.678 == cl_arg_opt_get_numeric_min(p));
    CL_TESTS_ASSERT(12.34 == cl_arg_opt_get_numeric_max(p));

    cl_arg_opt_destroy(p);

    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_arg_opt_create_required_numeric_creates_arg_opt(void) {
    cl_arg_opt *p;
    cl_arg_opt a;
    
    p = cl_arg_opt_create_required_numeric("REQ", 100.436, 567.890, &a);
    
    CL_TESTS_ASSERT(cl_opt_get_name((cl_opt*)p));
    CL_TESTS_ASSERT(cl_opt_get_desc((cl_opt*)p));
    CL_TESTS_ASSERT(&a == cl_arg_opt_get_next(p));
    CL_TESTS_ASSERT(CL_TRUE == cl_opt_is_required((cl_opt*)p));
    CL_TESTS_ASSERT(CL_TRUE == cl_arg_opt_is_numeric(p));
    CL_TESTS_ASSERT(100.436 == cl_arg_opt_get_numeric_min(p));
    CL_TESTS_ASSERT(567.890 == cl_arg_opt_get_numeric_max(p));

    cl_arg_opt_destroy(p);

    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_arg_opt_create_creates_different_instances(void) {
    cl_arg_opt *o1, *o2, *o3;

    o1 = cl_arg_opt_create("o1", "o1", NULL);
    o2 = cl_arg_opt_create("o2", "o2", NULL);
    o3 = cl_arg_opt_create("o3", "o3", NULL);

    CL_TESTS_ASSERT(o1 != o2);
    CL_TESTS_ASSERT(o2 != o3);

    cl_arg_opt_destroy(o1);
    cl_arg_opt_destroy(o2);
    cl_arg_opt_destroy(o3);

    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_arg_opt_destroy_releases_instance(void) {
    cl_arg_opt *o1, *o2, *o3;

    o1 = cl_arg_opt_create("o1", "o1", NULL);
    o2 = cl_arg_opt_create("o2", "o2", NULL);

    cl_arg_opt_destroy(o2);

    o3 = cl_arg_opt_create("o3", "o3", NULL);

    CL_TESTS_ASSERT(o3 == o2);

    cl_arg_opt_destroy(o1);
    cl_arg_opt_destroy(o3);

    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_arg_opt_destroy_chain_releases_all_instances(void) {
    cl_arg_opt *o1, *o2, *o3, *o1_2, *o2_2, *o3_2;

    o3 = cl_arg_opt_create("o3", "o3", NULL);
    o2 = cl_arg_opt_create("o2", "o2", o3);
    o1 = cl_arg_opt_create("o1", "o1", o2);

    cl_arg_opt_destroy_chain(o1);

    o3_2 = cl_arg_opt_create("o3_2", "o3_2", NULL);
    o2_2 = cl_arg_opt_create("o2_2", "o2_2", NULL);
    o1_2 = cl_arg_opt_create("o1_2", "o1_2", NULL);

    CL_TESTS_ASSERT(o3 == o3_2);
    CL_TESTS_ASSERT(o2 == o2_2);
    CL_TESTS_ASSERT(o1 == o1_2);

    cl_arg_opt_destroy(o3_2);
    cl_arg_opt_destroy(o2_2);
    cl_arg_opt_destroy(o1_2);

    return CL_TESTS_NO_ERR;
}

CL_TESTS_ERR cl_arg_opt_tests(void) {
    CL_TESTS_RUN(cl_arg_opt_is_numeric_returns_is_numeric);
    CL_TESTS_RUN(cl_arg_opt_get_numeric_min_returns_value);
    CL_TESTS_RUN(cl_arg_opt_get_numeric_max_returns_value);
    CL_TESTS_RUN(cl_arg_opt_create_creates_arg_opt);
    CL_TESTS_RUN(cl_arg_opt_create_required_creates_arg_opt);
    CL_TESTS_RUN(cl_arg_opt_create_numeric_creates_arg_opt);
    CL_TESTS_RUN(cl_arg_opt_create_required_numeric_creates_arg_opt);
    CL_TESTS_RUN(cl_arg_opt_create_creates_different_instances);
    CL_TESTS_RUN(cl_arg_opt_destroy_releases_instance);
    CL_TESTS_RUN(cl_arg_opt_destroy_chain_releases_all_instances);
    return CL_TESTS_NO_ERR;
}