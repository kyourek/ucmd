#include "ucmdtests.h"

uc_TEST(ucArgTok_get_next_returns_next)
    ucArgTok *a1 = "a1\0a2\0\n";
    uc_TRUE(ucBool_true == ucTok_equals((ucTok*)ucArgTok_get_next(a1), "a2"));
uc_PASS

uc_TEST(ucArgTok_get_next_returns_null_when_followed_by_switch)
    ucArgTok *a = "a\0-s";
    uc_TRUE(NULL == ucArgTok_get_next(a));
uc_PASS

uc_TEST(ucArgTok_get_next_returns_null_when_followed_by_terminator)
    ucArgTok *a = "arg\0\n";
    uc_TRUE(NULL == ucArgTok_get_next(a));
uc_PASS

uc_TEST(ucArgTok_count_counts_arguments)
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    uc_TRUE(3 == ucArgTok_count(a1));
uc_PASS

uc_TEST(ucArgTok_find_finds_argument)
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    uc_TRUE(ucBool_true == ucTok_equals((ucTok*)ucArgTok_find(a1, "a2"), "a2"));
    uc_TRUE(ucBool_true == ucTok_equals((ucTok*)ucArgTok_find(a1, "a3"), "a3"));
uc_PASS

uc_TEST(ucArgTok_find_returns_null_if_no_argument_match)
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    uc_TRUE(NULL == ucArgTok_find(a1, "a4"));
uc_PASS

uc_TEST(ucArgTok_contains_returns_true_when_contained)
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    uc_TRUE(ucBool_true == ucArgTok_contains(a1, "a2"));
    uc_TRUE(ucBool_true == ucArgTok_contains(a1, "a3"));
uc_PASS

uc_TEST(ucArgTok_contains_returns_false_when_not_contained)
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    uc_TRUE(ucBool_false == ucArgTok_contains(a1, "a4"));
uc_PASS

uc_TEST_GROUP(ucArgTok, NULL,
    ucArgTok_get_next_returns_next,
    ucArgTok_count_counts_arguments,
    ucArgTok_find_finds_argument,
    ucArgTok_find_returns_null_if_no_argument_match,
    ucArgTok_contains_returns_true_when_contained,
    ucArgTok_contains_returns_false_when_not_contained,
    ucArgTok_get_next_returns_null_when_followed_by_switch,
    ucArgTok_get_next_returns_null_when_followed_by_terminator)
