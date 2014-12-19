#include <stdlib.h>
#include <string.h>
#include "ucArgOptOwner.h"
#include "ucSwitchOpt_p.h"
#include "ucTest.h"

static ucTestErr ucSwitchOpt_get_next_returns_next(ucTestGroup *p) {
    ucSwitchOpt o1;
    ucSwitchOpt o2;
    o1.next = &o2;
    ucTest_ASSERT(ucSwitchOpt_get_next(&o1) == &o2);
    return ucTestErr_NONE;
}

static ucTestErr ucSwitchOpt_create_creates_switch_opt(ucTestGroup *p) {
    ucArgOpt *a;
    ucSwitchOpt *s1;
    ucSwitchOpt *s2;
    
    a = ucArgOpt_create("n", "d", NULL);
    s2 = ucSwitchOpt_create("s2", "d", NULL, NULL);
    s1 = ucSwitchOpt_create("-sw-name", "{-sw-name description}", a, s2);

    ucTest_ASSERT(ucOpt_get_name((ucOpt*)s1));
    ucTest_ASSERT(ucOpt_get_desc((ucOpt*)s1));
    ucTest_ASSERT(a == ucArgOptOwner_get_arg_opt((ucArgOptOwner*)s1));
    ucTest_ASSERT(ucBool_false == ucOpt_is_required((ucOpt*)s1));
    ucTest_ASSERT(s2 == ucSwitchOpt_get_next(s1));

    ucArgOpt_destroy(a);
    ucSwitchOpt_destroy(s2);
    ucSwitchOpt_destroy(s1);

    return ucTestErr_NONE;
}

static ucTestErr ucSwitchOpt_create_required_creates_switch_opt(ucTestGroup *p) {
    ucArgOpt *a;
    ucSwitchOpt *s1;
    ucSwitchOpt *s2;
    
    a = ucArgOpt_create("a", "description", NULL);
    s2 = ucSwitchOpt_create("-sw", "desc", NULL, NULL);
    s1 = ucSwitchOpt_create_required("-switch", "switch desc", a, s2);

    ucTest_ASSERT(ucOpt_get_name((ucOpt*)s1));
    ucTest_ASSERT(ucOpt_get_desc((ucOpt*)s1));
    ucTest_ASSERT(a == ucArgOptOwner_get_arg_opt((ucArgOptOwner*)s1));
    ucTest_ASSERT(ucBool_true == ucOpt_is_required((ucOpt*)s1));
    ucTest_ASSERT(s2 == ucSwitchOpt_get_next(s1));

    ucArgOpt_destroy(a);
    ucSwitchOpt_destroy(s2);
    ucSwitchOpt_destroy(s1);

    return ucTestErr_NONE;
}

static ucTestErr ucSwitchOpt_create_creates_different_instances(ucTestGroup *p) {
    ucSwitchOpt *s1, *s2, *s3;

    s1 = ucSwitchOpt_create("s1", "s1", NULL, NULL);
    s2 = ucSwitchOpt_create("s2", "s2", NULL, NULL);
    s3 = ucSwitchOpt_create("s3", "s3", NULL, NULL);

    ucTest_ASSERT(s1 != s2);
    ucTest_ASSERT(s2 != s3);

    ucSwitchOpt_destroy(s1);
    ucSwitchOpt_destroy(s2);
    ucSwitchOpt_destroy(s3);

    return ucTestErr_NONE;
}

static ucTestErr ucSwitchOpt_destroy_releases_instance(ucTestGroup *p) {
    ucSwitchOpt *s1, *s2, *s3;

    s1 = ucSwitchOpt_create("s1", "s1", NULL, NULL);
    s2 = ucSwitchOpt_create("s2", "s2", NULL, NULL);

    ucSwitchOpt_destroy(s2);

    s3 = ucSwitchOpt_create("s3", "s3", NULL, NULL);

    ucTest_ASSERT(s3 == s2);

    ucSwitchOpt_destroy(s1);
    ucSwitchOpt_destroy(s3);

    return ucTestErr_NONE;
}

static ucTestErr ucSwitchOpt_destroy_chain_releases_all_instances(ucTestGroup *p) {
    ucArgOpt *a1, *a2, *a3, *a4, *a4_2;
    ucSwitchOpt *s1, *s2, *s3, *s3_2;

    a4 = ucArgOpt_create("a4", "a4", NULL);
    a3 = ucArgOpt_create("a3", "a3", a4);
    a2 = ucArgOpt_create("a2", "a2", a3);

    a1 = ucArgOpt_create("a1", "a1", NULL);

    s3 = ucSwitchOpt_create("s3", "s3", a2, NULL);
    s2 = ucSwitchOpt_create("s2", "s2", a1, s3);
    s1 = ucSwitchOpt_create("s1", "s1", NULL, s2);

    ucSwitchOpt_destroy_chain(s1);

    a4_2 = ucArgOpt_create("a4_2", "a4_2", NULL);
    ucTest_ASSERT(a4_2 == a4);

    s3_2 = ucSwitchOpt_create("s3_2", "s3_2", NULL, NULL);
    ucTest_ASSERT(s3_2 == s3);

    ucArgOpt_destroy(a4_2);
    ucSwitchOpt_destroy(s3_2);

    return ucTestErr_NONE;
}

static ucTestErr ucSwitchOpt_format_validation_err_catches_required_switch(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_get_instance();
    ucSwitchOpt *s = ucSwitchOpt_create_required("-s\0\n", NULL, NULL, NULL);

    err = ucSwitchOpt_format_validation_err(s, cmd, NULL);
    ucTest_ASSERT(NULL != err);

    err = ucSwitchOpt_format_validation_err(s, cmd, "-s\0\n");
    ucTest_ASSERT(NULL == err);

    ucSwitchOpt_destroy(s);
    return ucTestErr_NONE;
}

static ucTestErr ucSwitchOpt_format_validation_err_catches_required_arg(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_get_instance();
    ucArgOpt *a = ucArgOpt_create_required("a", NULL, NULL);
    ucSwitchOpt *s = ucSwitchOpt_create("-s", NULL, a, NULL);

    err = ucSwitchOpt_format_validation_err(s, cmd, "-s\0\n");
    ucTest_ASSERT(NULL != err);

    err = ucSwitchOpt_format_validation_err(s, cmd, "-s\0a\0\n");
    ucTest_ASSERT(NULL == err);

    ucArgOpt_destroy(a);
    ucSwitchOpt_destroy(s);
    return ucTestErr_NONE;
}

static ucTestErr ucSwitchOpt_format_validation_err_allows_multiple_arguments(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_get_instance();
    ucArgOpt *a = ucArgOpt_create_multiple("a", NULL, 0, 3);
    ucSwitchOpt *s = ucSwitchOpt_create("-s", NULL, a, NULL);

    err = ucSwitchOpt_format_validation_err(s, cmd, "-s\0\n");
    ucTest_ASSERT(NULL == err);

    err = ucSwitchOpt_format_validation_err(s, cmd, "-s\0a1\0a2\0a3\0\n");
    ucTest_ASSERT(NULL == err);

    ucArgOpt_destroy(a);
    ucSwitchOpt_destroy(s);

    return ucTestErr_NONE;
}

ucTestGroup *ucSwitchOpt_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_test_func *tests[] = {
        ucSwitchOpt_get_next_returns_next,
        ucSwitchOpt_create_creates_switch_opt,
        ucSwitchOpt_create_required_creates_switch_opt,
        ucSwitchOpt_create_creates_different_instances,
        ucSwitchOpt_destroy_releases_instance,
        ucSwitchOpt_destroy_chain_releases_all_instances,
        ucSwitchOpt_format_validation_err_catches_required_switch,
        ucSwitchOpt_format_validation_err_catches_required_arg,
        ucSwitchOpt_format_validation_err_allows_multiple_arguments,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
