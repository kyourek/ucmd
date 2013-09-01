#include <stdlib.h>
#include <string.h>
#include "cl_arg_opt_owner.h"
#include "cl_switch_opt.h"
#include "cl_switch_opt_p.h"
#include "cl_switch_opt_tests.h"
#include "cl_tests.h"
#include "cl_tests_p.h"

static CL_TESTS_ERR cl_switch_opt_get_next_returns_next(void) {
    cl_switch_opt o1;
    cl_switch_opt o2;
    o1.next = &o2;
    CL_TESTS_ASSERT(cl_switch_opt_get_next(&o1) == &o2);
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_switch_opt_create_creates_switch_opt(void) {
    cl_arg_opt *a;
    cl_switch_opt *s1;
    cl_switch_opt *s2;
    
    a = cl_arg_opt_create("n", "d", NULL);
    s2 = cl_switch_opt_create("s2", "d", NULL, NULL);
    s1 = cl_switch_opt_create("-sw-name", "{-sw-name description}", a, s2);

    CL_TESTS_ASSERT(cl_opt_get_name((cl_opt*)s1));
    CL_TESTS_ASSERT(cl_opt_get_desc((cl_opt*)s1));
    CL_TESTS_ASSERT(a == cl_arg_opt_owner_get_arg_opt((cl_arg_opt_owner*)s1));
    CL_TESTS_ASSERT(CL_FALSE == cl_opt_is_required((cl_opt*)s1));
    CL_TESTS_ASSERT(s2 == cl_switch_opt_get_next(s1));

    cl_arg_opt_destroy(a);
    cl_switch_opt_destroy(s2);
    cl_switch_opt_destroy(s1);

    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_switch_opt_create_required_creates_switch_opt(void) {
    cl_arg_opt *a;
    cl_switch_opt *s1;
    cl_switch_opt *s2;
    
    a = cl_arg_opt_create("a", "description", NULL);
    s2 = cl_switch_opt_create("-sw", "desc", NULL, NULL);
    s1 = cl_switch_opt_create_required("-switch", "switch desc", a, s2);

    CL_TESTS_ASSERT(cl_opt_get_name((cl_opt*)s1));
    CL_TESTS_ASSERT(cl_opt_get_desc((cl_opt*)s1));
    CL_TESTS_ASSERT(a == cl_arg_opt_owner_get_arg_opt((cl_arg_opt_owner*)s1));
    CL_TESTS_ASSERT(CL_TRUE == cl_opt_is_required((cl_opt*)s1));
    CL_TESTS_ASSERT(s2 == cl_switch_opt_get_next(s1));

    cl_arg_opt_destroy(a);
    cl_switch_opt_destroy(s2);
    cl_switch_opt_destroy(s1);

    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_switch_opt_create_creates_different_instances() {
    cl_switch_opt *s1, *s2, *s3;

    s1 = cl_switch_opt_create("s1", "s1", NULL, NULL);
    s2 = cl_switch_opt_create("s2", "s2", NULL, NULL);
    s3 = cl_switch_opt_create("s3", "s3", NULL, NULL);

    CL_TESTS_ASSERT(s1 != s2);
    CL_TESTS_ASSERT(s2 != s3);

    cl_switch_opt_destroy(s1);
    cl_switch_opt_destroy(s2);
    cl_switch_opt_destroy(s3);

    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_switch_opt_destroy_releases_instance() {
    cl_switch_opt *s1, *s2, *s3;

    s1 = cl_switch_opt_create("s1", "s1", NULL, NULL);
    s2 = cl_switch_opt_create("s2", "s2", NULL, NULL);

    cl_switch_opt_destroy(s2);

    s3 = cl_switch_opt_create("s3", "s3", NULL, NULL);

    CL_TESTS_ASSERT(s3 == s2);

    cl_switch_opt_destroy(s1);
    cl_switch_opt_destroy(s3);

    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_switch_opt_destroy_chain_releases_all_instances() {
    cl_arg_opt *a1, *a2, *a3, *a4, *a4_2;
    cl_switch_opt *s1, *s2, *s3, *s3_2;

    a4 = cl_arg_opt_create("a4", "a4", NULL);
    a3 = cl_arg_opt_create("a3", "a3", a4);
    a2 = cl_arg_opt_create("a2", "a2", a3);

    a1 = cl_arg_opt_create("a1", "a1", NULL);

    s3 = cl_switch_opt_create("s3", "s3", a2, NULL);
    s2 = cl_switch_opt_create("s2", "s2", a1, s3);
    s1 = cl_switch_opt_create("s1", "s1", NULL, s2);

    cl_switch_opt_destroy_chain(s1);

    a4_2 = cl_arg_opt_create("a4_2", "a4_2", NULL);
    CL_TESTS_ASSERT(a4_2 == a4);

    s3_2 = cl_switch_opt_create("s3_2", "s3_2", NULL, NULL);
    CL_TESTS_ASSERT(s3_2 == s3);

    cl_arg_opt_destroy(a4_2);
    cl_switch_opt_destroy(s3_2);

    return CL_TESTS_NO_ERR;
}

CL_TESTS_ERR cl_switch_opt_tests(void) {
    CL_TESTS_RUN(cl_switch_opt_get_next_returns_next);
    CL_TESTS_RUN(cl_switch_opt_create_creates_switch_opt);
    CL_TESTS_RUN(cl_switch_opt_create_required_creates_switch_opt);
    CL_TESTS_RUN(cl_switch_opt_create_creates_different_instances);
    CL_TESTS_RUN(cl_switch_opt_destroy_releases_instance);
    CL_TESTS_RUN(cl_switch_opt_destroy_chain_releases_all_instances);
    return CL_TESTS_NO_ERR;
}
