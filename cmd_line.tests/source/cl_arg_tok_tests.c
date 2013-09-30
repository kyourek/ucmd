#include <string.h>
#include "cl_arg_tok.h"
#include "cl_arg_tok_tests.h"
#include "cl_tests_p.h"
#include "cl_tok.h"

static CL_TESTS_ERR cl_arg_tok_get_next_returns_next(void) {
    cl_arg_tok *a1 = "a1\0a2\0\n";
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_arg_tok_get_next(a1), "a2"));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_arg_tok_get_next_returns_null_when_followed_by_switch(void) {
    cl_arg_tok *a = "a\0-s";
    CL_TESTS_ASSERT(NULL == cl_arg_tok_get_next(a));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_arg_tok_get_next_returns_null_when_followed_by_terminator(void) {
    cl_arg_tok *a = "arg\0\n";
    CL_TESTS_ASSERT(NULL == cl_arg_tok_get_next(a));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_arg_tok_count_counts_arguments(void) {
    cl_arg_tok *a1 = "a1\0a2\0a3\0\n";
    CL_TESTS_ASSERT(3 == cl_arg_tok_count(a1));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_arg_tok_find_finds_argument(void) {
    cl_arg_tok *a1 = "a1\0a2\0a3\0\n";
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_arg_tok_find(a1, "a2"), "a2"));
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_arg_tok_find(a1, "a3"), "a3"));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_arg_tok_find_returns_null_if_no_argument_match(void) {
    cl_arg_tok *a1 = "a1\0a2\0a3\0\n";
    CL_TESTS_ASSERT(NULL == cl_arg_tok_find(a1, "a4"));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_arg_tok_contains_returns_true_when_contained(void) {
    cl_arg_tok *a1 = "a1\0a2\0a3\0\n";
    CL_TESTS_ASSERT(CL_TRUE == cl_arg_tok_contains(a1, "a2"));
    CL_TESTS_ASSERT(CL_TRUE == cl_arg_tok_contains(a1, "a3"));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_arg_tok_contains_returns_false_when_not_contained(void) {
    cl_arg_tok *a1 = "a1\0a2\0a3\0\n";
    CL_TESTS_ASSERT(CL_FALSE == cl_arg_tok_contains(a1, "a4"));
    return CL_TESTS_NO_ERR;
}

CL_TESTS_ERR cl_arg_tok_tests(void) {
    CL_TESTS_RUN(cl_arg_tok_get_next_returns_next);
    CL_TESTS_RUN(cl_arg_tok_count_counts_arguments);
    CL_TESTS_RUN(cl_arg_tok_find_finds_argument);
    CL_TESTS_RUN(cl_arg_tok_find_returns_null_if_no_argument_match);
    CL_TESTS_RUN(cl_arg_tok_contains_returns_true_when_contained);
    CL_TESTS_RUN(cl_arg_tok_contains_returns_false_when_not_contained);
    CL_TESTS_RUN(cl_arg_tok_get_next_returns_null_when_followed_by_switch);
    CL_TESTS_RUN(cl_arg_tok_get_next_returns_null_when_followed_by_terminator);
    return CL_TESTS_NO_ERR;
}
