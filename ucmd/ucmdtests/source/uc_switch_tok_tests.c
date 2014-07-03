#include <stddef.h>
#include "uc_switch_tok.h"
#include "uc_switch_tok_tests.h"
#include "uc_test.h"
#include "uc_tok.h"

static uc_test_err uc_switch_tok_get_next_returns_next_switch(uc_test_group *p) {
    uc_switch_tok *s = "-s1\0-s2\0-s3\0\n";
    UC_TEST_ASSERT(uc_tok_equals((uc_tok*)uc_switch_tok_get_next(s), "-s2"));
    s = uc_switch_tok_get_next(s);
    UC_TEST_ASSERT(uc_tok_equals((uc_tok*)uc_switch_tok_get_next(s), "-s3"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_switch_tok_get_next_returns_null(uc_test_group *p) {
    uc_switch_tok *s = "-switch\0\arg\0\a2\0\n";
    UC_TEST_ASSERT(NULL == uc_switch_tok_get_next(s));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_switch_tok_get_arg_is_initially_null(uc_test_group *p) {
    uc_switch_tok *swtch = "\0\n";
    UC_TEST_ASSERT(NULL == uc_switch_tok_get_arg(swtch));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_switch_tok_count_counts_switches(uc_test_group *p) {
    uc_switch_tok *s1 = (uc_switch_tok*)"-s1\0-s2\0-s3\0\n";
    UC_TEST_ASSERT(3 == uc_switch_tok_count(s1));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_switch_tok_contains_returns_true_if_contained(uc_test_group *p) {
    uc_switch_tok *s1 = (uc_switch_tok*)"-s1\0-s2\0-s3\0\n";
    UC_TEST_ASSERT(UC_TRUE == uc_switch_tok_contains(s1, "-s2"));
    UC_TEST_ASSERT(UC_TRUE == uc_switch_tok_contains(s1, "-s3"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_switch_tok_contains_returns_false_if_not_contained(uc_test_group *p) {
    uc_switch_tok *s1 = (uc_switch_tok*)"-s1\0-s2\0-s3\0\n";
    UC_TEST_ASSERT(UC_FALSE == uc_switch_tok_contains(s1, "-s4"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_switch_tok_find_finds_switch(uc_test_group *p) {
    uc_switch_tok *s = "-sw1\0sw1a1\0-sw2\0sw2a1\0sw2a2\0-sw3\0-sw4\0\n";
    UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)uc_switch_tok_find(s, "-sw3"), "-sw3"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_switch_tok_find_returns_null(uc_test_group *p) {
    uc_switch_tok *s = "-sw1\0sw1a1\0-sw2\0sw2a1\0sw2a2\0-sw3\0-sw4\0\n-sw5\0\n";
    UC_TEST_ASSERT(NULL == uc_switch_tok_find(s, "-sw5"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_switch_tok_get_arg_gets_first_arg(uc_test_group *p) {
    uc_switch_tok *s = "-s\0sarg\0arg2\0-s2\0\n";
    UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)uc_switch_tok_get_arg(s), "sarg"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_switch_tok_get_arg_returns_null(uc_test_group *p) {
    uc_switch_tok *s = "-s1\0-s2\0a1\0\n";
    UC_TEST_ASSERT(NULL == uc_switch_tok_get_arg(s));
    return UC_TEST_ERR_NONE;
}

uc_test_group *uc_switch_tok_tests_get_group(void) {
    static uc_test_group group;
    static uc_test_group_test_func *tests[] = {
        uc_switch_tok_get_next_returns_next_switch,
        uc_switch_tok_get_next_returns_null,
        uc_switch_tok_get_arg_is_initially_null,
        uc_switch_tok_count_counts_switches,
        uc_switch_tok_contains_returns_true_if_contained,
        uc_switch_tok_contains_returns_false_if_not_contained,
        uc_switch_tok_find_finds_switch,
        uc_switch_tok_find_returns_null,
        uc_switch_tok_get_arg_gets_first_arg,
        uc_switch_tok_get_arg_returns_null,
        NULL
    };

    return uc_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
