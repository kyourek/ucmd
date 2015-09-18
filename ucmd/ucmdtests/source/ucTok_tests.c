#include <stdlib.h>
#include <string.h>
#include "ucTest.h"

static ucTestErr ucTok_get_length_returns_string_length(ucTestGroup *p) {
    char s[] = { 'h', 'e', 'l', 'l', 'o', '\0' };
    ucTok *ptr = s;
    ucTest_ASSERT(ucTok_get_length(ptr) == strlen(s));
    return ucTestErr_NONE;
}

static ucTestErr ucTok_get_length_returns_zero_for_empty_token(ucTestGroup *p) {
    ucTest_ASSERT(0 == ucTok_get_length((ucTok*)""));
    ucTest_ASSERT(0 == ucTok_get_length((ucTok*)NULL));
    return ucTestErr_NONE;
}

static ucTestErr ucTok_get_length_ignores_chars_trailing_separator(ucTestGroup *p) {
    char s[] = { '1', '2', '3', '\0', '4', '5', '\0' };
    ucTok *ptr = s;
    ucTest_ASSERT(3 == ucTok_get_length(ptr));
    return ucTestErr_NONE;
}

static ucTestErr ucTok_get_value_returns_string_value(ucTestGroup *p) {
    char s[] = { 'g', 'o', 'o', 'd', 'b', 'y', 'e', '\0' };
    ucTok *ptr = s;
    ucTest_ASSERT(ucTok_get_value(ptr));
    return ucTestErr_NONE;
}

static ucTestErr ucTok_get_value_ignores_chars_after_separator(ucTestGroup *p) {
    char s[] = { 'g', 'o', 'o', 'd', '\0', 'b', 'y', 'e', '\0' };
    ucTok *ptr = s;
    ucTest_ASSERT(ucTok_get_value(ptr));
    return ucTestErr_NONE;
}

static ucTestErr ucTok_equals_returns_true_if_strings_are_equal(ucTestGroup *p) {
    char *value1 = "value";
    char value2[] = { 'v', 'a', 'l', 'u', 'e', '\0', 'n', 'a', '\0' };
    ucTok *ptr = "value";
    ucTest_ASSERT(ucBool_TRUE == ucTok_equals(ptr, value1));
    ucTest_ASSERT(ucBool_TRUE == ucTok_equals(ptr, value2));
    return ucTestErr_NONE;
}

static ucTestErr ucTok_equals_returns_true_for_nulls(ucTestGroup *p) {
    char  *value = NULL;
    ucTok *ptr = NULL;
    ucTest_ASSERT(ucBool_TRUE == ucTok_equals(ptr, value));
    return ucTestErr_NONE;
}

static ucTestErr ucTok_equals_returns_false_if_strings_are_different(ucTestGroup *p) {
    char *value1 = "value";
    char value2[] = { 'v', 'a', 'l', 'u', '\0', 'u', '\0' };
    ucTok *ptr = "valuu";
    ucTest_ASSERT(ucBool_FALSE == ucTok_equals(ptr, value1));
    ucTest_ASSERT(ucBool_FALSE == ucTok_equals(ptr, value2));
    return ucTestErr_NONE;
}

static ucTestErr ucTok_equals_returns_false_if_either_arg_is_null(ucTestGroup *p) {
    char *value = "value";
    ucTok *ptr = NULL;
    ucTest_ASSERT(ucBool_FALSE == ucTok_equals(ptr, value));

    ptr = value;
    value = NULL;
    ucTest_ASSERT(ucBool_FALSE == ucTok_equals(ptr, value));

    return ucTestErr_NONE;
}

static ucTestErr ucTok_is_numeric_returns_true_for_numbers(ucTestGroup *p) {
    char n1[] = { '4', '8', '.', '9', '1', '\0', 'n', 'a', '\0' };
    ucTest_ASSERT(ucBool_TRUE == ucTok_is_numeric((ucTok*)"1"));
    ucTest_ASSERT(ucBool_TRUE == ucTok_is_numeric((ucTok*)"1.2"));
    ucTest_ASSERT(ucBool_TRUE == ucTok_is_numeric((ucTok*)"87"));
    ucTest_ASSERT(ucBool_TRUE == ucTok_is_numeric((ucTok*)"-1"));
    ucTest_ASSERT(ucBool_TRUE == ucTok_is_numeric((ucTok*)n1));
    ucTest_ASSERT(ucBool_TRUE == ucTok_is_numeric((ucTok*)"-543.234610"));
    return ucTestErr_NONE;
}

static ucTestErr ucTok_is_numeric_returns_false_for_non_numbers(ucTestGroup *p) {
    char c1[] = { '4', '8', '.', '9', '1', 'n', 'a', '\0' };
    ucTest_ASSERT(ucBool_FALSE == ucTok_is_numeric((ucTok*)"a"));
    ucTest_ASSERT(ucBool_FALSE == ucTok_is_numeric((ucTok*)"1234_"));
    ucTest_ASSERT(ucBool_FALSE == ucTok_is_numeric((ucTok*)"1_234"));
    ucTest_ASSERT(ucBool_FALSE == ucTok_is_numeric((ucTok*)c1));
    ucTest_ASSERT(ucBool_FALSE == ucTok_is_numeric((ucTok*)NULL));
    return ucTestErr_NONE;
}

static ucTestErr ucTok_is_switch_returns_true_for_switches(ucTestGroup *p) {
    char s1[] = { '-', 's', '1', '\0', 'n', 'a', '\0' };
    ucTest_ASSERT(ucBool_TRUE == ucTok_is_switch((ucTok*)"-switch"));
    ucTest_ASSERT(ucBool_TRUE == ucTok_is_switch((ucTok*)"--double"));
    ucTest_ASSERT(ucBool_TRUE == ucTok_is_switch((ucTok*)"-em-bed"));
    ucTest_ASSERT(ucBool_TRUE == ucTok_is_switch((ucTok*)s1));
    ucTest_ASSERT(ucBool_TRUE == ucTok_is_switch((ucTok*)"-1.23.4"));
    return ucTestErr_NONE;
}

static ucTestErr ucTok_is_switch_returns_false_for_non_switches(ucTestGroup *p) {
    char a1[] = { 'a', 'r', 'g', '-', 'd', 'a', 's', 'h', '\0' };
    ucTest_ASSERT(ucBool_FALSE == ucTok_is_switch((ucTok*)"arg"));
    ucTest_ASSERT(ucBool_FALSE == ucTok_is_switch((ucTok*)"e-mbed"));
    ucTest_ASSERT(ucBool_FALSE == ucTok_is_switch((ucTok*)"-1"));
    ucTest_ASSERT(ucBool_FALSE == ucTok_is_switch((ucTok*)"-1.234"));
    ucTest_ASSERT(ucBool_FALSE == ucTok_is_switch((ucTok*)a1));
    ucTest_ASSERT(ucBool_FALSE == ucTok_is_switch((ucTok*)NULL));
    return ucTestErr_NONE;
}

static ucTestErr ucTok_get_next_returns_next_token(ucTestGroup *p) {
    ucTest_ASSERT(ucTok_equals(ucTok_get_next((ucTok*)"t1\0t2"), "t2"));
    ucTest_ASSERT(ucTok_equals(ucTok_get_next((ucTok*)"t1\0-s1\0t2"), "-s1"));
    ucTest_ASSERT(ucTok_equals(ucTok_get_next((ucTok*)"a1\0\na2"), NULL));
    return ucTestErr_NONE;
}

static ucTestErr ucTok_count_counts_tokens(ucTestGroup *p) {
    ucTest_ASSERT(0 == ucTok_count((ucTok*)NULL));
    ucTest_ASSERT(0 == ucTok_count((ucTok*)"\n"));
    ucTest_ASSERT(0 == ucTok_count((ucTok*)"\0\0\0\n"));
    ucTest_ASSERT(1 == ucTok_count((ucTok*)"a1\0\n"));
    ucTest_ASSERT(1 == ucTok_count((ucTok*)"-s1\0\n"));
    ucTest_ASSERT(3 == ucTok_count((ucTok*)"tok en\0a1\0-s1\0\nNA"));
    return ucTestErr_NONE;
}

static ucTestErr ucTok_parse_numeric__test(ucTestGroup *p, const char *s, ucBool parsed) {
    double expected, actual;
    ucTest_ASSERT(parsed == ucTok_parse_numeric((ucTok*)s, &actual));
    if (parsed) {
        expected = atof(s);
        ucTest_ASSERT(actual == expected);
    }
    return ucTestErr_NONE;
}

static ucTestErr ucTok_parse_numeric__does_not_parse_null(ucTestGroup *p) {
    return ucTok_parse_numeric__test(p, NULL, ucBool_FALSE);
}

static ucTestErr ucTok_parse_numeric__does_not_parse_string(ucTestGroup *p) {
    return ucTok_parse_numeric__test(p, "not numeric", ucBool_FALSE);
}

static ucTestErr ucTok_parse_numeric__does_not_parse_invalid_number(ucTestGroup *p) {
    return ucTok_parse_numeric__test(p, "1.34.0", ucBool_FALSE);
}

static ucTestErr ucTok_parse_numeric__parses_number(ucTestGroup *p) {
    return ucTok_parse_numeric__test(p, "1.34", ucBool_TRUE);
}

static ucTestErr ucTok_parse_numeric__parses_negative_number(ucTestGroup *p) {
    return ucTok_parse_numeric__test(p, "-349.104", ucBool_TRUE);
}

static ucTestErr ucTok_parse_numeric__parses_zero(ucTestGroup *p) {
    ucTest_ASSERT(ucTestErr_NONE == ucTok_parse_numeric__test(p, "0", ucBool_TRUE));
    ucTest_ASSERT(ucTestErr_NONE == ucTok_parse_numeric__test(p, "0.000", ucBool_TRUE));
    return ucTestErr_NONE;
}

static ucTestErr ucTok_is_boolean__test(ucTestGroup *p, const char *s, ucBool is_boolean) {
    ucTest_ASSERT(is_boolean == ucTok_is_boolean((ucTok*)s));
    return ucTestErr_NONE;
}

static ucTestErr ucTok_is_boolean__returns_true_for_1(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "1", ucBool_TRUE);
}

static ucTestErr ucTok_is_boolean__returns_true_for_on(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "on", ucBool_TRUE);
}

static ucTestErr ucTok_is_boolean__returns_true_for_yes(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "yes", ucBool_TRUE);
}

static ucTestErr ucTok_is_boolean__returns_true_for_true(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "true", ucBool_TRUE);
}

static ucTestErr ucTok_is_boolean__returns_true_for_0(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "0", ucBool_TRUE);
}

static ucTestErr ucTok_is_boolean__returns_true_for_off(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "off", ucBool_TRUE);
}

static ucTestErr ucTok_is_boolean__returns_true_for_no(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "no", ucBool_TRUE);
}

static ucTestErr ucTok_is_boolean__returns_true_for_false(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "false", ucBool_TRUE);
}

static ucTestErr ucTok_is_boolean__returns_false_for_numeric(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "1.0", ucBool_FALSE);
}

static ucTestErr ucTok_is_boolean__returns_false_for_null(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, NULL, ucBool_FALSE);
}

static ucTestErr ucTok_is_boolean__returns_false_for_string(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "not a bool", ucBool_FALSE);
}

static ucTestErr ucTok_parse_boolean__test(ucTestGroup *p, const char *s, ucBool is_boolean, ucBool expected) {
    ucBool actual;
    ucTest_ASSERT(is_boolean == ucTok_parse_boolean((ucTok*)s, &actual));
    if (is_boolean) {
        ucTest_ASSERT(expected == actual);
    }
    return ucTestErr_NONE;
}

static ucTestErr ucTok_parse_boolean__parses_1(ucTestGroup *p) {
    return ucTok_parse_boolean__test(p, "1", ucBool_TRUE, ucBool_TRUE);
}

static ucTestErr ucTok_parse_boolean__parses_on(ucTestGroup *p) {
    return ucTok_parse_boolean__test(p, "on", ucBool_TRUE, ucBool_TRUE);
}

static ucTestErr ucTok_parse_boolean__parses_yes(ucTestGroup *p) {
    return ucTok_parse_boolean__test(p, "yes", ucBool_TRUE, ucBool_TRUE);
}

static ucTestErr ucTok_parse_boolean__parses_true(ucTestGroup *p) {
    return ucTok_parse_boolean__test(p, "true", ucBool_TRUE, ucBool_TRUE);
}

static ucTestErr ucTok_parse_boolean__parses_0(ucTestGroup *p) {
    return ucTok_parse_boolean__test(p, "0", ucBool_TRUE, ucBool_FALSE);
}

static ucTestErr ucTok_parse_boolean__parses_off(ucTestGroup *p) {
    return ucTok_parse_boolean__test(p, "off", ucBool_TRUE, ucBool_FALSE);
}

static ucTestErr ucTok_parse_boolean__parses_no(ucTestGroup *p) {
    return ucTok_parse_boolean__test(p, "no", ucBool_TRUE, ucBool_FALSE);
}

static ucTestErr ucTok_parse_boolean__parses_false(ucTestGroup *p) {
    return ucTok_parse_boolean__test(p, "false", ucBool_TRUE, ucBool_FALSE);
}

static ucTestErr ucTok_parse_boolean__does_not_parse_invalid_string(ucTestGroup *p) {
    return ucTok_parse_boolean__test(p, "invalid", ucBool_FALSE, ucBool_FALSE);    
}

static ucTestErr ucTok_parse_boolean__does_not_parse_null(ucTestGroup *p) {
    return ucTok_parse_boolean__test(p, NULL, ucBool_FALSE, ucBool_FALSE);
}

static ucTestErr ucTok_parse_boolean__does_not_parse_numeric(ucTestGroup *p) {
    return ucTok_parse_boolean__test(p, "0.0", ucBool_FALSE, ucBool_FALSE);
}

ucTestGroup *ucTok_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_TestFunc *tests[] = {
        ucTok_get_length_returns_string_length,
        ucTok_get_length_returns_zero_for_empty_token,
        ucTok_get_length_ignores_chars_trailing_separator,
        ucTok_get_value_returns_string_value,
        ucTok_get_value_ignores_chars_after_separator,
        ucTok_equals_returns_true_if_strings_are_equal,
        ucTok_equals_returns_true_for_nulls,
        ucTok_equals_returns_false_if_strings_are_different,
        ucTok_equals_returns_false_if_either_arg_is_null,
        ucTok_is_numeric_returns_true_for_numbers,
        ucTok_is_numeric_returns_false_for_non_numbers,
        ucTok_is_numeric_returns_false_for_non_numbers,
        ucTok_is_switch_returns_true_for_switches,
        ucTok_is_switch_returns_false_for_non_switches,
        ucTok_get_next_returns_next_token,
        ucTok_count_counts_tokens,
        ucTok_parse_numeric__does_not_parse_null,
        ucTok_parse_numeric__does_not_parse_string,
        ucTok_parse_numeric__does_not_parse_invalid_number,
        ucTok_parse_numeric__parses_number,
        ucTok_parse_numeric__parses_negative_number,
        ucTok_parse_numeric__parses_zero,
        ucTok_is_boolean__returns_true_for_1,
        ucTok_is_boolean__returns_true_for_on,
        ucTok_is_boolean__returns_true_for_yes,
        ucTok_is_boolean__returns_true_for_true,
        ucTok_is_boolean__returns_true_for_0,
        ucTok_is_boolean__returns_true_for_off,
        ucTok_is_boolean__returns_true_for_no,
        ucTok_is_boolean__returns_true_for_false,
        ucTok_is_boolean__returns_false_for_numeric,
        ucTok_is_boolean__returns_false_for_null,
        ucTok_is_boolean__returns_false_for_string,
        ucTok_parse_boolean__parses_1,
        ucTok_parse_boolean__parses_on,
        ucTok_parse_boolean__parses_yes,
        ucTok_parse_boolean__parses_true,
        ucTok_parse_boolean__parses_0,
        ucTok_parse_boolean__parses_off,
        ucTok_parse_boolean__parses_no,
        ucTok_parse_boolean__parses_false,
        ucTok_parse_boolean__does_not_parse_invalid_string,
        ucTok_parse_boolean__does_not_parse_null,
        ucTok_parse_boolean__does_not_parse_numeric,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
