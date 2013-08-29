#include <stdlib.h>
#include <string.h>
#include "cl_arg_opt_owner.h"
#include "cl_tests.h"
#include "cl_switch_opt.h"
#include "cl_switch_opt_p.h"
#include "cl_switch_opt_tests.h"

static char *cl_switch_opt_get_next_returns_next(void) {
    cl_switch_opt o1;
    cl_switch_opt o2;
    o1.next = &o2;
    cl_assert("cl_switch_opt_get_next did not return next pointer.", cl_switch_opt_get_next(&o1) == &o2);
    return 0;
}

static char *cl_switch_opt_create_creates_switch_opt(void) {
    cl_arg_opt *a;
    cl_switch_opt *s1;
    cl_switch_opt *s2;
    
    a = cl_arg_opt_create("n", "d", NULL);
    s2 = cl_switch_opt_create("s2", "d", NULL, NULL);
    s1 = cl_switch_opt_create("-sw-name", "{-sw-name description}", a, s2);

    cl_assert("cl_switch_opt_create did not set name.", 0 == strcmp("-sw-name", cl_opt_get_name((cl_opt*)s1)));
    cl_assert("cl_switch_opt_create did not set desc.", 0 == strcmp("{-sw-name description}", cl_opt_get_desc((cl_opt*)s1)));
    cl_assert("cl_switch_opt_create did not set arg_opt.", a == cl_arg_opt_owner_get_arg_opt((cl_arg_opt_owner*)s1));
    cl_assert("cl_switch_opt_create created required switch.", CL_FALSE == cl_opt_is_required((cl_opt*)s1));
    cl_assert("cl_switch_opt_create did not set next.", s2 == cl_switch_opt_get_next(s1));

    cl_arg_opt_destroy(a);
    cl_switch_opt_destroy(s2);
    cl_switch_opt_destroy(s1);

    return 0;
}

static char *cl_switch_opt_create_required_creates_switch_opt(void) {
    cl_arg_opt *a;
    cl_switch_opt *s1;
    cl_switch_opt *s2;
    
    a = cl_arg_opt_create("a", "description", NULL);
    s2 = cl_switch_opt_create("-sw", "desc", NULL, NULL);
    s1 = cl_switch_opt_create_required("-switch", "switch desc", a, s2);

    cl_assert("cl_switch_opt_create_required did not set name.", 0 == strcmp("-switch", cl_opt_get_name((cl_opt*)s1)));
    cl_assert("cl_switch_opt_create_required did not set desc.", 0 == strcmp("switch desc", cl_opt_get_desc((cl_opt*)s1)));
    cl_assert("cl_switch_opt_create_required did not set arg_opt.", a == cl_arg_opt_owner_get_arg_opt((cl_arg_opt_owner*)s1));
    cl_assert("cl_switch_opt_create_required created non-required switch.", CL_TRUE == cl_opt_is_required((cl_opt*)s1));
    cl_assert("cl_switch_opt_create_required did not set next.", s2 == cl_switch_opt_get_next(s1));

    cl_arg_opt_destroy(a);
    cl_switch_opt_destroy(s2);
    cl_switch_opt_destroy(s1);

    return 0;
}

static char *cl_switch_opt_create_creates_different_instances() {
    cl_switch_opt *s1, *s2, *s3;
    char *message = "cl_switch_opt_create did not create different instances.";

    s1 = cl_switch_opt_create("s1", "s1", NULL, NULL);
    s2 = cl_switch_opt_create("s2", "s2", NULL, NULL);
    s3 = cl_switch_opt_create("s3", "s3", NULL, NULL);

    cl_assert(message, s1 != s2);
    cl_assert(message, s2 != s3);

    cl_switch_opt_destroy(s1);
    cl_switch_opt_destroy(s2);
    cl_switch_opt_destroy(s3);

    return 0;
}

static char *cl_switch_opt_destroy_releases_instance() {
    cl_switch_opt *s1, *s2, *s3;
    char *message = "cl_switch_opt_destroy did not release instance.";

    s1 = cl_switch_opt_create("s1", "s1", NULL, NULL);
    s2 = cl_switch_opt_create("s2", "s2", NULL, NULL);

    cl_switch_opt_destroy(s2);

    s3 = cl_switch_opt_create("s3", "s3", NULL, NULL);

    cl_assert(message, s3 == s2);

    cl_switch_opt_destroy(s1);
    cl_switch_opt_destroy(s3);

    return 0;
}

static char *cl_switch_opt_destroy_chain_releases_all_instances() {
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
    cl_assert("cl_switch_opt_destroy_chain did not release argument options.", a4_2 == a4);

    s3_2 = cl_switch_opt_create("s3_2", "s3_2", NULL, NULL);
    cl_assert("cl_switch_opt_destroy_chain did not release switch options.", s3_2 == s3);

    cl_arg_opt_destroy(a4_2);
    cl_switch_opt_destroy(s3_2);

    return 0;
}

char *cl_switch_opt_tests(void) {
    cl_run_test(cl_switch_opt_get_next_returns_next);
    cl_run_test(cl_switch_opt_create_creates_switch_opt);
    cl_run_test(cl_switch_opt_create_required_creates_switch_opt);
    cl_run_test(cl_switch_opt_create_creates_different_instances);
    cl_run_test(cl_switch_opt_destroy_releases_instance);
    cl_run_test(cl_switch_opt_destroy_chain_releases_all_instances);
    return 0;
}
