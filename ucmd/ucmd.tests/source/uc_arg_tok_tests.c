#include <string.h>
#include "uc_arg_tok.h"
#include "uc_arg_tok_tests.h"
#include "uc_test.h"
#include "uc_tok.h"

static uc_test_err uc_arg_tok_get_next_returns_next(uc_test_group *p) {
    uc_arg_tok *a1 = "a1\0a2\0\n";
    UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)uc_arg_tok_get_next(a1), "a2"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_arg_tok_get_next_returns_null_when_followed_by_switch(uc_test_group *p) {
    uc_arg_tok *a = "a\0-s";
    UC_TEST_ASSERT(NULL == uc_arg_tok_get_next(a));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_arg_tok_get_next_returns_null_when_followed_by_terminator(uc_test_group *p) {
    uc_arg_tok *a = "arg\0\n";
    UC_TEST_ASSERT(NULL == uc_arg_tok_get_next(a));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_arg_tok_count_counts_arguments(uc_test_group *p) {
    uc_arg_tok *a1 = "a1\0a2\0a3\0\n";
    UC_TEST_ASSERT(3 == uc_arg_tok_count(a1));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_arg_tok_find_finds_argument(uc_test_group *p) {
    uc_arg_tok *a1 = "a1\0a2\0a3\0\n";
    UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)uc_arg_tok_find(a1, "a2"), "a2"));
    UC_TEST_ASSERT(UC_TRUE == uc_tok_equals((uc_tok*)uc_arg_tok_find(a1, "a3"), "a3"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_arg_tok_find_returns_null_if_no_argument_match(uc_test_group *p) {
    uc_arg_tok *a1 = "a1\0a2\0a3\0\n";
    UC_TEST_ASSERT(NULL == uc_arg_tok_find(a1, "a4"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_arg_tok_contains_returns_true_when_contained(uc_test_group *p) {
    uc_arg_tok *a1 = "a1\0a2\0a3\0\n";
    UC_TEST_ASSERT(UC_TRUE == uc_arg_tok_contains(a1, "a2"));
    UC_TEST_ASSERT(UC_TRUE == uc_arg_tok_contains(a1, "a3"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_arg_tok_contains_returns_false_when_not_contained(uc_test_group *p) {
    uc_arg_tok *a1 = "a1\0a2\0a3\0\n";
    UC_TEST_ASSERT(UC_FALSE == uc_arg_tok_contains(a1, "a4"));
    return UC_TEST_ERR_NONE;
}

uc_test_group *uc_arg_tok_tests_get_group(void) {
    static uc_test_group group;
    static uc_test_group_test_func *tests[] = {
        uc_arg_tok_get_next_returns_next,
        uc_arg_tok_count_counts_arguments,
        uc_arg_tok_find_finds_argument,
        uc_arg_tok_find_returns_null_if_no_argument_match,
        uc_arg_tok_contains_returns_true_when_contained,
        uc_arg_tok_contains_returns_false_when_not_contained,
        uc_arg_tok_get_next_returns_null_when_followed_by_switch,
        uc_arg_tok_get_next_returns_null_when_followed_by_terminator,
        NULL
    };

    return uc_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
