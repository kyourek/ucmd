#include <string.h>
#include "ucArgTok.h"
#include "ucArgTok_tests.h"
#include "uc_test.h"
#include "uc_tok.h"

static uc_test_err ucArgTok_get_next_returns_next(uc_test_group *p) {
    ucArgTok *a1 = "a1\0a2\0\n";
    UC_TEST_ASSERT(ucBool_true == uc_tok_equals((uc_tok*)ucArgTok_get_next(a1), "a2"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgTok_get_next_returns_null_when_followed_by_switch(uc_test_group *p) {
    ucArgTok *a = "a\0-s";
    UC_TEST_ASSERT(NULL == ucArgTok_get_next(a));
    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgTok_get_next_returns_null_when_followed_by_terminator(uc_test_group *p) {
    ucArgTok *a = "arg\0\n";
    UC_TEST_ASSERT(NULL == ucArgTok_get_next(a));
    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgTok_count_counts_arguments(uc_test_group *p) {
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    UC_TEST_ASSERT(3 == ucArgTok_count(a1));
    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgTok_find_finds_argument(uc_test_group *p) {
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    UC_TEST_ASSERT(ucBool_true == uc_tok_equals((uc_tok*)ucArgTok_find(a1, "a2"), "a2"));
    UC_TEST_ASSERT(ucBool_true == uc_tok_equals((uc_tok*)ucArgTok_find(a1, "a3"), "a3"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgTok_find_returns_null_if_no_argument_match(uc_test_group *p) {
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    UC_TEST_ASSERT(NULL == ucArgTok_find(a1, "a4"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgTok_contains_returns_true_when_contained(uc_test_group *p) {
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    UC_TEST_ASSERT(ucBool_true == ucArgTok_contains(a1, "a2"));
    UC_TEST_ASSERT(ucBool_true == ucArgTok_contains(a1, "a3"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err ucArgTok_contains_returns_false_when_not_contained(uc_test_group *p) {
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    UC_TEST_ASSERT(ucBool_false == ucArgTok_contains(a1, "a4"));
    return UC_TEST_ERR_NONE;
}

uc_test_group *ucArgTok_tests_get_group(void) {
    static uc_test_group group;
    static uc_test_group_test_func *tests[] = {
        ucArgTok_get_next_returns_next,
        ucArgTok_count_counts_arguments,
        ucArgTok_find_finds_argument,
        ucArgTok_find_returns_null_if_no_argument_match,
        ucArgTok_contains_returns_true_when_contained,
        ucArgTok_contains_returns_false_when_not_contained,
        ucArgTok_get_next_returns_null_when_followed_by_switch,
        ucArgTok_get_next_returns_null_when_followed_by_terminator,
        NULL
    };

    return uc_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
