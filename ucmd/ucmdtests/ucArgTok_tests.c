#include "ucmdtests.h"

static ucTestErr ucArgTok_get_next__returns_next(ucTestGroup *p) {
    ucArgTok *a1 = "a1\0a2\0\n";
    ucTRUE(ucBool_true == ucTok_equals((ucTok*)ucArgTok_get_next(a1), "a2"));
    ucPASS();
}

static ucTestErr ucArgTok_get_next__returns_null_when_followed_by_switch(ucTestGroup *p) {
    ucArgTok *a = "a\0-s";
    ucTRUE(NULL == ucArgTok_get_next(a));
    ucPASS();
}

static ucTestErr ucArgTok_get_next__returns_null_when_followed_by_terminator(ucTestGroup *p) {
    ucArgTok *a = "arg\0\n";
    ucTRUE(NULL == ucArgTok_get_next(a));
    ucPASS();
}

static ucTestErr ucArgTok_count__counts_arguments(ucTestGroup *p) {
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    ucTRUE(3 == ucArgTok_count(a1));
    ucPASS();
}

static ucTestErr ucArgTok_find__finds_argument(ucTestGroup *p) {
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    ucTRUE(ucBool_true == ucTok_equals((ucTok*)ucArgTok_find(a1, "a2"), "a2"));
    ucTRUE(ucBool_true == ucTok_equals((ucTok*)ucArgTok_find(a1, "a3"), "a3"));
    ucPASS();
}

static ucTestErr ucArgTok_find__returns_null_if_no_argument_match(ucTestGroup *p) {
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    ucTRUE(NULL == ucArgTok_find(a1, "a4"));
    ucPASS();
}

static ucTestErr ucArgTok_contains__returns_true_when_contained(ucTestGroup *p) {
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    ucTRUE(ucBool_true == ucArgTok_contains(a1, "a2"));
    ucTRUE(ucBool_true == ucArgTok_contains(a1, "a3"));
    ucPASS();
}

static ucTestErr ucArgTok_contains__returns_false_when_not_contained(ucTestGroup *p) {
    ucArgTok *a1 = "a1\0a2\0a3\0\n";
    ucTRUE(ucBool_false == ucArgTok_contains(a1, "a4"));
    ucPASS();
}

ucTestGroup *ucArgTok_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_TestFunc *tests[] = {
        ucArgTok_get_next__returns_next,
        ucArgTok_count__counts_arguments,
        ucArgTok_find__finds_argument,
        ucArgTok_find__returns_null_if_no_argument_match,
        ucArgTok_contains__returns_true_when_contained,
        ucArgTok_contains__returns_false_when_not_contained,
        ucArgTok_get_next__returns_null_when_followed_by_switch,
        ucArgTok_get_next__returns_null_when_followed_by_terminator,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
