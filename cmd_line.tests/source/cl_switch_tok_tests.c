#include <stddef.h>
#include "cl_switch_tok.h"
#include "cl_switch_tok_tests.h"
#include "cl_test.h"
#include "cl_tok.h"

static cl_test_err cl_switch_tok_get_next_returns_next_switch(cl_test_group *p) {
    cl_switch_tok *s = "-s1\0-s2\0-s3\0\n";
    CL_TEST_ASSERT(cl_tok_equals((cl_tok*)cl_switch_tok_get_next(s), "-s2"));
    s = cl_switch_tok_get_next(s);
    CL_TEST_ASSERT(cl_tok_equals((cl_tok*)cl_switch_tok_get_next(s), "-s3"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_switch_tok_get_next_returns_null(cl_test_group *p) {
    cl_switch_tok *s = "-switch\0\arg\0\a2\0\n";
    CL_TEST_ASSERT(NULL == cl_switch_tok_get_next(s));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_switch_tok_get_arg_is_initially_null(cl_test_group *p) {
    cl_switch_tok *swtch = "\0\n";
    CL_TEST_ASSERT(NULL == cl_switch_tok_get_arg(swtch));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_switch_tok_count_counts_switches(cl_test_group *p) {
    cl_switch_tok *s1 = (cl_switch_tok*)"-s1\0-s2\0-s3\0\n";
    CL_TEST_ASSERT(3 == cl_switch_tok_count(s1));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_switch_tok_contains_returns_true_if_contained(cl_test_group *p) {
    cl_switch_tok *s1 = (cl_switch_tok*)"-s1\0-s2\0-s3\0\n";
    CL_TEST_ASSERT(CL_TRUE == cl_switch_tok_contains(s1, "-s2"));
    CL_TEST_ASSERT(CL_TRUE == cl_switch_tok_contains(s1, "-s3"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_switch_tok_contains_returns_false_if_not_contained(cl_test_group *p) {
    cl_switch_tok *s1 = (cl_switch_tok*)"-s1\0-s2\0-s3\0\n";
    CL_TEST_ASSERT(CL_FALSE == cl_switch_tok_contains(s1, "-s4"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_switch_tok_find_finds_switch(cl_test_group *p) {
    cl_switch_tok *s = "-sw1\0sw1a1\0-sw2\0sw2a1\0sw2a2\0-sw3\0-sw4\0\n";
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_switch_tok_find(s, "-sw3"), "-sw3"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_switch_tok_find_returns_null(cl_test_group *p) {
    cl_switch_tok *s = "-sw1\0sw1a1\0-sw2\0sw2a1\0sw2a2\0-sw3\0-sw4\0\n-sw5\0\n";
    CL_TEST_ASSERT(NULL == cl_switch_tok_find(s, "-sw5"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_switch_tok_get_arg_gets_first_arg(cl_test_group *p) {
    cl_switch_tok *s = "-s\0sarg\0arg2\0-s2\0\n";
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_switch_tok_get_arg(s), "sarg"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_switch_tok_get_arg_returns_null(cl_test_group *p) {
    cl_switch_tok *s = "-s1\0-s2\0a1\0\n";
    CL_TEST_ASSERT(NULL == cl_switch_tok_get_arg(s));
    return CL_TEST_ERR_NONE;
}

cl_test_group *cl_switch_tok_tests_get_group(void) {
    static cl_test_group group;
    static cl_test_group_test_func *tests[] = {
        cl_switch_tok_get_next_returns_next_switch,
        cl_switch_tok_get_next_returns_null,
        cl_switch_tok_get_arg_is_initially_null,
        cl_switch_tok_count_counts_switches,
        cl_switch_tok_contains_returns_true_if_contained,
        cl_switch_tok_contains_returns_false_if_not_contained,
        cl_switch_tok_find_finds_switch,
        cl_switch_tok_find_returns_null,
        cl_switch_tok_get_arg_gets_first_arg,
        cl_switch_tok_get_arg_returns_null,
        NULL
    };

    return cl_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
