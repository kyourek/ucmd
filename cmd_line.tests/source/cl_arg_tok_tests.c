#include <string.h>
#include "cl_arg_tok.h"
#include "cl_arg_tok_tests.h"
#include "cl_test.h"
#include "cl_tok.h"

static cl_test_err cl_arg_tok_get_next_returns_next(cl_test_group *p) {
    cl_arg_tok *a1 = "a1\0a2\0\n";
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_arg_tok_get_next(a1), "a2"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_tok_get_next_returns_null_when_followed_by_switch(cl_test_group *p) {
    cl_arg_tok *a = "a\0-s";
    CL_TEST_ASSERT(NULL == cl_arg_tok_get_next(a));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_tok_get_next_returns_null_when_followed_by_terminator(cl_test_group *p) {
    cl_arg_tok *a = "arg\0\n";
    CL_TEST_ASSERT(NULL == cl_arg_tok_get_next(a));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_tok_count_counts_arguments(cl_test_group *p) {
    cl_arg_tok *a1 = "a1\0a2\0a3\0\n";
    CL_TEST_ASSERT(3 == cl_arg_tok_count(a1));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_tok_find_finds_argument(cl_test_group *p) {
    cl_arg_tok *a1 = "a1\0a2\0a3\0\n";
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_arg_tok_find(a1, "a2"), "a2"));
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_arg_tok_find(a1, "a3"), "a3"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_tok_find_returns_null_if_no_argument_match(cl_test_group *p) {
    cl_arg_tok *a1 = "a1\0a2\0a3\0\n";
    CL_TEST_ASSERT(NULL == cl_arg_tok_find(a1, "a4"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_tok_contains_returns_true_when_contained(cl_test_group *p) {
    cl_arg_tok *a1 = "a1\0a2\0a3\0\n";
    CL_TEST_ASSERT(CL_TRUE == cl_arg_tok_contains(a1, "a2"));
    CL_TEST_ASSERT(CL_TRUE == cl_arg_tok_contains(a1, "a3"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_arg_tok_contains_returns_false_when_not_contained(cl_test_group *p) {
    cl_arg_tok *a1 = "a1\0a2\0a3\0\n";
    CL_TEST_ASSERT(CL_FALSE == cl_arg_tok_contains(a1, "a4"));
    return CL_TEST_ERR_NONE;
}

cl_test_group *cl_arg_tok_tests_get_group(void) {
    static cl_test_group group;
    static cl_test_group_test_func *tests[] = {
        cl_arg_tok_get_next_returns_next,
        cl_arg_tok_count_counts_arguments,
        cl_arg_tok_find_finds_argument,
        cl_arg_tok_find_returns_null_if_no_argument_match,
        cl_arg_tok_contains_returns_true_when_contained,
        cl_arg_tok_contains_returns_false_when_not_contained,
        cl_arg_tok_get_next_returns_null_when_followed_by_switch,
        cl_arg_tok_get_next_returns_null_when_followed_by_terminator,
        NULL
    };

    return cl_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
