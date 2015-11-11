#include <stdlib.h>
#include "ucmdtests.h"

static ucCmd *cmd;

uc_TEST(prior)
    cmd = ucCmd_create();
uc_PASS

uc_TEST(after)
    ucCmd_destroy(cmd);
uc_PASS

uc_TEST(setup)
    ucTestGroup_setup_test(p, prior, after);
uc_PASS

uc_TEST(ucSwitchOpt_get_next_returns_next)
    ucSwitchOpt o1;
    ucSwitchOpt o2;
    o1.next = &o2;
    uc_TRUE(ucSwitchOpt_get_next(&o1) == &o2);
uc_PASS

uc_TEST(ucSwitchOpt_create_creates_switch_opt)
    ucArgOpt *a;
    ucSwitchOpt *s1;
    ucSwitchOpt *s2;
    
    a = ucArgOpt_create("n", "d", NULL);
    s2 = ucSwitchOpt_create("s2", "d", NULL, NULL);
    s1 = ucSwitchOpt_create("-sw-name", "{-sw-name description}", a, s2);

    uc_TRUE(ucOpt_get_name((ucOpt*)s1));
    uc_TRUE(ucOpt_get_desc((ucOpt*)s1));
    uc_TRUE(a == ucArgOptOwner_get_arg_opt((ucArgOptOwner*)s1));
    uc_TRUE(ucBool_false == ucOpt_is_required((ucOpt*)s1));
    uc_TRUE(s2 == ucSwitchOpt_get_next(s1));

    ucArgOpt_destroy(a);
    ucSwitchOpt_destroy(s2);
    ucSwitchOpt_destroy(s1);
uc_PASS

uc_TEST(ucSwitchOpt_create_required_creates_switch_opt)
    ucArgOpt *a;
    ucSwitchOpt *s1;
    ucSwitchOpt *s2;
    
    a = ucArgOpt_create("a", "description", NULL);
    s2 = ucSwitchOpt_create("-sw", "desc", NULL, NULL);
    s1 = ucSwitchOpt_create_required("-switch", "switch desc", a, s2);

    uc_TRUE(ucOpt_get_name((ucOpt*)s1));
    uc_TRUE(ucOpt_get_desc((ucOpt*)s1));
    uc_TRUE(a == ucArgOptOwner_get_arg_opt((ucArgOptOwner*)s1));
    uc_TRUE(ucBool_true == ucOpt_is_required((ucOpt*)s1));
    uc_TRUE(s2 == ucSwitchOpt_get_next(s1));

    ucArgOpt_destroy(a);
    ucSwitchOpt_destroy(s2);
    ucSwitchOpt_destroy(s1);
uc_PASS

uc_TEST(ucSwitchOpt_create_creates_different_instances)
    ucSwitchOpt *s1, *s2, *s3;

    s1 = ucSwitchOpt_create("s1", "s1", NULL, NULL);
    s2 = ucSwitchOpt_create("s2", "s2", NULL, NULL);
    s3 = ucSwitchOpt_create("s3", "s3", NULL, NULL);

    uc_TRUE(s1 != s2);
    uc_TRUE(s2 != s3);

    ucSwitchOpt_destroy(s1);
    ucSwitchOpt_destroy(s2);
    ucSwitchOpt_destroy(s3);
uc_PASS

uc_TEST(ucSwitchOpt_destroy_releases_instance)
    ucSwitchOpt *s1, *s2, *s3;

    s1 = ucSwitchOpt_create("s1", "s1", NULL, NULL);
    s2 = ucSwitchOpt_create("s2", "s2", NULL, NULL);

    ucSwitchOpt_destroy(s2);

    s3 = ucSwitchOpt_create("s3", "s3", NULL, NULL);

    uc_TRUE(s3 == s2);

    ucSwitchOpt_destroy(s1);
    ucSwitchOpt_destroy(s3);
uc_PASS

uc_TEST(ucSwitchOpt_destroy_chain_releases_all_instances)
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
    uc_TRUE(a4_2 == a4);

    s3_2 = ucSwitchOpt_create("s3_2", "s3_2", NULL, NULL);
    uc_TRUE(s3_2 == s3);

    ucArgOpt_destroy(a4_2);
    ucSwitchOpt_destroy(s3_2);
uc_PASS

uc_TEST(ucSwitchOpt_format_validation_err_catches_required_switch)
    const char *err;
    ucSwitchOpt *s = ucSwitchOpt_create_required("-s\0\n", NULL, NULL, NULL);

    err = ucSwitchOpt_format_validation_err(s, cmd, NULL);
    uc_TRUE(NULL != err);

    err = ucSwitchOpt_format_validation_err(s, cmd, "-s\0\n");
    uc_TRUE(NULL == err);

    ucSwitchOpt_destroy(s);
uc_PASS

uc_TEST(ucSwitchOpt_format_validation_err_catches_required_arg)
    const char *err;
    ucArgOpt *a = ucArgOpt_create_required("a", NULL, NULL);
    ucSwitchOpt *s = ucSwitchOpt_create("-s", NULL, a, NULL);

    err = ucSwitchOpt_format_validation_err(s, cmd, "-s\0\n");
    uc_TRUE(NULL != err);

    err = ucSwitchOpt_format_validation_err(s, cmd, "-s\0a\0\n");
    uc_TRUE(NULL == err);

    ucArgOpt_destroy(a);
    ucSwitchOpt_destroy(s);
uc_PASS

uc_TEST(ucSwitchOpt_format_validation_err_allows_multiple_arguments)
    const char *err;
    ucArgOpt *a = ucArgOpt_create_multiple("a", NULL, 0, 3);
    ucSwitchOpt *s = ucSwitchOpt_create("-s", NULL, a, NULL);

    err = ucSwitchOpt_format_validation_err(s, cmd, "-s\0\n");
    uc_TRUE(NULL == err);

    err = ucSwitchOpt_format_validation_err(s, cmd, "-s\0a1\0a2\0a3\0\n");
    uc_TRUE(NULL == err);

    ucArgOpt_destroy(a);
    ucSwitchOpt_destroy(s);
uc_PASS

uc_TEST(ucSwitchOpt_format_validation_error_creates_correct_error, ucSwitchOpt *switch_opt, ucSwitchTok *switch_tok, const char *correct_error)
    const char *switch_opt_validation_error = ucSwitchOpt_format_validation_err(switch_opt, cmd, switch_tok);
    ucSwitchOpt_destroy(switch_opt);
    uc_TRUE(uc_STR_EQ(correct_error, switch_opt_validation_error));
uc_PASS
uc_CASE(ucSwitchOpt_format_validation_error_creates_correct_error, if_it_is_required, ucSwitchOpt_create_required("-mys", NULL, NULL, NULL), NULL, ucOpt_INVALID "Switch '-mys' is required.")
uc_CASE(ucSwitchOpt_format_validation_error_creates_correct_error, if_an_arg_does_not_exist, ucSwitchOpt_create("-s", NULL, NULL, NULL), "-sw\0arg\0\n", ucOpt_INVALID "Switch '-s' requires no arguments.")
uc_CASE(ucSwitchOpt_format_validation_error_creates_correct_error, if_an_arg_is_required, ucSwitchOpt_create("-sw", NULL, ucArgOpt_create_required("mya", NULL, NULL), NULL), "-sw\0\n", ucOpt_INVALID "Argument 'mya' is required for switch '-sw'.")
uc_CASE(ucSwitchOpt_format_validation_error_creates_correct_error, if_too_many_args_are_passed, ucSwitchOpt_create("-w1", NULL, ucArgOpt_create("a1", NULL, NULL), NULL), "-w1\0arg1\0arg2\0\n", ucOpt_INVALID "Switch '-w1' has no option for argument 'arg2'.")

uc_TEST_GROUP(ucSwitchOpt, setup,
    ucSwitchOpt_get_next_returns_next,
    ucSwitchOpt_create_creates_switch_opt,
    ucSwitchOpt_create_required_creates_switch_opt,
    ucSwitchOpt_create_creates_different_instances,
    ucSwitchOpt_destroy_releases_instance,
    ucSwitchOpt_destroy_chain_releases_all_instances,
    ucSwitchOpt_format_validation_err_allows_multiple_arguments,
    ucSwitchOpt_format_validation_err_catches_required_switch,
    ucSwitchOpt_format_validation_err_catches_required_arg,
    ucSwitchOpt_format_validation_error_creates_correct_error_if_an_arg_does_not_exist,
    ucSwitchOpt_format_validation_error_creates_correct_error_if_an_arg_is_required,
    ucSwitchOpt_format_validation_error_creates_correct_error_if_it_is_required,
    ucSwitchOpt_format_validation_error_creates_correct_error_if_too_many_args_are_passed)