#include <float.h>
#include <string.h>
#include "cl_arg_opt_p.h"
#include "cl_test.h"

static cl_test_err cl_arg_opt_is_numeric_returns_is_numeric(cl_test_group *p) {
    cl_arg_opt o;
    
    o.is_numeric = CL_FALSE;
    CL_TEST_ASSERT(CL_FALSE == cl_arg_opt_is_numeric(&o));

    o.is_numeric = CL_TRUE;
    CL_TEST_ASSERT(CL_TRUE == cl_arg_opt_is_numeric(&o));

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_opt_get_numeric_min_returns_value(cl_test_group *p) {
    cl_arg_opt o;
    o.numeric_min = -765.432;
    CL_TEST_ASSERT(-765.432 == cl_arg_opt_get_numeric_min(&o));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_opt_get_numeric_max_returns_value(cl_test_group *p) {
    cl_arg_opt o;
    o.numeric_max = 0.123456789;
    CL_TEST_ASSERT(0.123456789 == cl_arg_opt_get_numeric_max(&o));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_opt_create_creates_arg_opt(cl_test_group *p) {
    cl_arg_opt *ptr;
    cl_arg_opt a;
    
    ptr = cl_arg_opt_create("a-name", "a-desc", &a);
    
    CL_TEST_ASSERT(cl_opt_get_name((cl_opt*)ptr));
    CL_TEST_ASSERT(cl_opt_get_desc((cl_opt*)ptr));
    CL_TEST_ASSERT(&a == cl_arg_opt_get_next(ptr));
    CL_TEST_ASSERT(CL_FALSE == cl_opt_is_required((cl_opt*)ptr));
    CL_TEST_ASSERT(CL_FALSE == cl_arg_opt_is_numeric(ptr));
    CL_TEST_ASSERT(0 == cl_arg_opt_get_min_tok_count(ptr));
    CL_TEST_ASSERT(1 == cl_arg_opt_get_max_tok_count(ptr));

    cl_arg_opt_destroy(ptr);

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_opt_create_multiple_creates_arg_opt(cl_test_group *p) {
    cl_arg_opt *ptr;
    int min_tok_count = 4, max_tok_count = 37;
    const char *name = "a-name", *desc = "a-desc";

    ptr = cl_arg_opt_create_multiple(name, desc, min_tok_count, max_tok_count);
    
    CL_TEST_ASSERT(0 == strcmp(name, cl_opt_get_name((cl_opt*)ptr)));
    CL_TEST_ASSERT(0 == strcmp(desc, cl_opt_get_desc((cl_opt*)ptr)));
    CL_TEST_ASSERT(NULL == cl_arg_opt_get_next(ptr));
    CL_TEST_ASSERT(CL_TRUE == cl_opt_is_required((cl_opt*)ptr));
    CL_TEST_ASSERT(CL_FALSE == cl_arg_opt_is_numeric(ptr));
    CL_TEST_ASSERT(min_tok_count == cl_arg_opt_get_min_tok_count(ptr));
    CL_TEST_ASSERT(max_tok_count == cl_arg_opt_get_max_tok_count(ptr));

    cl_arg_opt_destroy(ptr);

    ptr = cl_arg_opt_create_multiple(name, desc, 0, max_tok_count);

    CL_TEST_ASSERT(CL_FALSE == cl_opt_is_required((cl_opt*)ptr));

    cl_arg_opt_destroy(ptr);

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_opt_create_required_creates_arg_opt(cl_test_group *p) {
    cl_arg_opt *ptr;
    cl_arg_opt a;

    ptr = cl_arg_opt_create_required("aname", "adesc", &a);

    CL_TEST_ASSERT(cl_opt_get_name((cl_opt*)ptr));
    CL_TEST_ASSERT(cl_opt_get_desc((cl_opt*)ptr));
    CL_TEST_ASSERT(&a == cl_arg_opt_get_next(ptr));
    CL_TEST_ASSERT(CL_TRUE == cl_opt_is_required((cl_opt*)ptr));
    CL_TEST_ASSERT(CL_FALSE == cl_arg_opt_is_numeric(ptr));
    CL_TEST_ASSERT(1 == cl_arg_opt_get_min_tok_count(ptr));
    CL_TEST_ASSERT(1 == cl_arg_opt_get_max_tok_count(ptr));

    cl_arg_opt_destroy(ptr);

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_opt_create_multiple_numeric_creates_arg_opt(cl_test_group *p) {
    cl_arg_opt *ptr;
    int min_tok_count = 65, max_tok_count = 107;
    double numeric_min = -203.41, numeric_max = +419.26;
    const char *desc = "my_desc";

    ptr = cl_arg_opt_create_multiple_numeric(desc, min_tok_count, max_tok_count, numeric_min, numeric_max);
    
    CL_TEST_ASSERT(0 == strcmp(desc, cl_opt_get_desc((cl_opt*)ptr)));
    CL_TEST_ASSERT(NULL == cl_arg_opt_get_next(ptr));
    CL_TEST_ASSERT(CL_TRUE == cl_opt_is_required((cl_opt*)ptr));
    CL_TEST_ASSERT(CL_TRUE == cl_arg_opt_is_numeric(ptr));
    CL_TEST_ASSERT(min_tok_count == cl_arg_opt_get_min_tok_count(ptr));
    CL_TEST_ASSERT(max_tok_count == cl_arg_opt_get_max_tok_count(ptr));
    CL_TEST_ASSERT(numeric_min == cl_arg_opt_get_numeric_min(ptr));
    CL_TEST_ASSERT(numeric_max == cl_arg_opt_get_numeric_max(ptr));

    cl_arg_opt_destroy(ptr);

    ptr = cl_arg_opt_create_multiple_numeric(desc, 0, max_tok_count, numeric_min, numeric_max);

    CL_TEST_ASSERT(CL_FALSE == cl_opt_is_required((cl_opt*)ptr));

    cl_arg_opt_destroy(ptr);

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_opt_create_numeric_creates_arg_opt(cl_test_group *p) {
    cl_arg_opt *ptr;
    cl_arg_opt a;
    
    ptr = cl_arg_opt_create_numeric("ds", -5.678, 12.34, &a);
    
    CL_TEST_ASSERT(cl_opt_get_name((cl_opt*)ptr));
    CL_TEST_ASSERT(cl_opt_get_desc((cl_opt*)ptr));
    CL_TEST_ASSERT(&a == cl_arg_opt_get_next(ptr));
    CL_TEST_ASSERT(CL_FALSE == cl_opt_is_required((cl_opt*)ptr));
    CL_TEST_ASSERT(CL_TRUE == cl_arg_opt_is_numeric(ptr));
    CL_TEST_ASSERT(-5.678 == cl_arg_opt_get_numeric_min(ptr));
    CL_TEST_ASSERT(12.34 == cl_arg_opt_get_numeric_max(ptr));
    CL_TEST_ASSERT(0 == cl_arg_opt_get_min_tok_count(ptr));
    CL_TEST_ASSERT(1 == cl_arg_opt_get_max_tok_count(ptr));

    cl_arg_opt_destroy(ptr);

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_opt_create_required_numeric_creates_arg_opt(cl_test_group *p) {
    cl_arg_opt *ptr;
    cl_arg_opt a;
    
    ptr = cl_arg_opt_create_required_numeric("REQ", 100.436, 567.890, &a);
    
    CL_TEST_ASSERT(cl_opt_get_name((cl_opt*)ptr));
    CL_TEST_ASSERT(cl_opt_get_desc((cl_opt*)ptr));
    CL_TEST_ASSERT(&a == cl_arg_opt_get_next(ptr));
    CL_TEST_ASSERT(CL_TRUE == cl_opt_is_required((cl_opt*)ptr));
    CL_TEST_ASSERT(CL_TRUE == cl_arg_opt_is_numeric(ptr));
    CL_TEST_ASSERT(100.436 == cl_arg_opt_get_numeric_min(ptr));
    CL_TEST_ASSERT(567.890 == cl_arg_opt_get_numeric_max(ptr));
    CL_TEST_ASSERT(1 == cl_arg_opt_get_min_tok_count(ptr));
    CL_TEST_ASSERT(1 == cl_arg_opt_get_max_tok_count(ptr));

    cl_arg_opt_destroy(ptr);

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_opt_create_creates_different_instances(cl_test_group *p) {
    cl_arg_opt *o1, *o2, *o3;

    o1 = cl_arg_opt_create("o1", "o1", NULL);
    o2 = cl_arg_opt_create("o2", "o2", NULL);
    o3 = cl_arg_opt_create("o3", "o3", NULL);

    CL_TEST_ASSERT(o1 != o2);
    CL_TEST_ASSERT(o2 != o3);

    cl_arg_opt_destroy(o1);
    cl_arg_opt_destroy(o2);
    cl_arg_opt_destroy(o3);

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_opt_destroy_releases_instance(cl_test_group *p) {
    cl_arg_opt *o1, *o2, *o3;

    o1 = cl_arg_opt_create("o1", "o1", NULL);
    o2 = cl_arg_opt_create("o2", "o2", NULL);

    cl_arg_opt_destroy(o2);

    o3 = cl_arg_opt_create("o3", "o3", NULL);

    CL_TEST_ASSERT(o3 == o2);

    cl_arg_opt_destroy(o1);
    cl_arg_opt_destroy(o3);

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_opt_destroy_chain_releases_all_instances(cl_test_group *p) {
    cl_arg_opt *o1, *o2, *o3, *o1_2, *o2_2, *o3_2;

    o3 = cl_arg_opt_create("o3", "o3", NULL);
    o2 = cl_arg_opt_create("o2", "o2", o3);
    o1 = cl_arg_opt_create("o1", "o1", o2);

    cl_arg_opt_destroy_chain(o1);

    o3_2 = cl_arg_opt_create("o3_2", "o3_2", NULL);
    o2_2 = cl_arg_opt_create("o2_2", "o2_2", NULL);
    o1_2 = cl_arg_opt_create("o1_2", "o1_2", NULL);

    CL_TEST_ASSERT(o3 == o3_2);
    CL_TEST_ASSERT(o2 == o2_2);
    CL_TEST_ASSERT(o1 == o1_2);

    cl_arg_opt_destroy(o3_2);
    cl_arg_opt_destroy(o2_2);
    cl_arg_opt_destroy(o1_2);

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_opt_format_validation_err_catches_numeric_err(cl_test_group *p) {
    const char *err;
    cl_cmd_line *cmd = cl_cmd_line_get_instance();
    cl_arg_opt *a = cl_arg_opt_create_numeric(NULL, -DBL_MAX, DBL_MAX, NULL);

    err = cl_arg_opt_format_validation_err(a, cmd, "non-num", NULL);
    CL_TEST_ASSERT(NULL != err);

    err = cl_arg_opt_format_validation_err(a, cmd, "-2.243", NULL);
    CL_TEST_ASSERT(NULL == err);

    cl_arg_opt_destroy(a);
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_opt_format_validation_err_catches_out_of_range_numeric(cl_test_group *p) {
    const char *err;
    cl_cmd_line *cmd = cl_cmd_line_get_instance();
    cl_arg_opt *a = cl_arg_opt_create_numeric(NULL, -5.4, +2.1, NULL);

    err = cl_arg_opt_format_validation_err(a, cmd, "-5.5", NULL);
    CL_TEST_ASSERT(NULL != err);

    err = cl_arg_opt_format_validation_err(a, cmd, "2.2", NULL);
    CL_TEST_ASSERT(NULL != err);

    err = cl_arg_opt_format_validation_err(a, cmd, "-5.399", NULL);
    CL_TEST_ASSERT(NULL == err);

    err = cl_arg_opt_format_validation_err(a, cmd, "2.099", NULL);
    CL_TEST_ASSERT(NULL == err);

    cl_arg_opt_destroy(a);
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_opt_format_validation_err_catches_required_arg(cl_test_group *p) {
    const char *err;
    cl_cmd_line *cmd = cl_cmd_line_get_instance();
    cl_arg_opt *a = cl_arg_opt_create_required("arg", NULL, NULL);

    err = cl_arg_opt_format_validation_err(a, cmd, NULL, NULL);
    CL_TEST_ASSERT(NULL != err);

    err = cl_arg_opt_format_validation_err(a, cmd, "bla", NULL);
    CL_TEST_ASSERT(NULL == err);

    cl_arg_opt_destroy(a);
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_opt_format_validation_err_catches_not_enough_tokens(cl_test_group *p) {
    const char *err;
    cl_cmd_line *cmd = cl_cmd_line_get_instance();
    cl_arg_opt *a = cl_arg_opt_create_multiple("arg", NULL, 4, 5);

    err = cl_arg_opt_format_validation_err(a, cmd, "arg1\0arg2\0arg3\0\n", NULL);
    CL_TEST_ASSERT(NULL != err);

    cl_arg_opt_destroy(a);

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_opt_format_validation_err_catches_too_many_tokens(cl_test_group *p) {
    const char *err;
    cl_cmd_line *cmd = cl_cmd_line_get_instance();
    cl_arg_opt *a = cl_arg_opt_create_multiple("arg", NULL, 4, 5);

    err = cl_arg_opt_format_validation_err(a, cmd, "arg1\0arg2\0arg3\0arg4\0arg5\0arg6\0\n", NULL);
    CL_TEST_ASSERT(NULL != err);

    cl_arg_opt_destroy(a);

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_opt_format_validation_err_allows_correct_number_of_tokens(cl_test_group *p) {
    const char *err;
    cl_cmd_line *cmd = cl_cmd_line_get_instance();
    cl_arg_opt *a = cl_arg_opt_create_multiple("arg", NULL, 3, 3);

    err = cl_arg_opt_format_validation_err(a, cmd, "arg1\0arg2\0arg3\0\n", NULL);
    CL_TEST_ASSERT(NULL == err);

    cl_arg_opt_destroy(a);

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_opt_format_validation_err_catches_multiple_numbers(cl_test_group *p) {
    const char *err;
    cl_cmd_line *cmd = cl_cmd_line_get_instance();
    cl_arg_opt *a = cl_arg_opt_create_multiple_numeric(NULL, 2, 8, -100, +100);

    err = cl_arg_opt_format_validation_err(a, cmd, "3" "\0" "4.789" "\0" "notnum" "\0" "91.23" "\0\n", NULL);
    CL_TEST_ASSERT(NULL != err);

    err = cl_arg_opt_format_validation_err(a, cmd, "89.1" "\0" "4.789" "\0" "0.987" "\0" "91.23" "\0\n", NULL);
    CL_TEST_ASSERT(NULL == err);

    err = cl_arg_opt_format_validation_err(a, cmd, "89.1" "\0" "4.789" "\0" "987" "\0" "91.23" "\0\n", NULL);
    CL_TEST_ASSERT(NULL != err);

    cl_arg_opt_destroy(a);

    return CL_TEST_ERR_NONE;
}

cl_test_group *cl_arg_opt_tests_get_group(void) {
    static cl_test_group group;
    static cl_test_group_test_func *tests[] = {
        cl_arg_opt_is_numeric_returns_is_numeric,
        cl_arg_opt_get_numeric_min_returns_value,
        cl_arg_opt_get_numeric_max_returns_value,
        cl_arg_opt_create_creates_arg_opt,
        cl_arg_opt_create_required_creates_arg_opt,
        cl_arg_opt_create_numeric_creates_arg_opt,
        cl_arg_opt_create_required_numeric_creates_arg_opt,
        cl_arg_opt_create_creates_different_instances,
        cl_arg_opt_destroy_releases_instance,
        cl_arg_opt_destroy_chain_releases_all_instances,
        cl_arg_opt_format_validation_err_catches_numeric_err,
        cl_arg_opt_format_validation_err_catches_out_of_range_numeric,
        cl_arg_opt_format_validation_err_catches_required_arg,
        cl_arg_opt_create_multiple_creates_arg_opt,
        cl_arg_opt_create_multiple_numeric_creates_arg_opt,
        cl_arg_opt_format_validation_err_catches_not_enough_tokens,
        cl_arg_opt_format_validation_err_catches_too_many_tokens,
        cl_arg_opt_format_validation_err_allows_correct_number_of_tokens,
        cl_arg_opt_format_validation_err_catches_multiple_numbers,
        NULL
    };

    return cl_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
