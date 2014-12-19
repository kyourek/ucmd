#include <float.h>
#include <string.h>
#include "ucArgOpt_p.h"
#include "uc_test.h"

static uc_test_err ucArgOpt_is_numeric_returns_is_numeric(uc_test_group *p) {
    ucArgOpt o;
    
    o.is_numeric = ucBool_false;
    UC_TEST_ASSERT(ucBool_false == ucArgOpt_is_numeric(&o));

    o.is_numeric = ucBool_true;
    UC_TEST_ASSERT(ucBool_true == ucArgOpt_is_numeric(&o));

    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgOpt_get_numeric_min_returns_value(uc_test_group *p) {
    ucArgOpt o;
    o.numeric_min = -765.432;
    UC_TEST_ASSERT(-765.432 == ucArgOpt_get_numeric_min(&o));
    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgOpt_get_numeric_max_returns_value(uc_test_group *p) {
    ucArgOpt o;
    o.numeric_max = 0.123456789;
    UC_TEST_ASSERT(0.123456789 == ucArgOpt_get_numeric_max(&o));
    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgOpt_create_creates_arg_opt(uc_test_group *p) {
    ucArgOpt *ptr;
    ucArgOpt a;
    
    ptr = ucArgOpt_create("a-name", "a-desc", &a);
    
    UC_TEST_ASSERT(uc_opt_get_name((uc_opt*)ptr));
    UC_TEST_ASSERT(uc_opt_get_desc((uc_opt*)ptr));
    UC_TEST_ASSERT(&a == ucArgOpt_get_next(ptr));
    UC_TEST_ASSERT(ucBool_false == uc_opt_is_required((uc_opt*)ptr));
    UC_TEST_ASSERT(ucBool_false == ucArgOpt_is_numeric(ptr));
    UC_TEST_ASSERT(0 == ucArgOpt_get_min_tok_count(ptr));
    UC_TEST_ASSERT(1 == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);

    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgOpt_create_multiple_creates_arg_opt(uc_test_group *p) {
    ucArgOpt *ptr;
    int min_tok_count = 4, max_tok_count = 37;
    const char *name = "a-name", *desc = "a-desc";

    ptr = ucArgOpt_create_multiple(name, desc, min_tok_count, max_tok_count);
    
    UC_TEST_ASSERT(0 == strcmp(name, uc_opt_get_name((uc_opt*)ptr)));
    UC_TEST_ASSERT(0 == strcmp(desc, uc_opt_get_desc((uc_opt*)ptr)));
    UC_TEST_ASSERT(NULL == ucArgOpt_get_next(ptr));
    UC_TEST_ASSERT(ucBool_true == uc_opt_is_required((uc_opt*)ptr));
    UC_TEST_ASSERT(ucBool_false == ucArgOpt_is_numeric(ptr));
    UC_TEST_ASSERT(min_tok_count == ucArgOpt_get_min_tok_count(ptr));
    UC_TEST_ASSERT(max_tok_count == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);

    ptr = ucArgOpt_create_multiple(name, desc, 0, max_tok_count);

    UC_TEST_ASSERT(ucBool_false == uc_opt_is_required((uc_opt*)ptr));

    ucArgOpt_destroy(ptr);

    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgOpt_create_required_creates_arg_opt(uc_test_group *p) {
    ucArgOpt *ptr;
    ucArgOpt a;

    ptr = ucArgOpt_create_required("aname", "adesc", &a);

    UC_TEST_ASSERT(uc_opt_get_name((uc_opt*)ptr));
    UC_TEST_ASSERT(uc_opt_get_desc((uc_opt*)ptr));
    UC_TEST_ASSERT(&a == ucArgOpt_get_next(ptr));
    UC_TEST_ASSERT(ucBool_true == uc_opt_is_required((uc_opt*)ptr));
    UC_TEST_ASSERT(ucBool_false == ucArgOpt_is_numeric(ptr));
    UC_TEST_ASSERT(1 == ucArgOpt_get_min_tok_count(ptr));
    UC_TEST_ASSERT(1 == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);

    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgOpt_create_multiple_numeric_creates_arg_opt(uc_test_group *p) {
    ucArgOpt *ptr;
    int min_tok_count = 65, max_tok_count = 107;
    double numeric_min = -203.41, numeric_max = +419.26;
    const char *desc = "my_desc";

    ptr = ucArgOpt_create_multiple_numeric(desc, min_tok_count, max_tok_count, numeric_min, numeric_max);
    
    UC_TEST_ASSERT(0 == strcmp(desc, uc_opt_get_desc((uc_opt*)ptr)));
    UC_TEST_ASSERT(NULL == ucArgOpt_get_next(ptr));
    UC_TEST_ASSERT(ucBool_true == uc_opt_is_required((uc_opt*)ptr));
    UC_TEST_ASSERT(ucBool_true == ucArgOpt_is_numeric(ptr));
    UC_TEST_ASSERT(min_tok_count == ucArgOpt_get_min_tok_count(ptr));
    UC_TEST_ASSERT(max_tok_count == ucArgOpt_get_max_tok_count(ptr));
    UC_TEST_ASSERT(numeric_min == ucArgOpt_get_numeric_min(ptr));
    UC_TEST_ASSERT(numeric_max == ucArgOpt_get_numeric_max(ptr));

    ucArgOpt_destroy(ptr);

    ptr = ucArgOpt_create_multiple_numeric(desc, 0, max_tok_count, numeric_min, numeric_max);

    UC_TEST_ASSERT(ucBool_false == uc_opt_is_required((uc_opt*)ptr));

    ucArgOpt_destroy(ptr);

    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgOpt_create_numeric_creates_arg_opt(uc_test_group *p) {
    ucArgOpt *ptr;
    ucArgOpt a;
    
    ptr = ucArgOpt_create_numeric("ds", -5.678, 12.34, &a);
    
    UC_TEST_ASSERT(uc_opt_get_name((uc_opt*)ptr));
    UC_TEST_ASSERT(uc_opt_get_desc((uc_opt*)ptr));
    UC_TEST_ASSERT(&a == ucArgOpt_get_next(ptr));
    UC_TEST_ASSERT(ucBool_false == uc_opt_is_required((uc_opt*)ptr));
    UC_TEST_ASSERT(ucBool_true == ucArgOpt_is_numeric(ptr));
    UC_TEST_ASSERT(-5.678 == ucArgOpt_get_numeric_min(ptr));
    UC_TEST_ASSERT(12.34 == ucArgOpt_get_numeric_max(ptr));
    UC_TEST_ASSERT(0 == ucArgOpt_get_min_tok_count(ptr));
    UC_TEST_ASSERT(1 == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);

    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgOpt_create_required_numeric_creates_arg_opt(uc_test_group *p) {
    ucArgOpt *ptr;
    ucArgOpt a;
    
    ptr = ucArgOpt_create_required_numeric("REQ", 100.436, 567.890, &a);
    
    UC_TEST_ASSERT(uc_opt_get_name((uc_opt*)ptr));
    UC_TEST_ASSERT(uc_opt_get_desc((uc_opt*)ptr));
    UC_TEST_ASSERT(&a == ucArgOpt_get_next(ptr));
    UC_TEST_ASSERT(ucBool_true == uc_opt_is_required((uc_opt*)ptr));
    UC_TEST_ASSERT(ucBool_true == ucArgOpt_is_numeric(ptr));
    UC_TEST_ASSERT(100.436 == ucArgOpt_get_numeric_min(ptr));
    UC_TEST_ASSERT(567.890 == ucArgOpt_get_numeric_max(ptr));
    UC_TEST_ASSERT(1 == ucArgOpt_get_min_tok_count(ptr));
    UC_TEST_ASSERT(1 == ucArgOpt_get_max_tok_count(ptr));

    ucArgOpt_destroy(ptr);

    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgOpt_create_creates_different_instances(uc_test_group *p) {
    ucArgOpt *o1, *o2, *o3;

    o1 = ucArgOpt_create("o1", "o1", NULL);
    o2 = ucArgOpt_create("o2", "o2", NULL);
    o3 = ucArgOpt_create("o3", "o3", NULL);

    UC_TEST_ASSERT(o1 != o2);
    UC_TEST_ASSERT(o2 != o3);

    ucArgOpt_destroy(o1);
    ucArgOpt_destroy(o2);
    ucArgOpt_destroy(o3);

    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgOpt_destroy_releases_instance(uc_test_group *p) {
    ucArgOpt *o1, *o2, *o3;

    o1 = ucArgOpt_create("o1", "o1", NULL);
    o2 = ucArgOpt_create("o2", "o2", NULL);

    ucArgOpt_destroy(o2);

    o3 = ucArgOpt_create("o3", "o3", NULL);

    UC_TEST_ASSERT(o3 == o2);

    ucArgOpt_destroy(o1);
    ucArgOpt_destroy(o3);

    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgOpt_destroy_chain_releases_all_instances(uc_test_group *p) {
    ucArgOpt *o1, *o2, *o3, *o1_2, *o2_2, *o3_2;

    o3 = ucArgOpt_create("o3", "o3", NULL);
    o2 = ucArgOpt_create("o2", "o2", o3);
    o1 = ucArgOpt_create("o1", "o1", o2);

    ucArgOpt_destroy_chain(o1);

    o3_2 = ucArgOpt_create("o3_2", "o3_2", NULL);
    o2_2 = ucArgOpt_create("o2_2", "o2_2", NULL);
    o1_2 = ucArgOpt_create("o1_2", "o1_2", NULL);

    UC_TEST_ASSERT(o3 == o3_2);
    UC_TEST_ASSERT(o2 == o2_2);
    UC_TEST_ASSERT(o1 == o1_2);

    ucArgOpt_destroy(o3_2);
    ucArgOpt_destroy(o2_2);
    ucArgOpt_destroy(o1_2);

    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgOpt_format_validation_err_catches_numeric_err(uc_test_group *p) {
    const char *err;
    uc_cmd_line *cmd = uc_cmd_line_get_instance();
    ucArgOpt *a = ucArgOpt_create_numeric(NULL, -DBL_MAX, DBL_MAX, NULL);

    err = ucArgOpt_format_validation_err(a, cmd, "non-num", NULL);
    UC_TEST_ASSERT(NULL != err);

    err = ucArgOpt_format_validation_err(a, cmd, "-2.243", NULL);
    UC_TEST_ASSERT(NULL == err);

    ucArgOpt_destroy(a);
    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgOpt_format_validation_err_catches_out_of_range_numeric(uc_test_group *p) {
    const char *err;
    uc_cmd_line *cmd = uc_cmd_line_get_instance();
    ucArgOpt *a = ucArgOpt_create_numeric(NULL, -5.4, +2.1, NULL);

    err = ucArgOpt_format_validation_err(a, cmd, "-5.5", NULL);
    UC_TEST_ASSERT(NULL != err);

    err = ucArgOpt_format_validation_err(a, cmd, "2.2", NULL);
    UC_TEST_ASSERT(NULL != err);

    err = ucArgOpt_format_validation_err(a, cmd, "-5.399", NULL);
    UC_TEST_ASSERT(NULL == err);

    err = ucArgOpt_format_validation_err(a, cmd, "2.099", NULL);
    UC_TEST_ASSERT(NULL == err);

    ucArgOpt_destroy(a);
    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgOpt_format_validation_err_catches_required_arg(uc_test_group *p) {
    const char *err;
    uc_cmd_line *cmd = uc_cmd_line_get_instance();
    ucArgOpt *a = ucArgOpt_create_required("arg", NULL, NULL);

    err = ucArgOpt_format_validation_err(a, cmd, NULL, NULL);
    UC_TEST_ASSERT(NULL != err);

    err = ucArgOpt_format_validation_err(a, cmd, "bla", NULL);
    UC_TEST_ASSERT(NULL == err);

    ucArgOpt_destroy(a);
    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgOpt_format_validation_err_catches_not_enough_tokens(uc_test_group *p) {
    const char *err;
    uc_cmd_line *cmd = uc_cmd_line_get_instance();
    ucArgOpt *a = ucArgOpt_create_multiple("arg", NULL, 4, 5);

    err = ucArgOpt_format_validation_err(a, cmd, "arg1\0arg2\0arg3\0\n", NULL);
    UC_TEST_ASSERT(NULL != err);

    ucArgOpt_destroy(a);

    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgOpt_format_validation_err_catches_too_many_tokens(uc_test_group *p) {
    const char *err;
    uc_cmd_line *cmd = uc_cmd_line_get_instance();
    ucArgOpt *a = ucArgOpt_create_multiple("arg", NULL, 4, 5);

    err = ucArgOpt_format_validation_err(a, cmd, "arg1\0arg2\0arg3\0arg4\0arg5\0arg6\0\n", NULL);
    UC_TEST_ASSERT(NULL != err);

    ucArgOpt_destroy(a);

    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgOpt_format_validation_err_allows_correct_number_of_tokens(uc_test_group *p) {
    const char *err;
    uc_cmd_line *cmd = uc_cmd_line_get_instance();
    ucArgOpt *a = ucArgOpt_create_multiple("arg", NULL, 3, 3);

    err = ucArgOpt_format_validation_err(a, cmd, "arg1\0arg2\0arg3\0\n", NULL);
    UC_TEST_ASSERT(NULL == err);

    ucArgOpt_destroy(a);

    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgOpt_format_validation_err_catches_multiple_numbers(uc_test_group *p) {
    const char *err;
    uc_cmd_line *cmd = uc_cmd_line_get_instance();
    ucArgOpt *a = ucArgOpt_create_multiple_numeric(NULL, 2, 8, -100, +100);

    err = ucArgOpt_format_validation_err(a, cmd, "3" "\0" "4.789" "\0" "notnum" "\0" "91.23" "\0\n", NULL);
    UC_TEST_ASSERT(NULL != err);

    err = ucArgOpt_format_validation_err(a, cmd, "89.1" "\0" "4.789" "\0" "0.987" "\0" "91.23" "\0\n", NULL);
    UC_TEST_ASSERT(NULL == err);

    err = ucArgOpt_format_validation_err(a, cmd, "89.1" "\0" "4.789" "\0" "987" "\0" "91.23" "\0\n", NULL);
    UC_TEST_ASSERT(NULL != err);

    ucArgOpt_destroy(a);

    return UC_TEST_ERR_NONE;
}

uc_test_group *ucArgOpt_tests_get_group(void) {
    static uc_test_group group;
    static uc_test_group_test_func *tests[] = {
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

    return uc_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
