#include <stdlib.h>
#include <string.h>
#include "ucmdtests.h"

static ucTestErr ucTok_get_length_returns_string_length(ucTestGroup *p) {
    char s[] = { 'h', 'e', 'l', 'l', 'o', '\0' };
    ucTok *ptr = s;
    ucTEST(ucTok_get_length(ptr) == strlen(s));
    ucPASS();
}

static ucTestErr ucTok_get_length_returns_zero_for_empty_token(ucTestGroup *p) {
    ucTEST(0 == ucTok_get_length((ucTok*)""));
    ucTEST(0 == ucTok_get_length((ucTok*)NULL));
    ucPASS();
}

static ucTestErr ucTok_get_length_ignores_chars_trailing_separator(ucTestGroup *p) {
    char s[] = { '1', '2', '3', '\0', '4', '5', '\0' };
    ucTok *ptr = s;
    ucTEST(3 == ucTok_get_length(ptr));
    ucPASS();
}

static ucTestErr ucTok_get_value_returns_string_value(ucTestGroup *p) {
    char s[] = { 'g', 'o', 'o', 'd', 'b', 'y', 'e', '\0' };
    ucTok *ptr = s;
    ucTEST(ucTok_get_value(ptr));
    ucPASS();
}

static ucTestErr ucTok_get_value_ignores_chars_after_separator(ucTestGroup *p) {
    char s[] = { 'g', 'o', 'o', 'd', '\0', 'b', 'y', 'e', '\0' };
    ucTok *ptr = s;
    ucTEST(ucTok_get_value(ptr));
    ucPASS();
}

static ucTestErr ucTok_equals_returns_true_if_strings_are_equal(ucTestGroup *p) {
    char *value1 = "value";
    char value2[] = { 'v', 'a', 'l', 'u', 'e', '\0', 'n', 'a', '\0' };
    ucTok *ptr = "value";
    ucTEST(ucBool_TRUE == ucTok_equals(ptr, value1));
    ucTEST(ucBool_TRUE == ucTok_equals(ptr, value2));
    ucPASS();
}

static ucTestErr ucTok_equals_returns_true_for_nulls(ucTestGroup *p) {
    char  *value = NULL;
    ucTok *ptr = NULL;
    ucTEST(ucBool_TRUE == ucTok_equals(ptr, value));
    ucPASS();
}

static ucTestErr ucTok_equals_returns_false_if_strings_are_different(ucTestGroup *p) {
    char *value1 = "value";
    char value2[] = { 'v', 'a', 'l', 'u', '\0', 'u', '\0' };
    ucTok *ptr = "valuu";
    ucTEST(ucBool_FALSE == ucTok_equals(ptr, value1));
    ucTEST(ucBool_FALSE == ucTok_equals(ptr, value2));
    ucPASS();
}

static ucTestErr ucTok_equals_returns_false_if_either_arg_is_null(ucTestGroup *p) {
    char *value = "value";
    ucTok *ptr = NULL;
    ucTEST(ucBool_FALSE == ucTok_equals(ptr, value));

    ptr = value;
    value = NULL;
    ucTEST(ucBool_FALSE == ucTok_equals(ptr, value));

    ucPASS();
}

static ucTestErr ucTok_is_numeric_returns_true_for_numbers(ucTestGroup *p) {
    char n1[] = { '4', '8', '.', '9', '1', '\0', 'n', 'a', '\0' };
    ucTEST(ucBool_TRUE == ucTok_is_numeric((ucTok*)"1"));
    ucTEST(ucBool_TRUE == ucTok_is_numeric((ucTok*)"1.2"));
    ucTEST(ucBool_TRUE == ucTok_is_numeric((ucTok*)"87"));
    ucTEST(ucBool_TRUE == ucTok_is_numeric((ucTok*)"-1"));
    ucTEST(ucBool_TRUE == ucTok_is_numeric((ucTok*)n1));
    ucTEST(ucBool_TRUE == ucTok_is_numeric((ucTok*)"-543.234610"));
    ucPASS();
}

static ucTestErr ucTok_is_numeric_returns_false_for_non_numbers(ucTestGroup *p) {
    char c1[] = { '4', '8', '.', '9', '1', 'n', 'a', '\0' };
    ucTEST(ucBool_FALSE == ucTok_is_numeric((ucTok*)"a"));
    ucTEST(ucBool_FALSE == ucTok_is_numeric((ucTok*)"1234_"));
    ucTEST(ucBool_FALSE == ucTok_is_numeric((ucTok*)"1_234"));
    ucTEST(ucBool_FALSE == ucTok_is_numeric((ucTok*)c1));
    ucTEST(ucBool_FALSE == ucTok_is_numeric((ucTok*)NULL));
    ucPASS();
}

static ucTestErr ucTok_is_switch_returns_true_for_switches(ucTestGroup *p) {
    char s1[] = { '-', 's', '1', '\0', 'n', 'a', '\0' };
    ucTEST(ucBool_TRUE == ucTok_is_switch((ucTok*)"-switch"));
    ucTEST(ucBool_TRUE == ucTok_is_switch((ucTok*)"--double"));
    ucTEST(ucBool_TRUE == ucTok_is_switch((ucTok*)"-em-bed"));
    ucTEST(ucBool_TRUE == ucTok_is_switch((ucTok*)s1));
    ucTEST(ucBool_TRUE == ucTok_is_switch((ucTok*)"-1.23.4"));
    ucPASS();
}

static ucTestErr ucTok_is_switch_returns_false_for_non_switches(ucTestGroup *p) {
    char a1[] = { 'a', 'r', 'g', '-', 'd', 'a', 's', 'h', '\0' };
    ucTEST(ucBool_FALSE == ucTok_is_switch((ucTok*)"arg"));
    ucTEST(ucBool_FALSE == ucTok_is_switch((ucTok*)"e-mbed"));
    ucTEST(ucBool_FALSE == ucTok_is_switch((ucTok*)"-1"));
    ucTEST(ucBool_FALSE == ucTok_is_switch((ucTok*)"-1.234"));
    ucTEST(ucBool_FALSE == ucTok_is_switch((ucTok*)a1));
    ucTEST(ucBool_FALSE == ucTok_is_switch((ucTok*)NULL));
    ucPASS();
}

static ucTestErr ucTok_get_next_returns_next_token(ucTestGroup *p) {
    ucTEST(ucTok_equals(ucTok_get_next((ucTok*)"t1\0t2"), "t2"));
    ucTEST(ucTok_equals(ucTok_get_next((ucTok*)"t1\0-s1\0t2"), "-s1"));
    ucTEST(ucTok_equals(ucTok_get_next((ucTok*)"a1\0\na2"), NULL));
    ucPASS();
}

static ucTestErr ucTok_count_counts_tokens(ucTestGroup *p) {
    ucTEST(0 == ucTok_count((ucTok*)NULL));
    ucTEST(0 == ucTok_count((ucTok*)"\n"));
    ucTEST(0 == ucTok_count((ucTok*)"\0\0\0\n"));
    ucTEST(1 == ucTok_count((ucTok*)"a1\0\n"));
    ucTEST(1 == ucTok_count((ucTok*)"-s1\0\n"));
    ucTEST(3 == ucTok_count((ucTok*)"tok en\0a1\0-s1\0\nNA"));
    ucPASS();
}

static ucTestErr ucTok_try_parse_numeric__test(ucTestGroup *p, const char *s, ucBool parsed) {
    double expected, actual;
    ucTEST(parsed == ucTok_try_parse_numeric((ucTok*)s, &actual));
    if (parsed) {
        expected = atof(s);
        ucTEST(actual == expected);
    }
    ucPASS();
}

static ucTestErr ucTok_try_parse_numeric__does_not_parse_null(ucTestGroup *p) {
    return ucTok_try_parse_numeric__test(p, NULL, ucBool_FALSE);
}

static ucTestErr ucTok_try_parse_numeric__does_not_parse_string(ucTestGroup *p) {
    return ucTok_try_parse_numeric__test(p, "not numeric", ucBool_FALSE);
}

static ucTestErr ucTok_try_parse_numeric__does_not_parse_invalid_number(ucTestGroup *p) {
    return ucTok_try_parse_numeric__test(p, "1.34.0", ucBool_FALSE);
}

static ucTestErr ucTok_try_parse_numeric__parses_number(ucTestGroup *p) {
    return ucTok_try_parse_numeric__test(p, "1.34", ucBool_TRUE);
}

static ucTestErr ucTok_try_parse_numeric__parses_negative_number(ucTestGroup *p) {
    return ucTok_try_parse_numeric__test(p, "-349.104", ucBool_TRUE);
}

static ucTestErr ucTok_try_parse_numeric__parses_zero(ucTestGroup *p) {
    ucTEST(!ucTok_try_parse_numeric__test(p, "0", ucBool_TRUE));
    ucTEST(!ucTok_try_parse_numeric__test(p, "0.000", ucBool_TRUE));
    ucPASS();
}

static ucTestErr ucTok_is_boolean__test(ucTestGroup *p, const char *s, ucBool is_boolean) {
    ucTEST(is_boolean == ucTok_is_boolean((ucTok*)s));
    ucPASS();
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

static ucTestErr ucTok_try_parse_boolean__test(ucTestGroup *p, const char *s, ucBool is_boolean, ucBool expected) {
    ucBool actual;
    ucTEST(is_boolean == ucTok_try_parse_boolean((ucTok*)s, &actual));
    if (is_boolean) {
        ucTEST(expected == actual);
    }
    ucPASS();
}

static ucTestErr ucTok_try_parse_boolean__parses_1(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "1", ucBool_TRUE, ucBool_TRUE);
}

static ucTestErr ucTok_try_parse_boolean__parses_on(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "on", ucBool_TRUE, ucBool_TRUE);
}

static ucTestErr ucTok_try_parse_boolean__parses_yes(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "yes", ucBool_TRUE, ucBool_TRUE);
}

static ucTestErr ucTok_try_parse_boolean__parses_true(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "true", ucBool_TRUE, ucBool_TRUE);
}

static ucTestErr ucTok_try_parse_boolean__parses_0(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "0", ucBool_TRUE, ucBool_FALSE);
}

static ucTestErr ucTok_try_parse_boolean__parses_off(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "off", ucBool_TRUE, ucBool_FALSE);
}

static ucTestErr ucTok_try_parse_boolean__parses_no(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "no", ucBool_TRUE, ucBool_FALSE);
}

static ucTestErr ucTok_try_parse_boolean__parses_false(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "false", ucBool_TRUE, ucBool_FALSE);
}

static ucTestErr ucTok_try_parse_boolean__does_not_parse_invalid_string(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "invalid", ucBool_FALSE, ucBool_FALSE);    
}

static ucTestErr ucTok_try_parse_boolean__does_not_parse_null(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, NULL, ucBool_FALSE, ucBool_FALSE);
}

static ucTestErr ucTok_try_parse_boolean__does_not_parse_numeric(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "0.0", ucBool_FALSE, ucBool_FALSE);
}

static ucTestErr ucTok_parse_numeric__test(ucTestGroup *p, const char *s, double expected) {
    ucTEST(expected == ucTok_parse_numeric((ucTok*)s));
    ucPASS();
}

static ucTestErr ucTok_parse_numeric__parses_number(ucTestGroup *p) {
    return ucTok_parse_numeric__test(p, "5.43210", 5.43210);
}

static ucTestErr ucTok_parse_numeric__does_not_parse_invalid(ucTestGroup *p) {
    return ucTok_parse_numeric__test(p, "invalid", 0);
}

static ucTestErr ucTok_parse_boolean__test(ucTestGroup *p, const char *s, ucBool expected) {
    ucTEST(expected == ucTok_parse_boolean((ucTok*)s));
    ucPASS();
}

static ucTestErr ucTok_parse_boolean__parses_true(ucTestGroup *p) {
    ucTEST(!ucTok_parse_boolean__test(p, "true", ucBool_TRUE));
    ucTEST(!ucTok_parse_boolean__test(p, "yes", ucBool_TRUE));
    ucTEST(!ucTok_parse_boolean__test(p, "on", ucBool_TRUE));
    ucTEST(!ucTok_parse_boolean__test(p, "1", ucBool_TRUE));
    ucPASS();
}

static ucTestErr ucTok_parse_boolean__parses_false(ucTestGroup *p) {
    ucTEST(!ucTok_parse_boolean__test(p, "false", ucBool_FALSE));
    ucTEST(!ucTok_parse_boolean__test(p, "no", ucBool_FALSE));
    ucTEST(!ucTok_parse_boolean__test(p, "off", ucBool_FALSE));
    ucTEST(!ucTok_parse_boolean__test(p, "0", ucBool_FALSE));
    ucPASS();
}

static ucTestErr ucTok_parse_boolean__does_not_parse_invalid(ucTestGroup *p) {
    ucTEST(!ucTok_parse_boolean__test(p, "truthy", ucBool_FALSE));
    ucTEST(!ucTok_parse_boolean__test(p, "yup", ucBool_FALSE));
    ucTEST(!ucTok_parse_boolean__test(p, "bzzz", ucBool_FALSE));
    ucTEST(!ucTok_parse_boolean__test(p, "1.0", ucBool_FALSE));
    ucPASS();
}

static ucTestErr ucTok_is_integer__test(ucTestGroup *p, const char *s, ucBool expected) {
    ucTEST(expected == ucTok_is_integer((ucTok*)s));
    ucPASS();
}

static ucTestErr ucTok_is_integer__returns_true_for_int(ucTestGroup *p) {
    ucTEST(!ucTok_is_integer__test(p, "-4923", ucBool_TRUE));
    ucTEST(!ucTok_is_integer__test(p, "-1", ucBool_TRUE));
    ucTEST(!ucTok_is_integer__test(p, "0", ucBool_TRUE));
    ucTEST(!ucTok_is_integer__test(p, "1", ucBool_TRUE));
    ucTEST(!ucTok_is_integer__test(p, "2341", ucBool_TRUE));
    ucPASS();
}

static ucTestErr ucTok_is_integer__returns_false_for_numeric(ucTestGroup *p) {
    ucTEST(!ucTok_is_integer__test(p, "-49.23", ucBool_FALSE));
    ucTEST(!ucTok_is_integer__test(p, "-1.1", ucBool_FALSE));
    ucTEST(!ucTok_is_integer__test(p, "0.0", ucBool_FALSE));
    ucTEST(!ucTok_is_integer__test(p, "1.0", ucBool_FALSE));
    ucTEST(!ucTok_is_integer__test(p, "234.1", ucBool_FALSE));
    ucPASS();
}

static ucTestErr ucTok_is_integer__returns_false_for_null(ucTestGroup *p) {
    ucTEST(!ucTok_is_integer__test(p, NULL, ucBool_FALSE));
    ucPASS();
}

static ucTestErr ucTok_is_integer__returns_false_for_strings(ucTestGroup *p) {
    ucTEST(!ucTok_is_integer__test(p, "a-4923", ucBool_FALSE));
    ucTEST(!ucTok_is_integer__test(p, "-11b", ucBool_FALSE));
    ucTEST(!ucTok_is_integer__test(p, "--00", ucBool_FALSE));
    ucTEST(!ucTok_is_integer__test(p, "56    -", ucBool_FALSE));
    ucTEST(!ucTok_is_integer__test(p, "NOT AN INT", ucBool_FALSE));
    ucPASS();
}

static ucTestErr ucTok_try_parse_integer__test(ucTestGroup *p, const char *s, ucBool parsed, int result) {
    int r;
    ucTEST(parsed == ucTok_try_parse_integer((ucTok*)s, &r));
    if (parsed) {
        ucTEST(result == r);
    }
    ucPASS();
}

static ucTestErr ucTok_try_parse_integer__returns_false_if_not_parsed(ucTestGroup *p) {
    ucTEST(!ucTok_try_parse_integer__test(p, "invalid", ucBool_FALSE, 0));
    ucTEST(!ucTok_try_parse_integer__test(p, "--4", ucBool_FALSE, 0));
    ucTEST(!ucTok_try_parse_integer__test(p, "-4a", ucBool_FALSE, 0));
    ucTEST(!ucTok_try_parse_integer__test(p, "1.0", ucBool_FALSE, 0));
    ucPASS();
}

static ucTestErr ucTok_try_parse_integer__returns_true_if_parsed(ucTestGroup *p) {
    ucTEST(!ucTok_try_parse_integer__test(p, "43", ucBool_TRUE, 43));
    ucTEST(!ucTok_try_parse_integer__test(p, "-211", ucBool_TRUE, -211));
    ucTEST(!ucTok_try_parse_integer__test(p, "0", ucBool_TRUE, 0));
    ucTEST(!ucTok_try_parse_integer__test(p, "1", ucBool_TRUE, 1));
    ucPASS();
}

static ucTestErr ucTok_parse_integer__test(ucTestGroup *p, const char *s, int i) {
    ucTEST(i == ucTok_parse_integer((ucTok*)s));
    ucPASS();
}

static ucTestErr ucTok_parse_integer__returns_0_for_invalid_strings(ucTestGroup *p) {
    ucTEST(!ucTok_parse_integer__test(p, "--1", 0));
    ucTEST(!ucTok_parse_integer__test(p, "2.1", 0));
    ucPASS();
}

static ucTestErr ucTok_parse_integer__returns_integer_value(ucTestGroup *p) {
    ucTEST(!ucTok_parse_integer__test(p, "1", 1));
    ucTEST(!ucTok_parse_integer__test(p, "21", 21));
    ucTEST(!ucTok_parse_integer__test(p, "-3294821", -3294821));
    ucPASS();
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
        ucTok_try_parse_numeric__does_not_parse_null,
        ucTok_try_parse_numeric__does_not_parse_string,
        ucTok_try_parse_numeric__does_not_parse_invalid_number,
        ucTok_try_parse_numeric__parses_number,
        ucTok_try_parse_numeric__parses_negative_number,
        ucTok_try_parse_numeric__parses_zero,
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
        ucTok_try_parse_boolean__parses_1,
        ucTok_try_parse_boolean__parses_on,
        ucTok_try_parse_boolean__parses_yes,
        ucTok_try_parse_boolean__parses_true,
        ucTok_try_parse_boolean__parses_0,
        ucTok_try_parse_boolean__parses_off,
        ucTok_try_parse_boolean__parses_no,
        ucTok_try_parse_boolean__parses_false,
        ucTok_try_parse_boolean__does_not_parse_invalid_string,
        ucTok_try_parse_boolean__does_not_parse_null,
        ucTok_try_parse_boolean__does_not_parse_numeric,
        ucTok_parse_numeric__parses_number,
        ucTok_parse_numeric__does_not_parse_invalid,
        ucTok_parse_boolean__parses_true,
        ucTok_parse_boolean__parses_false,
        ucTok_parse_boolean__does_not_parse_invalid,
        ucTok_is_integer__returns_true_for_int,
        ucTok_is_integer__returns_false_for_numeric,
        ucTok_is_integer__returns_false_for_null,
        ucTok_is_integer__returns_false_for_strings,
        ucTok_try_parse_integer__returns_false_if_not_parsed,
        ucTok_try_parse_integer__returns_true_if_parsed,
        ucTok_parse_integer__returns_0_for_invalid_strings,
        ucTok_parse_integer__returns_integer_value,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
