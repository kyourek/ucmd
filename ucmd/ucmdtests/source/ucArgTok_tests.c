#include <string.h>
#include "ucArgTok.h"
#include "ucTest.h"
#include "ucTok.h"

static ucTestErr ucArgTok_get_next_returns_next(ucTestGroup *p) {
    ucArgTok *a1 = "a1\0a2\0\n";
    ucTest_ASSERT(ucBool_TRUE == ucTok_equals((ucTok*)ucArgTok_get_next(a1), "a2"));
    return ucTestErr_NONE;
}

static ucTestErr ucArgTok_get_next_returns_null_when_followed_by_switch(ucTestGroup *p) {
    ucArgTok *a = "a\0-s";
    ucTest_ASSERT(NULL == ucArgTok_get_next(a));
    return ucTestErr_NONE;
}

static ucTestErr ucArgTok_get_next_returns_null_when_followed_by_terminator(ucTestGroup *p) {
    ucArgTok *a = "arg\0\n";
    ucTest_ASSERT(NULL == ucArgTok_get_next(a));
    return ucTestErr_NONE;
}

static ucTestErr ucArgTok_count_counts_arguments(ucTestGroup *p) {
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    ucTest_ASSERT(3 == ucArgTok_count(a1));
    return ucTestErr_NONE;
}

static ucTestErr ucArgTok_find_finds_argument(ucTestGroup *p) {
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    ucTest_ASSERT(ucBool_TRUE == ucTok_equals((ucTok*)ucArgTok_find(a1, "a2"), "a2"));
    ucTest_ASSERT(ucBool_TRUE == ucTok_equals((ucTok*)ucArgTok_find(a1, "a3"), "a3"));
    return ucTestErr_NONE;
}

static ucTestErr ucArgTok_find_returns_null_if_no_argument_match(ucTestGroup *p) {
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    ucTest_ASSERT(NULL == ucArgTok_find(a1, "a4"));
    return ucTestErr_NONE;
}

static ucTestErr ucArgTok_contains_returns_true_when_contained(ucTestGroup *p) {
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    ucTest_ASSERT(ucBool_TRUE == ucArgTok_contains(a1, "a2"));
    ucTest_ASSERT(ucBool_TRUE == ucArgTok_contains(a1, "a3"));
    return ucTestErr_NONE;
}

static ucTestErr ucArgTok_contains_returns_false_when_not_contained(ucTestGroup *p) {
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    ucTest_ASSERT(ucBool_FALSE == ucArgTok_contains(a1, "a4"));
    return ucTestErr_NONE;
}

ucTestGroup *ucArgTok_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_TestFunc *tests[] = {
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

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
