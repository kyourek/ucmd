#include "cl_tests.h"
#include "cl_switch_tok.h"
#include "cl_switch_tok_tests.h"
#include "cl_tok.h"

static char *cl_switch_tok_get_next_returns_next_switch(void) {
    cl_switch_tok *s = "-s1\0-s2\0-s3\0\n";
    char *message = "cl_switch_tok_get_next did not return next switch.";
    cl_assert(message, cl_tok_equals((cl_tok*)cl_switch_tok_get_next(s), "-s2"));
    s = cl_switch_tok_get_next(s);
    cl_assert(message, cl_tok_equals((cl_tok*)cl_switch_tok_get_next(s), "-s3"));
    return 0;
}

static char *cl_switch_tok_get_next_returns_null(void) {
    cl_switch_tok *s = "-switch\0\arg\0\a2\0\n";
    cl_assert("cl_switch_tok_get_next did not return null.", NULL == cl_switch_tok_get_next(s));
    return 0;
}

static char *cl_switch_tok_get_arg_is_initially_null(void) {
    cl_switch_tok *swtch = "\0\n";
    cl_assert("cl_switch_tok_get_arg returned non-null initially.", NULL == cl_switch_tok_get_arg(swtch));
    return 0;
}

static char *cl_switch_tok_count_counts_switches(void) {
    cl_switch_tok *s1 = (cl_switch_tok*)"-s1\0-s2\0-s3\0\n";
    cl_assert("cl_switch_tok_count did not count switches.", 3 == cl_switch_tok_count(s1));
    return 0;
}

static char *cl_switch_tok_contains_returns_true_if_contained(void) {
    cl_switch_tok *s1 = (cl_switch_tok*)"-s1\0-s2\0-s3\0\n";
    cl_assert("cl_switch_tok_contains did not return true.", CL_TRUE == cl_switch_tok_contains(s1, "-s2"));
    cl_assert("cl_switch_tok_contains did not return true.", CL_TRUE == cl_switch_tok_contains(s1, "-s3"));
    return 0;
}

static char *cl_switch_tok_contains_returns_false_if_not_contained(void) {
    cl_switch_tok *s1 = (cl_switch_tok*)"-s1\0-s2\0-s3\0\n";
    cl_assert("cl_switch_tok_contains did not return true.", CL_FALSE == cl_switch_tok_contains(s1, "-s4"));
    return 0;
}

static char *cl_switch_tok_find_finds_switch(void) {
    cl_switch_tok *s = "-sw1\0sw1a1\0-sw2\0sw2a1\0sw2a2\0-sw3\0-sw4\0\n";
    cl_assert("cl_switch_tok_find did not find switch.", CL_TRUE == cl_tok_equals((cl_tok*)cl_switch_tok_find(s, "-sw3"), "-sw3"));
    return 0;
}

static char *cl_switch_tok_find_returns_null(void) {
    cl_switch_tok *s = "-sw1\0sw1a1\0-sw2\0sw2a1\0sw2a2\0-sw3\0-sw4\0\n-sw5\0\n";
    cl_assert("cl_switch_tok_find did not return null.", NULL == cl_switch_tok_find(s, "-sw5"));
    return 0;
}

static char *cl_switch_tok_get_arg_gets_first_arg(void) {
    cl_switch_tok *s = "-s\0sarg\0arg2\0-s2\0\n";
    cl_assert("cl_switch_tok_get_arg did not return first arg.", CL_TRUE == cl_tok_equals((cl_tok*)cl_switch_tok_get_arg(s), "sarg"));
    return 0;
}

static char *cl_switch_tok_get_arg_returns_null(void) {
    cl_switch_tok *s = "-s1\0-s2\0a1\0\n";
    cl_assert("cl_switch_tok_get_arg did not return null.", NULL == cl_switch_tok_get_arg(s));
    return 0;
}

char *cl_switch_tok_tests(void) {
    cl_run_test(cl_switch_tok_get_next_returns_next_switch);
    cl_run_test(cl_switch_tok_get_next_returns_null);
    cl_run_test(cl_switch_tok_get_arg_is_initially_null);
    cl_run_test(cl_switch_tok_count_counts_switches);
    cl_run_test(cl_switch_tok_contains_returns_true_if_contained);
    cl_run_test(cl_switch_tok_contains_returns_false_if_not_contained);
    cl_run_test(cl_switch_tok_find_finds_switch);
    cl_run_test(cl_switch_tok_find_returns_null);
    cl_run_test(cl_switch_tok_get_arg_gets_first_arg);
    cl_run_test(cl_switch_tok_get_arg_returns_null);
    return 0;
}
