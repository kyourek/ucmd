#include <float.h>
#include <string.h>
#include "ucArgOpt_p.h"
#include "ucTest.h"

static ucTestErr ucArgOpt_is_numeric_returns_is_numeric(ucTestGroup *p) {
    ucArgOpt o;
    
    o.is_numeric = ucBool_false;
    ucTest_ASSERT(ucBool_false == ucArgOpt_is_numeric(&o));

    o.is_numeric = ucBool_true;
    ucTest_ASSERT(ucBool_true == ucArgOpt_is_numeric(&o));

    return ucTestErr_NONE;
}

static ucTestErr ucArgOpt_get_numeric_min_returns_value(ucTestGroup *p) {
    ucArgOpt o;
    o.numeric_min = -765.432;
    ucTest_ASSERT(-765.432 == ucArgOpt_get_numeric_min(&o));
    return ucTestErr_NONE;
}

static ucTestErr ucArgOpt_get_numeric_max_returns_value(ucTestGroup *p) {
    ucArgOpt o;
    o.numeric_max = 0.123456789;
    ucTest_ASSERT(0.123456789 == ucArgOpt_get_numeric_max(&o));
    return ucTestErr_NONE;
}

static ucTestErr ucArgOpt_create_creates_arg_opt(ucTestGroup *p) {
    ucArgOpt *ptr;
    ucArgOpt a;
    
    ptr = ucArgOpt_create("a-name", "a-desc", &a);
    
    ucTest_ASSERT(ucOpt_get_name((ucOpt*)ptr));
    ucTest_ASSERT(ucOpt_get_desc((ucOpt*)ptr));
    ucTest_ASSERT(&a == ucArgOpt_get_next(ptr));
    ucTest_ASSERT(ucBool_false == ucOpt_is_required((ucOpt*)ptr));
    ucTest_ASSERT(ucBool_false == ucArgOpt_is_numeric(ptr));
    ucTest_ASSERT(0 == ucArgOpt_get_min_tok_count(ptr));
    ucTest_ASSERT(1 == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);

    return ucTestErr_NONE;
}

static ucTestErr ucArgOpt_create_multiple_creates_arg_opt(ucTestGroup *p) {
    ucArgOpt *ptr;
    int min_tok_count = 4, max_tok_count = 37;
    const char *name = "a-name", *desc = "a-desc";

    ptr = ucArgOpt_create_multiple(name, desc, min_tok_count, max_tok_count);
    
    ucTest_ASSERT(0 == strcmp(name, ucOpt_get_name((ucOpt*)ptr)));
    ucTest_ASSERT(0 == strcmp(desc, ucOpt_get_desc((ucOpt*)ptr)));
    ucTest_ASSERT(NULL == ucArgOpt_get_next(ptr));
    ucTest_ASSERT(ucBool_true == ucOpt_is_required((ucOpt*)ptr));
    ucTest_ASSERT(ucBool_false == ucArgOpt_is_numeric(ptr));
    ucTest_ASSERT(min_tok_count == ucArgOpt_get_min_tok_count(ptr));
    ucTest_ASSERT(max_tok_count == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);

    ptr = ucArgOpt_create_multiple(name, desc, 0, max_tok_count);

    ucTest_ASSERT(ucBool_false == ucOpt_is_required((ucOpt*)ptr));

    ucArgOpt_destroy(ptr);

    return ucTestErr_NONE;
}

static ucTestErr ucArgOpt_create_required_creates_arg_opt(ucTestGroup *p) {
    ucArgOpt *ptr;
    ucArgOpt a;

    ptr = ucArgOpt_create_required("aname", "adesc", &a);

    ucTest_ASSERT(ucOpt_get_name((ucOpt*)ptr));
    ucTest_ASSERT(ucOpt_get_desc((ucOpt*)ptr));
    ucTest_ASSERT(&a == ucArgOpt_get_next(ptr));
    ucTest_ASSERT(ucBool_true == ucOpt_is_required((ucOpt*)ptr));
    ucTest_ASSERT(ucBool_false == ucArgOpt_is_numeric(ptr));
    ucTest_ASSERT(1 == ucArgOpt_get_min_tok_count(ptr));
    ucTest_ASSERT(1 == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);

    return ucTestErr_NONE;
}

static ucTestErr ucArgOpt_create_multiple_numeric_creates_arg_opt(ucTestGroup *p) {
    ucArgOpt *ptr;
    int min_tok_count = 65, max_tok_count = 107;
    double numeric_min = -203.41, numeric_max = +419.26;
    const char *desc = "my_desc";

    ptr = ucArgOpt_create_multiple_numeric(desc, min_tok_count, max_tok_count, numeric_min, numeric_max);
    
    ucTest_ASSERT(0 == strcmp(desc, ucOpt_get_desc((ucOpt*)ptr)));
    ucTest_ASSERT(NULL == ucArgOpt_get_next(ptr));
    ucTest_ASSERT(ucBool_true == ucOpt_is_required((ucOpt*)ptr));
    ucTest_ASSERT(ucBool_true == ucArgOpt_is_numeric(ptr));
    ucTest_ASSERT(min_tok_count == ucArgOpt_get_min_tok_count(ptr));
    ucTest_ASSERT(max_tok_count == ucArgOpt_get_max_tok_count(ptr));
    ucTest_ASSERT(numeric_min == ucArgOpt_get_numeric_min(ptr));
    ucTest_ASSERT(numeric_max == ucArgOpt_get_numeric_max(ptr));

    ucArgOpt_destroy(ptr);

    ptr = ucArgOpt_create_multiple_numeric(desc, 0, max_tok_count, numeric_min, numeric_max);

    ucTest_ASSERT(ucBool_false == ucOpt_is_required((ucOpt*)ptr));

    ucArgOpt_destroy(ptr);

    return ucTestErr_NONE;
}

static ucTestErr ucArgOpt_create_numeric_creates_arg_opt(ucTestGroup *p) {
    ucArgOpt *ptr;
    ucArgOpt a;
    
    ptr = ucArgOpt_create_numeric("ds", -5.678, 12.34, &a);
    
    ucTest_ASSERT(ucOpt_get_name((ucOpt*)ptr));
    ucTest_ASSERT(ucOpt_get_desc((ucOpt*)ptr));
    ucTest_ASSERT(&a == ucArgOpt_get_next(ptr));
    ucTest_ASSERT(ucBool_false == ucOpt_is_required((ucOpt*)ptr));
    ucTest_ASSERT(ucBool_true == ucArgOpt_is_numeric(ptr));
    ucTest_ASSERT(-5.678 == ucArgOpt_get_numeric_min(ptr));
    ucTest_ASSERT(12.34 == ucArgOpt_get_numeric_max(ptr));
    ucTest_ASSERT(0 == ucArgOpt_get_min_tok_count(ptr));
    ucTest_ASSERT(1 == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);

    return ucTestErr_NONE;
}

static ucTestErr ucArgOpt_create_required_numeric_creates_arg_opt(ucTestGroup *p) {
    ucArgOpt *ptr;
    ucArgOpt a;
    
    ptr = ucArgOpt_create_required_numeric("REQ", 100.436, 567.890, &a);
    
    ucTest_ASSERT(ucOpt_get_name((ucOpt*)ptr));
    ucTest_ASSERT(ucOpt_get_desc((ucOpt*)ptr));
    ucTest_ASSERT(&a == ucArgOpt_get_next(ptr));
    ucTest_ASSERT(ucBool_true == ucOpt_is_required((ucOpt*)ptr));
    ucTest_ASSERT(ucBool_true == ucArgOpt_is_numeric(ptr));
    ucTest_ASSERT(100.436 == ucArgOpt_get_numeric_min(ptr));
    ucTest_ASSERT(567.890 == ucArgOpt_get_numeric_max(ptr));
    ucTest_ASSERT(1 == ucArgOpt_get_min_tok_count(ptr));
    ucTest_ASSERT(1 == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);

    return ucTestErr_NONE;
}

static ucTestErr ucArgOpt_create_creates_different_instances(ucTestGroup *p) {
    ucArgOpt *o1, *o2, *o3;

    o1 = ucArgOpt_create("o1", "o1", NULL);
    o2 = ucArgOpt_create("o2", "o2", NULL);
    o3 = ucArgOpt_create("o3", "o3", NULL);

    ucTest_ASSERT(o1 != o2);
    ucTest_ASSERT(o2 != o3);

    ucArgOpt_destroy(o1);
    ucArgOpt_destroy(o2);
    ucArgOpt_destroy(o3);

    return ucTestErr_NONE;
}

static ucTestErr ucArgOpt_destroy_releases_instance(ucTestGroup *p) {
    ucArgOpt *o1, *o2, *o3;

    o1 = ucArgOpt_create("o1", "o1", NULL);
    o2 = ucArgOpt_create("o2", "o2", NULL);

    ucArgOpt_destroy(o2);

    o3 = ucArgOpt_create("o3", "o3", NULL);

    ucTest_ASSERT(o3 == o2);

    ucArgOpt_destroy(o1);
    ucArgOpt_destroy(o3);

    return ucTestErr_NONE;
}

static ucTestErr ucArgOpt_destroy_chain_releases_all_instances(ucTestGroup *p) {
    ucArgOpt *o1, *o2, *o3, *o1_2, *o2_2, *o3_2;

    o3 = ucArgOpt_create("o3", "o3", NULL);
    o2 = ucArgOpt_create("o2", "o2", o3);
    o1 = ucArgOpt_create("o1", "o1", o2);

    ucArgOpt_destroy_chain(o1);

    o3_2 = ucArgOpt_create("o3_2", "o3_2", NULL);
    o2_2 = ucArgOpt_create("o2_2", "o2_2", NULL);
    o1_2 = ucArgOpt_create("o1_2", "o1_2", NULL);

    ucTest_ASSERT(o3 == o3_2);
    ucTest_ASSERT(o2 == o2_2);
    ucTest_ASSERT(o1 == o1_2);

    ucArgOpt_destroy(o3_2);
    ucArgOpt_destroy(o2_2);
    ucArgOpt_destroy(o1_2);

    return ucTestErr_NONE;
}

static ucTestErr ucArgOpt_format_validation_err_catches_numeric_err(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_get_instance();
    ucArgOpt *a = ucArgOpt_create_numeric(NULL, -DBL_MAX, DBL_MAX, NULL);

    err = ucArgOpt_format_validation_err(a, cmd, "non-num", NULL);
    ucTest_ASSERT(NULL != err);

    err = ucArgOpt_format_validation_err(a, cmd, "-2.243", NULL);
    ucTest_ASSERT(NULL == err);

    ucArgOpt_destroy(a);
    return ucTestErr_NONE;
}

static ucTestErr ucArgOpt_format_validation_err_catches_out_of_range_numeric(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_get_instance();
    ucArgOpt *a = ucArgOpt_create_numeric(NULL, -5.4, +2.1, NULL);

    err = ucArgOpt_format_validation_err(a, cmd, "-5.5", NULL);
    ucTest_ASSERT(NULL != err);

    err = ucArgOpt_format_validation_err(a, cmd, "2.2", NULL);
    ucTest_ASSERT(NULL != err);

    err = ucArgOpt_format_validation_err(a, cmd, "-5.399", NULL);
    ucTest_ASSERT(NULL == err);

    err = ucArgOpt_format_validation_err(a, cmd, "2.099", NULL);
    ucTest_ASSERT(NULL == err);

    ucArgOpt_destroy(a);
    return ucTestErr_NONE;
}

static ucTestErr ucArgOpt_format_validation_err_catches_required_arg(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_get_instance();
    ucArgOpt *a = ucArgOpt_create_required("arg", NULL, NULL);

    err = ucArgOpt_format_validation_err(a, cmd, NULL, NULL);
    ucTest_ASSERT(NULL != err);

    err = ucArgOpt_format_validation_err(a, cmd, "bla", NULL);
    ucTest_ASSERT(NULL == err);

    ucArgOpt_destroy(a);
    return ucTestErr_NONE;
}

static ucTestErr ucArgOpt_format_validation_err_catches_not_enough_tokens(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_get_instance();
    ucArgOpt *a = ucArgOpt_create_multiple("arg", NULL, 4, 5);

    err = ucArgOpt_format_validation_err(a, cmd, "arg1\0arg2\0arg3\0\n", NULL);
    ucTest_ASSERT(NULL != err);

    ucArgOpt_destroy(a);

    return ucTestErr_NONE;
}

static ucTestErr ucArgOpt_format_validation_err_catches_too_many_tokens(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_get_instance();
    ucArgOpt *a = ucArgOpt_create_multiple("arg", NULL, 4, 5);

    err = ucArgOpt_format_validation_err(a, cmd, "arg1\0arg2\0arg3\0arg4\0arg5\0arg6\0\n", NULL);
    ucTest_ASSERT(NULL != err);

    ucArgOpt_destroy(a);

    return ucTestErr_NONE;
}

static ucTestErr ucArgOpt_format_validation_err_allows_correct_number_of_tokens(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_get_instance();
    ucArgOpt *a = ucArgOpt_create_multiple("arg", NULL, 3, 3);

    err = ucArgOpt_format_validation_err(a, cmd, "arg1\0arg2\0arg3\0\n", NULL);
    ucTest_ASSERT(NULL == err);

    ucArgOpt_destroy(a);

    return ucTestErr_NONE;
}

static ucTestErr ucArgOpt_format_validation_err_catches_multiple_numbers(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_get_instance();
    ucArgOpt *a = ucArgOpt_create_multiple_numeric(NULL, 2, 8, -100, +100);

    err = ucArgOpt_format_validation_err(a, cmd, "3" "\0" "4.789" "\0" "notnum" "\0" "91.23" "\0\n", NULL);
    ucTest_ASSERT(NULL != err);

    err = ucArgOpt_format_validation_err(a, cmd, "89.1" "\0" "4.789" "\0" "0.987" "\0" "91.23" "\0\n", NULL);
    ucTest_ASSERT(NULL == err);

    err = ucArgOpt_format_validation_err(a, cmd, "89.1" "\0" "4.789" "\0" "987" "\0" "91.23" "\0\n", NULL);
    ucTest_ASSERT(NULL != err);

    ucArgOpt_destroy(a);

    return ucTestErr_NONE;
}

ucTestGroup *ucArgOpt_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_TestFunc *tests[] = {
        ucArgOpt_is_numeric_returns_is_numeric,
        ucArgOpt_get_numeric_min_returns_value,
        ucArgOpt_get_numeric_max_returns_value,
        ucArgOpt_create_creates_arg_opt,
        ucArgOpt_create_required_creates_arg_opt,
        ucArgOpt_create_numeric_creates_arg_opt,
        ucArgOpt_create_required_numeric_creates_arg_opt,
        ucArgOpt_create_creates_different_instances,
        ucArgOpt_destroy_releases_instance,
        ucArgOpt_destroy_chain_releases_all_instances,
        ucArgOpt_format_validation_err_catches_numeric_err,
        ucArgOpt_format_validation_err_catches_out_of_range_numeric,
        ucArgOpt_format_validation_err_catches_required_arg,
        ucArgOpt_create_multiple_creates_arg_opt,
        ucArgOpt_create_multiple_numeric_creates_arg_opt,
        ucArgOpt_format_validation_err_catches_not_enough_tokens,
        ucArgOpt_format_validation_err_catches_too_many_tokens,
        ucArgOpt_format_validation_err_allows_correct_number_of_tokens,
        ucArgOpt_format_validation_err_catches_multiple_numbers,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
