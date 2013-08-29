#include <string.h>
#include "cl_tests.h"
#include "cl_arg_tok.h"
#include "cl_arg_tok_tests.h"
#include "cl_tok.h"

static char *cl_arg_tok_get_next_returns_next(void) {
    cl_arg_tok *a1 = "a1\0a2\0\n";
    cl_assert("cl_arg_tok_get_next did not return the next arg.", CL_TRUE == cl_tok_equals((cl_tok*)cl_arg_tok_get_next(a1), "a2"));
    return 0;
}

static char *cl_arg_tok_get_next_returns_null_when_followed_by_switch(void) {
    cl_arg_tok *a = "a\0-s";
    cl_assert("cl_arg_tok_get_next did not return null.", NULL == cl_arg_tok_get_next(a));
    return 0;
}

static char *cl_arg_tok_get_next_returns_null_when_followed_by_terminator(void) {
    cl_arg_tok *a = "arg\0\n";
    cl_assert("cl_arg_tok_get_next did not return null.", NULL == cl_arg_tok_get_next(a));
    return 0;
}

static char *cl_arg_tok_count_counts_arguments(void) {
    cl_arg_tok *a1 = "a1\0a2\0a3\0\n";
    cl_assert("cl_arg_tok_count did not count the arguments.", 3 == cl_arg_tok_count(a1));
    return 0;
}

static char *cl_arg_tok_find_finds_argument(void) {
    cl_arg_tok *a1 = "a1\0a2\0a3\0\n";
    cl_assert("cl_arg_tok_find did not find a2.", CL_TRUE == cl_tok_equals((cl_tok*)cl_arg_tok_find(a1, "a2"), "a2"));
    cl_assert("cl_arg_tok_find did not find a3.", CL_TRUE == cl_tok_equals((cl_tok*)cl_arg_tok_find(a1, "a3"), "a3"));
    return 0;
}

static char *cl_arg_tok_find_returns_null_if_no_argument_match(void) {
    cl_arg_tok *a1 = "a1\0a2\0a3\0\n";
    cl_assert("cl_arg_tok_find did not return null.", NULL == cl_arg_tok_find(a1, "a4"));
    return 0;
}

static char *cl_arg_tok_contains_returns_true_when_contained(void) {
    cl_arg_tok *a1 = "a1\0a2\0a3\0\n";
    cl_assert("cl_arg_tok_contains did not return true.", CL_TRUE == cl_arg_tok_contains(a1, "a2"));
    cl_assert("cl_arg_tok_contains did not return true.", CL_TRUE == cl_arg_tok_contains(a1, "a3"));
    return 0;
}

static char *cl_arg_tok_contains_returns_false_when_not_contained(void) {
    cl_arg_tok *a1 = "a1\0a2\0a3\0\n";
    cl_assert("cl_arg_tok_contains did not return false.", CL_FALSE == cl_arg_tok_contains(a1, "a4"));
    return 0;
}

char *cl_arg_tok_tests(void) {
    cl_run_test(cl_arg_tok_get_next_returns_next);
    cl_run_test(cl_arg_tok_count_counts_arguments);
    cl_run_test(cl_arg_tok_find_finds_argument);
    cl_run_test(cl_arg_tok_find_returns_null_if_no_argument_match);
    cl_run_test(cl_arg_tok_contains_returns_true_when_contained);
    cl_run_test(cl_arg_tok_contains_returns_false_when_not_contained);
    cl_run_test(cl_arg_tok_get_next_returns_null_when_followed_by_switch);
    cl_run_test(cl_arg_tok_get_next_returns_null_when_followed_by_terminator);
    return 0;
}
