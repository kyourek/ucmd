#include <stdlib.h>
#include <string.h>
#include "ucArgOpt_owner.h"
#include "uc_switch_opt_p.h"
#include "uc_switch_opt_tests.h"
#include "uc_test.h"

static uc_test_err uc_switch_opt_get_next_returns_next(uc_test_group *p) {
    uc_switch_opt o1;
    uc_switch_opt o2;
    o1.next = &o2;
    UC_TEST_ASSERT(uc_switch_opt_get_next(&o1) == &o2);
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_switch_opt_create_creates_switch_opt(uc_test_group *p) {
    ucArgOpt *a;
    uc_switch_opt *s1;
    uc_switch_opt *s2;
    
    a = ucArgOpt_create("n", "d", NULL);
    s2 = uc_switch_opt_create("s2", "d", NULL, NULL);
    s1 = uc_switch_opt_create("-sw-name", "{-sw-name description}", a, s2);

    UC_TEST_ASSERT(uc_opt_get_name((uc_opt*)s1));
    UC_TEST_ASSERT(uc_opt_get_desc((uc_opt*)s1));
    UC_TEST_ASSERT(a == ucArgOpt_owner_get_arg_opt((ucArgOpt_owner*)s1));
    UC_TEST_ASSERT(UC_FALSE == uc_opt_is_required((uc_opt*)s1));
    UC_TEST_ASSERT(s2 == uc_switch_opt_get_next(s1));

    ucArgOpt_destroy(a);
    uc_switch_opt_destroy(s2);
    uc_switch_opt_destroy(s1);

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_switch_opt_create_required_creates_switch_opt(uc_test_group *p) {
    ucArgOpt *a;
    uc_switch_opt *s1;
    uc_switch_opt *s2;
    
    a = ucArgOpt_create("a", "description", NULL);
    s2 = uc_switch_opt_create("-sw", "desc", NULL, NULL);
    s1 = uc_switch_opt_create_required("-switch", "switch desc", a, s2);

    UC_TEST_ASSERT(uc_opt_get_name((uc_opt*)s1));
    UC_TEST_ASSERT(uc_opt_get_desc((uc_opt*)s1));
    UC_TEST_ASSERT(a == ucArgOpt_owner_get_arg_opt((ucArgOpt_owner*)s1));
    UC_TEST_ASSERT(UC_TRUE == uc_opt_is_required((uc_opt*)s1));
    UC_TEST_ASSERT(s2 == uc_switch_opt_get_next(s1));

    ucArgOpt_destroy(a);
    uc_switch_opt_destroy(s2);
    uc_switch_opt_destroy(s1);

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_switch_opt_create_creates_different_instances(uc_test_group *p) {
    uc_switch_opt *s1, *s2, *s3;

    s1 = uc_switch_opt_create("s1", "s1", NULL, NULL);
    s2 = uc_switch_opt_create("s2", "s2", NULL, NULL);
    s3 = uc_switch_opt_create("s3", "s3", NULL, NULL);

    UC_TEST_ASSERT(s1 != s2);
    UC_TEST_ASSERT(s2 != s3);

    uc_switch_opt_destroy(s1);
    uc_switch_opt_destroy(s2);
    uc_switch_opt_destroy(s3);

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_switch_opt_destroy_releases_instance(uc_test_group *p) {
    uc_switch_opt *s1, *s2, *s3;

    s1 = uc_switch_opt_create("s1", "s1", NULL, NULL);
    s2 = uc_switch_opt_create("s2", "s2", NULL, NULL);

    uc_switch_opt_destroy(s2);

    s3 = uc_switch_opt_create("s3", "s3", NULL, NULL);

    UC_TEST_ASSERT(s3 == s2);

    uc_switch_opt_destroy(s1);
    uc_switch_opt_destroy(s3);

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_switch_opt_destroy_chain_releases_all_instances(uc_test_group *p) {
    ucArgOpt *a1, *a2, *a3, *a4, *a4_2;
    uc_switch_opt *s1, *s2, *s3, *s3_2;

    a4 = ucArgOpt_create("a4", "a4", NULL);
    a3 = ucArgOpt_create("a3", "a3", a4);
    a2 = ucArgOpt_create("a2", "a2", a3);

    a1 = ucArgOpt_create("a1", "a1", NULL);

    s3 = uc_switch_opt_create("s3", "s3", a2, NULL);
    s2 = uc_switch_opt_create("s2", "s2", a1, s3);
    s1 = uc_switch_opt_create("s1", "s1", NULL, s2);

    uc_switch_opt_destroy_chain(s1);

    a4_2 = ucArgOpt_create("a4_2", "a4_2", NULL);
    UC_TEST_ASSERT(a4_2 == a4);

    s3_2 = uc_switch_opt_create("s3_2", "s3_2", NULL, NULL);
    UC_TEST_ASSERT(s3_2 == s3);

    ucArgOpt_destroy(a4_2);
    uc_switch_opt_destroy(s3_2);

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_switch_opt_format_validation_err_catches_required_switch(uc_test_group *p) {
    const char *err;
    uc_cmd_line *cmd = uc_cmd_line_get_instance();
    uc_switch_opt *s = uc_switch_opt_create_required("-s\0\n", NULL, NULL, NULL);

    err = uc_switch_opt_format_validation_err(s, cmd, NULL);
    UC_TEST_ASSERT(NULL != err);

    err = uc_switch_opt_format_validation_err(s, cmd, "-s\0\n");
    UC_TEST_ASSERT(NULL == err);

    uc_switch_opt_destroy(s);
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_switch_opt_format_validation_err_catches_required_arg(uc_test_group *p) {
    const char *err;
    uc_cmd_line *cmd = uc_cmd_line_get_instance();
    ucArgOpt *a = ucArgOpt_create_required("a", NULL, NULL);
    uc_switch_opt *s = uc_switch_opt_create("-s", NULL, a, NULL);

    err = uc_switch_opt_format_validation_err(s, cmd, "-s\0\n");
    UC_TEST_ASSERT(NULL != err);

    err = uc_switch_opt_format_validation_err(s, cmd, "-s\0a\0\n");
    UC_TEST_ASSERT(NULL == err);

    ucArgOpt_destroy(a);
    uc_switch_opt_destroy(s);
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_switch_opt_format_validation_err_allows_multiple_arguments(uc_test_group *p) {
    const char *err;
    uc_cmd_line *cmd = uc_cmd_line_get_instance();
    ucArgOpt *a = ucArgOpt_create_multiple("a", NULL, 0, 3);
    uc_switch_opt *s = uc_switch_opt_create("-s", NULL, a, NULL);

    err = uc_switch_opt_format_validation_err(s, cmd, "-s\0\n");
    UC_TEST_ASSERT(NULL == err);

    err = uc_switch_opt_format_validation_err(s, cmd, "-s\0a1\0a2\0a3\0\n");
    UC_TEST_ASSERT(NULL == err);

    ucArgOpt_destroy(a);
    uc_switch_opt_destroy(s);

    return UC_TEST_ERR_NONE;
}

uc_test_group *uc_switch_opt_tests_get_group(void) {
    static uc_test_group group;
    static uc_test_group_test_func *tests[] = {
        uc_switch_opt_get_next_returns_next,
        uc_switch_opt_create_creates_switch_opt,
        uc_switch_opt_create_required_creates_switch_opt,
        uc_switch_opt_create_creates_different_instances,
        uc_switch_opt_destroy_releases_instance,
        uc_switch_opt_destroy_chain_releases_all_instances,
        uc_switch_opt_format_validation_err_catches_required_switch,
        uc_switch_opt_format_validation_err_catches_required_arg,
        uc_switch_opt_format_validation_err_allows_multiple_arguments,
        NULL
    };

    return uc_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
