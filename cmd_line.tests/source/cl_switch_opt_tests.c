#include <stdlib.h>
#include <string.h>
#include "cl_arg_opt_owner.h"
#include "cl_switch_opt_p.h"
#include "cl_switch_opt_tests.h"
#include "cl_test.h"

static cl_test_err cl_switch_opt_get_next_returns_next(cl_test_group *p) {
    cl_switch_opt o1;
    cl_switch_opt o2;
    o1.next = &o2;
    CL_TEST_ASSERT(cl_switch_opt_get_next(&o1) == &o2);
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_switch_opt_create_creates_switch_opt(cl_test_group *p) {
    cl_arg_opt *a;
    cl_switch_opt *s1;
    cl_switch_opt *s2;
    
    a = cl_arg_opt_create("n", "d", NULL);
    s2 = cl_switch_opt_create("s2", "d", NULL, NULL);
    s1 = cl_switch_opt_create("-sw-name", "{-sw-name description}", a, s2);

    CL_TEST_ASSERT(cl_opt_get_name((cl_opt*)s1));
    CL_TEST_ASSERT(cl_opt_get_desc((cl_opt*)s1));
    CL_TEST_ASSERT(a == cl_arg_opt_owner_get_arg_opt((cl_arg_opt_owner*)s1));
    CL_TEST_ASSERT(CL_FALSE == cl_opt_is_required((cl_opt*)s1));
    CL_TEST_ASSERT(s2 == cl_switch_opt_get_next(s1));

    cl_arg_opt_destroy(a);
    cl_switch_opt_destroy(s2);
    cl_switch_opt_destroy(s1);

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_switch_opt_create_required_creates_switch_opt(cl_test_group *p) {
    cl_arg_opt *a;
    cl_switch_opt *s1;
    cl_switch_opt *s2;
    
    a = cl_arg_opt_create("a", "description", NULL);
    s2 = cl_switch_opt_create("-sw", "desc", NULL, NULL);
    s1 = cl_switch_opt_create_required("-switch", "switch desc", a, s2);

    CL_TEST_ASSERT(cl_opt_get_name((cl_opt*)s1));
    CL_TEST_ASSERT(cl_opt_get_desc((cl_opt*)s1));
    CL_TEST_ASSERT(a == cl_arg_opt_owner_get_arg_opt((cl_arg_opt_owner*)s1));
    CL_TEST_ASSERT(CL_TRUE == cl_opt_is_required((cl_opt*)s1));
    CL_TEST_ASSERT(s2 == cl_switch_opt_get_next(s1));

    cl_arg_opt_destroy(a);
    cl_switch_opt_destroy(s2);
    cl_switch_opt_destroy(s1);

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_switch_opt_create_creates_different_instances(cl_test_group *p) {
    cl_switch_opt *s1, *s2, *s3;

    s1 = cl_switch_opt_create("s1", "s1", NULL, NULL);
    s2 = cl_switch_opt_create("s2", "s2", NULL, NULL);
    s3 = cl_switch_opt_create("s3", "s3", NULL, NULL);

    CL_TEST_ASSERT(s1 != s2);
    CL_TEST_ASSERT(s2 != s3);

    cl_switch_opt_destroy(s1);
    cl_switch_opt_destroy(s2);
    cl_switch_opt_destroy(s3);

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_switch_opt_destroy_releases_instance(cl_test_group *p) {
    cl_switch_opt *s1, *s2, *s3;

    s1 = cl_switch_opt_create("s1", "s1", NULL, NULL);
    s2 = cl_switch_opt_create("s2", "s2", NULL, NULL);

    cl_switch_opt_destroy(s2);

    s3 = cl_switch_opt_create("s3", "s3", NULL, NULL);

    CL_TEST_ASSERT(s3 == s2);

    cl_switch_opt_destroy(s1);
    cl_switch_opt_destroy(s3);

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_switch_opt_destroy_chain_releases_all_instances(cl_test_group *p) {
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
    CL_TEST_ASSERT(a4_2 == a4);

    s3_2 = cl_switch_opt_create("s3_2", "s3_2", NULL, NULL);
    CL_TEST_ASSERT(s3_2 == s3);

    cl_arg_opt_destroy(a4_2);
    cl_switch_opt_destroy(s3_2);

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_switch_opt_format_validation_err_catches_required_switch(cl_test_group *p) {
    const char *err;
    cl_cmd_line *cmd = cl_cmd_line_get_instance();
    cl_switch_opt *s = cl_switch_opt_create_required("-s\0\n", NULL, NULL, NULL);

    err = cl_switch_opt_format_validation_err(s, cmd, NULL);
    CL_TEST_ASSERT(NULL != err);

    err = cl_switch_opt_format_validation_err(s, cmd, "-s\0\n");
    CL_TEST_ASSERT(NULL == err);

    cl_switch_opt_destroy(s);
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_switch_opt_format_validation_err_catches_required_arg(cl_test_group *p) {
    const char *err;
    cl_cmd_line *cmd = cl_cmd_line_get_instance();
    cl_arg_opt *a = cl_arg_opt_create_required("a", NULL, NULL);
    cl_switch_opt *s = cl_switch_opt_create("-s", NULL, a, NULL);

    err = cl_switch_opt_format_validation_err(s, cmd, "-s\0\n");
    CL_TEST_ASSERT(NULL != err);

    err = cl_switch_opt_format_validation_err(s, cmd, "-s\0a\0\n");
    CL_TEST_ASSERT(NULL == err);

    cl_arg_opt_destroy(a);
    cl_switch_opt_destroy(s);
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_switch_opt_format_validation_err_allows_multiple_arguments(cl_test_group *p) {
    const char *err;
    cl_cmd_line *cmd = cl_cmd_line_get_instance();
    cl_arg_opt *a = cl_arg_opt_create_multiple("a", NULL, 0, 3);
    cl_switch_opt *s = cl_switch_opt_create("-s", NULL, a, NULL);

    err = cl_switch_opt_format_validation_err(s, cmd, "-s\0\n");
    CL_TEST_ASSERT(NULL == err);

    err = cl_switch_opt_format_validation_err(s, cmd, "-s\0a1\0a2\0a3\0\n");
    CL_TEST_ASSERT(NULL == err);

    cl_arg_opt_destroy(a);
    cl_switch_opt_destroy(s);

    return CL_TEST_ERR_NONE;
}

cl_test_group *cl_switch_opt_tests_get_group(void) {
    static cl_test_group group;
    static cl_test_group_test_func *tests[] = {
        cl_switch_opt_get_next_returns_next,
        cl_switch_opt_create_creates_switch_opt,
        cl_switch_opt_create_required_creates_switch_opt,
        cl_switch_opt_create_creates_different_instances,
        cl_switch_opt_destroy_releases_instance,
        cl_switch_opt_destroy_chain_releases_all_instances,
        cl_switch_opt_format_validation_err_catches_required_switch,
        cl_switch_opt_format_validation_err_catches_required_arg,
        cl_switch_opt_format_validation_err_allows_multiple_arguments,
        NULL
    };

    return cl_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
