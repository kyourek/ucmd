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

uc_TEST(ucArgTok_get_index_returns_argument_by_index_test, const char *arguments, int index, const char *argument)
    uc_TRUE(uc_STR_EQ(argument, ucArgTok_get_index(arguments, index)));
uc_PASS
uc_CASE(ucArgTok_get_index_returns_argument_by_index_test, 1st_argument, "a1\0a2\0me!\0\n", 0, "a1")
uc_CASE(ucArgTok_get_index_returns_argument_by_index_test, 3rd_argument, "a1\0a2\0me!\0\n", 2, "me!")

uc_TEST(ucArgTok_get_index_returns_null_if_index_does_not_exist_test, const char *arguments, int index)
    uc_TRUE(NULL == ucArgTok_get_index(arguments, index));
uc_PASS
uc_CASE(ucArgTok_get_index_returns_null_if_index_does_not_exist_test, negative_index, "a1\0a2\0me!\0\n", -1)
uc_CASE(ucArgTok_get_index_returns_null_if_index_does_not_exist_test, out_of_range, "a1\0a2\0me!\0\n", 3)

uc_TEST_GROUP(ucArgTok, NULL,
    ucArgTok_get_index_returns_argument_by_index_test_1st_argument,
    ucArgTok_get_index_returns_argument_by_index_test_3rd_argument,
    ucArgTok_get_index_returns_null_if_index_does_not_exist_test_negative_index,
    ucArgTok_get_index_returns_null_if_index_does_not_exist_test_out_of_range,
    ucArgTok_get_next_returns_next,
    ucArgTok_count_counts_arguments,
    ucArgTok_find_finds_argument,
    ucArgTok_find_returns_null_if_no_argument_match,
    ucArgTok_contains_returns_true_when_contained,
    ucArgTok_contains_returns_false_when_not_contained,
    ucArgTok_get_next_returns_null_when_followed_by_switch,
    ucArgTok_get_next_returns_null_when_followed_by_terminator)
