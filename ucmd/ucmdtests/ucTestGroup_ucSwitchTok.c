#include <stddef.h>
#include "ucmdtests.h"

uc_TEST(ucSwitchTok_get_next_returns_next_switch)
    ucSwitchTok *s = "-s1\0-s2\0-s3\0\n";
    uc_TRUE(ucTok_equals((ucTok*)ucSwitchTok_get_next(s), "-s2"));
    s = ucSwitchTok_get_next(s);
    uc_TRUE(ucTok_equals((ucTok*)ucSwitchTok_get_next(s), "-s3"));
uc_PASS

uc_TEST(ucSwitchTok_get_next_returns_null)
    ucSwitchTok *s = "-switch\0\arg\0\a2\0\n";
    uc_TRUE(NULL == ucSwitchTok_get_next(s));
uc_PASS

uc_TEST(ucSwitchTok_get_arg_is_initially_null)
    ucSwitchTok *swtch = "\0\n";
    uc_TRUE(NULL == ucSwitchTok_get_arg(swtch));
uc_PASS

uc_TEST(ucSwitchTok_count_counts_switches)
    ucSwitchTok *s1 = (ucSwitchTok*)"-s1\0-s2\0-s3\0\n";
    uc_TRUE(3 == ucSwitchTok_count(s1));
uc_PASS

uc_TEST(ucSwitchTok_contains_returns_true_if_contained)
    ucSwitchTok *s1 = (ucSwitchTok*)"-s1\0-s2\0-s3\0\n";
    uc_TRUE(ucBool_true == ucSwitchTok_contains(s1, "-s2"));
    uc_TRUE(ucBool_true == ucSwitchTok_contains(s1, "-s3"));
uc_PASS

uc_TEST(ucSwitchTok_contains_returns_false_if_not_contained)
    ucSwitchTok *s1 = (ucSwitchTok*)"-s1\0-s2\0-s3\0\n";
    uc_TRUE(ucBool_false == ucSwitchTok_contains(s1, "-s4"));
uc_PASS

uc_TEST(ucSwitchTok_find_finds_switch)
    ucSwitchTok *s = "-sw1\0sw1a1\0-sw2\0sw2a1\0sw2a2\0-sw3\0-sw4\0\n";
    uc_TRUE(ucBool_true == ucTok_equals((ucTok*)ucSwitchTok_find(s, "-sw3"), "-sw3"));
uc_PASS

uc_TEST(ucSwitchTok_find_returns_null)
    ucSwitchTok *s = "-sw1\0sw1a1\0-sw2\0sw2a1\0sw2a2\0-sw3\0-sw4\0\n-sw5\0\n";
    uc_TRUE(NULL == ucSwitchTok_find(s, "-sw5"));
uc_PASS

uc_TEST(ucSwitchTok_get_arg_gets_first_arg)
    ucSwitchTok *s = "-s\0sarg\0arg2\0-s2\0\n";
    uc_TRUE(ucBool_true == ucTok_equals((ucTok*)ucSwitchTok_get_arg(s), "sarg"));
uc_PASS

uc_TEST(ucSwitchTok_get_arg_returns_null)
    ucSwitchTok *s = "-s1\0-s2\0a1\0\n";
    uc_TRUE(NULL == ucSwitchTok_get_arg(s));
uc_PASS

uc_TEST_GROUP(ucSwitchTok, NULL,
    ucSwitchTok_get_next_returns_next_switch,
    ucSwitchTok_get_next_returns_null,
    ucSwitchTok_get_arg_is_initially_null,
    ucSwitchTok_count_counts_switches,
    ucSwitchTok_contains_returns_true_if_contained,
    ucSwitchTok_contains_returns_false_if_not_contained,
    ucSwitchTok_find_finds_switch,
    ucSwitchTok_find_returns_null,
    ucSwitchTok_get_arg_gets_first_arg,
    ucSwitchTok_get_arg_returns_null)
