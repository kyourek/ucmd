#include "cl_switch_tok.h"
#include "cl_switch_tok_tests.h"
#include "cl_tests.h"
#include "cl_tests_p.h"
#include "cl_tok.h"

static CL_TESTS_ERR cl_switch_tok_get_next_returns_next_switch(void) {
    cl_switch_tok *s = "-s1\0-s2\0-s3\0\n";
    CL_TESTS_ASSERT(cl_tok_equals((cl_tok*)cl_switch_tok_get_next(s), "-s2"));
    s = cl_switch_tok_get_next(s);
    CL_TESTS_ASSERT(cl_tok_equals((cl_tok*)cl_switch_tok_get_next(s), "-s3"));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_switch_tok_get_next_returns_null(void) {
    cl_switch_tok *s = "-switch\0\arg\0\a2\0\n";
    CL_TESTS_ASSERT(NULL == cl_switch_tok_get_next(s));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_switch_tok_get_arg_is_initially_null(void) {
    cl_switch_tok *swtch = "\0\n";
    CL_TESTS_ASSERT(NULL == cl_switch_tok_get_arg(swtch));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_switch_tok_count_counts_switches(void) {
    cl_switch_tok *s1 = (cl_switch_tok*)"-s1\0-s2\0-s3\0\n";
    CL_TESTS_ASSERT(3 == cl_switch_tok_count(s1));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_switch_tok_contains_returns_true_if_contained(void) {
    cl_switch_tok *s1 = (cl_switch_tok*)"-s1\0-s2\0-s3\0\n";
    CL_TESTS_ASSERT(CL_TRUE == cl_switch_tok_contains(s1, "-s2"));
    CL_TESTS_ASSERT(CL_TRUE == cl_switch_tok_contains(s1, "-s3"));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_switch_tok_contains_returns_false_if_not_contained(void) {
    cl_switch_tok *s1 = (cl_switch_tok*)"-s1\0-s2\0-s3\0\n";
    CL_TESTS_ASSERT(CL_FALSE == cl_switch_tok_contains(s1, "-s4"));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_switch_tok_find_finds_switch(void) {
    cl_switch_tok *s = "-sw1\0sw1a1\0-sw2\0sw2a1\0sw2a2\0-sw3\0-sw4\0\n";
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_switch_tok_find(s, "-sw3"), "-sw3"));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_switch_tok_find_returns_null(void) {
    cl_switch_tok *s = "-sw1\0sw1a1\0-sw2\0sw2a1\0sw2a2\0-sw3\0-sw4\0\n-sw5\0\n";
    CL_TESTS_ASSERT(NULL == cl_switch_tok_find(s, "-sw5"));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_switch_tok_get_arg_gets_first_arg(void) {
    cl_switch_tok *s = "-s\0sarg\0arg2\0-s2\0\n";
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_switch_tok_get_arg(s), "sarg"));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_switch_tok_get_arg_returns_null(void) {
    cl_switch_tok *s = "-s1\0-s2\0a1\0\n";
    CL_TESTS_ASSERT(NULL == cl_switch_tok_get_arg(s));
    return CL_TESTS_NO_ERR;
}

CL_TESTS_ERR cl_switch_tok_tests(void) {
    CL_TESTS_RUN(cl_switch_tok_get_next_returns_next_switch);
    CL_TESTS_RUN(cl_switch_tok_get_next_returns_null);
    CL_TESTS_RUN(cl_switch_tok_get_arg_is_initially_null);
    CL_TESTS_RUN(cl_switch_tok_count_counts_switches);
    CL_TESTS_RUN(cl_switch_tok_contains_returns_true_if_contained);
    CL_TESTS_RUN(cl_switch_tok_contains_returns_false_if_not_contained);
    CL_TESTS_RUN(cl_switch_tok_find_finds_switch);
    CL_TESTS_RUN(cl_switch_tok_find_returns_null);
    CL_TESTS_RUN(cl_switch_tok_get_arg_gets_first_arg);
    CL_TESTS_RUN(cl_switch_tok_get_arg_returns_null);
    return CL_TESTS_NO_ERR;
}
