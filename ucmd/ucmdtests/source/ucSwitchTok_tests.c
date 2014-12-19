#include <stddef.h>
#include "ucSwitchTok.h"
#include "ucSwitchTok_tests.h"
#include "ucTest.h"
#include "ucTok.h"

static ucTestErr ucSwitchTok_get_next_returns_next_switch(ucTestGroup *p) {
    ucSwitchTok *s = "-s1\0-s2\0-s3\0\n";
    ucTest_ASSERT(ucTok_equals((ucTok*)ucSwitchTok_get_next(s), "-s2"));
    s = ucSwitchTok_get_next(s);
    ucTest_ASSERT(ucTok_equals((ucTok*)ucSwitchTok_get_next(s), "-s3"));
    return ucTestErr_NONE;
}

static ucTestErr ucSwitchTok_get_next_returns_null(ucTestGroup *p) {
    ucSwitchTok *s = "-switch\0\arg\0\a2\0\n";
    ucTest_ASSERT(NULL == ucSwitchTok_get_next(s));
    return ucTestErr_NONE;
}

static ucTestErr ucSwitchTok_get_arg_is_initially_null(ucTestGroup *p) {
    ucSwitchTok *swtch = "\0\n";
    ucTest_ASSERT(NULL == ucSwitchTok_get_arg(swtch));
    return ucTestErr_NONE;
}

static ucTestErr ucSwitchTok_count_counts_switches(ucTestGroup *p) {
    ucSwitchTok *s1 = (ucSwitchTok*)"-s1\0-s2\0-s3\0\n";
    ucTest_ASSERT(3 == ucSwitchTok_count(s1));
    return ucTestErr_NONE;
}

static ucTestErr ucSwitchTok_contains_returns_true_if_contained(ucTestGroup *p) {
    ucSwitchTok *s1 = (ucSwitchTok*)"-s1\0-s2\0-s3\0\n";
    ucTest_ASSERT(ucBool_true == ucSwitchTok_contains(s1, "-s2"));
    ucTest_ASSERT(ucBool_true == ucSwitchTok_contains(s1, "-s3"));
    return ucTestErr_NONE;
}

static ucTestErr ucSwitchTok_contains_returns_false_if_not_contained(ucTestGroup *p) {
    ucSwitchTok *s1 = (ucSwitchTok*)"-s1\0-s2\0-s3\0\n";
    ucTest_ASSERT(ucBool_false == ucSwitchTok_contains(s1, "-s4"));
    return ucTestErr_NONE;
}

static ucTestErr ucSwitchTok_find_finds_switch(ucTestGroup *p) {
    ucSwitchTok *s = "-sw1\0sw1a1\0-sw2\0sw2a1\0sw2a2\0-sw3\0-sw4\0\n";
    ucTest_ASSERT(ucBool_true == ucTok_equals((ucTok*)ucSwitchTok_find(s, "-sw3"), "-sw3"));
    return ucTestErr_NONE;
}

static ucTestErr ucSwitchTok_find_returns_null(ucTestGroup *p) {
    ucSwitchTok *s = "-sw1\0sw1a1\0-sw2\0sw2a1\0sw2a2\0-sw3\0-sw4\0\n-sw5\0\n";
    ucTest_ASSERT(NULL == ucSwitchTok_find(s, "-sw5"));
    return ucTestErr_NONE;
}

static ucTestErr ucSwitchTok_get_arg_gets_first_arg(ucTestGroup *p) {
    ucSwitchTok *s = "-s\0sarg\0arg2\0-s2\0\n";
    ucTest_ASSERT(ucBool_true == ucTok_equals((ucTok*)ucSwitchTok_get_arg(s), "sarg"));
    return ucTestErr_NONE;
}

static ucTestErr ucSwitchTok_get_arg_returns_null(ucTestGroup *p) {
    ucSwitchTok *s = "-s1\0-s2\0a1\0\n";
    ucTest_ASSERT(NULL == ucSwitchTok_get_arg(s));
    return ucTestErr_NONE;
}

ucTestGroup *ucSwitchTok_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_test_func *tests[] = {
        ucSwitchTok_get_next_returns_next_switch,
        ucSwitchTok_get_next_returns_null,
        ucSwitchTok_get_arg_is_initially_null,
        ucSwitchTok_count_counts_switches,
        ucSwitchTok_contains_returns_true_if_contained,
        ucSwitchTok_contains_returns_false_if_not_contained,
        ucSwitchTok_find_finds_switch,
        ucSwitchTok_find_returns_null,
        ucSwitchTok_get_arg_gets_first_arg,
        ucSwitchTok_get_arg_returns_null,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
