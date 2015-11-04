#include <stdlib.h>
#include <string.h>
#include "ucmdtests.h"

static ucTestErr ucTok_get_length_returns_string_length(ucTestGroup *p) {
    char s[] = { 'h', 'e', 'l', 'l', 'o', '\0' };
    ucTok *ptr = s;
    ucTRUE(ucTok_get_length(ptr) == strlen(s));
    ucPASS();
}

static ucTestErr ucTok_get_length_returns_zero_for_empty_token(ucTestGroup *p) {
    ucTRUE(0 == ucTok_get_length((ucTok*)""));
    ucPASS();
}

static ucTestErr ucTok_get_length_ignores_chars_trailing_separator(ucTestGroup *p) {
    char s[] = { '1', '2', '3', '\0', '4', '5', '\0' };
    ucTok *ptr = s;
    ucTRUE(3 == ucTok_get_length(ptr));
    ucPASS();
}

static ucTestErr ucTok_get_value_returns_string_value(ucTestGroup *p) {
    char s[] = { 'g', 'o', 'o', 'd', 'b', 'y', 'e', '\0' };
    ucTok *ptr = s;
    ucTRUE(ucTok_get_value(ptr));
    ucPASS();
}

static ucTestErr ucTok_get_value_ignores_chars_after_separator(ucTestGroup *p) {
    char s[] = { 'g', 'o', 'o', 'd', '\0', 'b', 'y', 'e', '\0' };
    ucTok *ptr = s;
    ucTRUE(ucTok_get_value(ptr));
    ucPASS();
}

static ucTestErr ucTok_equals_returns_true_if_strings_are_equal(ucTestGroup *p) {
    char *value1 = "value";
    char value2[] = { 'v', 'a', 'l', 'u', 'e', '\0', 'n', 'a', '\0' };
    ucTok *ptr = "value";
    ucTRUE(ucBool_true == ucTok_equals(ptr, value1));
    ucTRUE(ucBool_true == ucTok_equals(ptr, value2));
    ucPASS();
}

static ucTestErr ucTok_equals_returns_true_for_empty_strings(ucTestGroup *p) {
    ucTok *ptr = "";
    char empty_string[1] = { "" };
    ucTRUE(ucBool_true == ucTok_equals(ptr, ""));
    ucTRUE(ucBool_true == ucTok_equals(ptr, empty_string));
    ucPASS();
}

static ucTestErr ucTok_equals_returns_false_if_strings_are_different(ucTestGroup *p) {
    char *value1 = "value";
    char value2[] = { 'v', 'a', 'l', 'u', '\0', 'u', '\0' };
    ucTok *ptr = "valuu";
    ucTRUE(ucBool_false == ucTok_equals(ptr, value1));
    ucTRUE(ucBool_false == ucTok_equals(ptr, value2));
    ucPASS();
}

static ucTestErr ucTok_equals_returns_false_if_value_is_null(ucTestGroup *p) {
    char *value = NULL;
    ucTok *ptr = "value";
    ucTRUE(ucBool_false == ucTok_equals(ptr, value));
    ucPASS();
}

static ucTestErr ucTok_is_numeric_returns_true_for_numbers(ucTestGroup *p) {
    char n1[] = { '4', '8', '.', '9', '1', '\0', 'n', 'a', '\0' };
    ucTRUE(ucBool_true == ucTok_is_numeric((ucTok*)"1"));
    ucTRUE(ucBool_true == ucTok_is_numeric((ucTok*)"1.2"));
    ucTRUE(ucBool_true == ucTok_is_numeric((ucTok*)"87"));
    ucTRUE(ucBool_true == ucTok_is_numeric((ucTok*)"-1"));
    ucTRUE(ucBool_true == ucTok_is_numeric((ucTok*)n1));
    ucTRUE(ucBool_true == ucTok_is_numeric((ucTok*)"-543.234610"));
    ucPASS();
}

static ucTestErr ucTok_is_numeric_returns_false_for_non_numbers(ucTestGroup *p) {
    char c1[] = { '4', '8', '.', '9', '1', 'n', 'a', '\0' };
    ucTRUE(ucBool_false == ucTok_is_numeric((ucTok*)"a"));
    ucTRUE(ucBool_false == ucTok_is_numeric((ucTok*)"1234_"));
    ucTRUE(ucBool_false == ucTok_is_numeric((ucTok*)"1_234"));
    ucTRUE(ucBool_false == ucTok_is_numeric((ucTok*)c1));
    ucPASS();
}

static ucTestErr ucTok_is_switch_returns_true_for_switches(ucTestGroup *p) {
    char s1[] = { '-', 's', '1', '\0', 'n', 'a', '\0' };
    ucTRUE(ucBool_true == ucTok_is_switch((ucTok*)"-switch"));
    ucTRUE(ucBool_true == ucTok_is_switch((ucTok*)"--double"));
    ucTRUE(ucBool_true == ucTok_is_switch((ucTok*)"-em-bed"));
    ucTRUE(ucBool_true == ucTok_is_switch((ucTok*)s1));
    ucTRUE(ucBool_true == ucTok_is_switch((ucTok*)"-1.23.4"));
    ucPASS();
}

static ucTestErr ucTok_is_switch_returns_false_for_non_switches(ucTestGroup *p) {
    char a1[] = { 'a', 'r', 'g', '-', 'd', 'a', 's', 'h', '\0' };
    ucTRUE(ucBool_false == ucTok_is_switch((ucTok*)"arg"));
    ucTRUE(ucBool_false == ucTok_is_switch((ucTok*)"e-mbed"));
    ucTRUE(ucBool_false == ucTok_is_switch((ucTok*)"-1"));
    ucTRUE(ucBool_false == ucTok_is_switch((ucTok*)"-1.234"));
    ucTRUE(ucBool_false == ucTok_is_switch((ucTok*)a1));
    ucPASS();
}

static ucTestErr ucTok_get_next_returns_next_token(ucTestGroup *p) {
    ucTRUE(ucTok_equals(ucTok_get_next((ucTok*)"t1\0t2"), "t2"));
    ucTRUE(ucTok_equals(ucTok_get_next((ucTok*)"t1\0-s1\0t2"), "-s1"));
    ucPASS();
}

static ucTestErr ucTok_get_next_returns_null_if_line_is_terminated(ucTestGroup *p) {
    ucTRUE(NULL == ucTok_get_next((ucTok*)"a1\0\na2"));
    ucPASS();
}

static ucTestErr ucTok_try_parse_numeric__test(ucTestGroup *p, const char *s, ucBool parsed) {
    double expected, actual;
    ucTRUE(parsed == ucTok_try_parse_numeric((ucTok*)s, &actual));
    if (parsed) {
        expected = atof(s);
        ucTRUE(actual == expected);
    }
    ucPASS();
}

static ucTestErr ucTok_try_parse_numeric__does_not_parse_string(ucTestGroup *p) {
    return ucTok_try_parse_numeric__test(p, "not numeric", ucBool_false);
}

static ucTestErr ucTok_try_parse_numeric__does_not_parse_invalid_number(ucTestGroup *p) {
    return ucTok_try_parse_numeric__test(p, "1.34.0", ucBool_false);
}

static ucTestErr ucTok_try_parse_numeric__parses_number(ucTestGroup *p) {
    return ucTok_try_parse_numeric__test(p, "1.34", ucBool_true);
}

static ucTestErr ucTok_try_parse_numeric__parses_negative_number(ucTestGroup *p) {
    return ucTok_try_parse_numeric__test(p, "-349.104", ucBool_true);
}

static ucTestErr ucTok_try_parse_numeric__parses_zero(ucTestGroup *p) {
    ucTRUE(!ucTok_try_parse_numeric__test(p, "0", ucBool_true));
    ucTRUE(!ucTok_try_parse_numeric__test(p, "0.000", ucBool_true));
    ucPASS();
}

static ucTestErr ucTok_is_boolean__test(ucTestGroup *p, const char *s, ucBool is_boolean) {
    ucTRUE(is_boolean == ucTok_is_boolean((ucTok*)s));
    ucPASS();
}

static ucTestErr ucTok_is_boolean__returns_true_for_1(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "1", ucBool_true);
}

static ucTestErr ucTok_is_boolean__returns_true_for_on(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "on", ucBool_true);
}

static ucTestErr ucTok_is_boolean__returns_true_for_yes(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "yes", ucBool_true);
}

static ucTestErr ucTok_is_boolean__returns_true_for_true(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "true", ucBool_true);
}

static ucTestErr ucTok_is_boolean__returns_true_for_0(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "0", ucBool_true);
}

static ucTestErr ucTok_is_boolean__returns_true_for_off(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "off", ucBool_true);
}

static ucTestErr ucTok_is_boolean__returns_true_for_no(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "no", ucBool_true);
}

static ucTestErr ucTok_is_boolean__returns_true_for_false(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "false", ucBool_true);
}

static ucTestErr ucTok_is_boolean__returns_false_for_numeric(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "1.0", ucBool_false);
}

static ucTestErr ucTok_is_boolean__returns_false_for_string(ucTestGroup *p) {
    return ucTok_is_boolean__test(p, "not a bool", ucBool_false);
}

static ucTestErr ucTok_try_parse_boolean__test(ucTestGroup *p, const char *s, ucBool is_boolean, ucBool expected) {
    ucBool actual;
    ucTRUE(is_boolean == ucTok_try_parse_boolean((ucTok*)s, &actual));
    if (is_boolean) {
        ucTRUE(expected == actual);
    }
    ucPASS();
}

static ucTestErr ucTok_try_parse_boolean__parses_1(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "1", ucBool_true, ucBool_true);
}

static ucTestErr ucTok_try_parse_boolean__parses_on(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "on", ucBool_true, ucBool_true);
}

static ucTestErr ucTok_try_parse_boolean__parses_yes(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "yes", ucBool_true, ucBool_true);
}

static ucTestErr ucTok_try_parse_boolean__parses_true(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "true", ucBool_true, ucBool_true);
}

static ucTestErr ucTok_try_parse_boolean__parses_0(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "0", ucBool_true, ucBool_false);
}

static ucTestErr ucTok_try_parse_boolean__parses_off(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "off", ucBool_true, ucBool_false);
}

static ucTestErr ucTok_try_parse_boolean__parses_no(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "no", ucBool_true, ucBool_false);
}

static ucTestErr ucTok_try_parse_boolean__parses_false(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "false", ucBool_true, ucBool_false);
}

static ucTestErr ucTok_try_parse_boolean__does_not_parse_invalid_string(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "invalid", ucBool_false, ucBool_false);    
}

static ucTestErr ucTok_try_parse_boolean__does_not_parse_numeric(ucTestGroup *p) {
    return ucTok_try_parse_boolean__test(p, "0.0", ucBool_false, ucBool_false);
}

static ucTestErr ucTok_parse_numeric__test(ucTestGroup *p, const char *s, double expected) {
    ucTRUE(expected == ucTok_parse_numeric((ucTok*)s));
    ucPASS();
}

static ucTestErr ucTok_parse_numeric__parses_number(ucTestGroup *p) {
    return ucTok_parse_numeric__test(p, "5.43210", 5.43210);
}

static ucTestErr ucTok_parse_numeric__does_not_parse_invalid(ucTestGroup *p) {
    return ucTok_parse_numeric__test(p, "invalid", 0);
}

static ucTestErr ucTok_parse_boolean__test(ucTestGroup *p, const char *s, ucBool expected) {
    ucTRUE(expected == ucTok_parse_boolean((ucTok*)s));
    ucPASS();
}

static ucTestErr ucTok_parse_boolean__parses_true(ucTestGroup *p) {
    ucTRUE(!ucTok_parse_boolean__test(p, "true", ucBool_true));
    ucTRUE(!ucTok_parse_boolean__test(p, "yes", ucBool_true));
    ucTRUE(!ucTok_parse_boolean__test(p, "on", ucBool_true));
    ucTRUE(!ucTok_parse_boolean__test(p, "1", ucBool_true));
    ucPASS();
}

static ucTestErr ucTok_parse_boolean__parses_false(ucTestGroup *p) {
    ucTRUE(!ucTok_parse_boolean__test(p, "false", ucBool_false));
    ucTRUE(!ucTok_parse_boolean__test(p, "no", ucBool_false));
    ucTRUE(!ucTok_parse_boolean__test(p, "off", ucBool_false));
    ucTRUE(!ucTok_parse_boolean__test(p, "0", ucBool_false));
    ucPASS();
}

static ucTestErr ucTok_parse_boolean__does_not_parse_invalid(ucTestGroup *p) {
    ucTRUE(!ucTok_parse_boolean__test(p, "truthy", ucBool_false));
    ucTRUE(!ucTok_parse_boolean__test(p, "yup", ucBool_false));
    ucTRUE(!ucTok_parse_boolean__test(p, "bzzz", ucBool_false));
    ucTRUE(!ucTok_parse_boolean__test(p, "1.0", ucBool_false));
    ucPASS();
}

static ucTestErr ucTok_is_integer__test(ucTestGroup *p, const char *s, ucBool expected) {
    ucTRUE(expected == ucTok_is_integer((ucTok*)s));
    ucPASS();
}

static ucTestErr ucTok_is_integer__returns_true_for_int(ucTestGroup *p) {
    ucTRUE(!ucTok_is_integer__test(p, "-4923", ucBool_true));
    ucTRUE(!ucTok_is_integer__test(p, "-1", ucBool_true));
    ucTRUE(!ucTok_is_integer__test(p, "0", ucBool_true));
    ucTRUE(!ucTok_is_integer__test(p, "1", ucBool_true));
    ucTRUE(!ucTok_is_integer__test(p, "2341", ucBool_true));
    ucPASS();
}

static ucTestErr ucTok_is_integer__returns_false_for_numeric(ucTestGroup *p) {
    ucTRUE(!ucTok_is_integer__test(p, "-49.23", ucBool_false));
    ucTRUE(!ucTok_is_integer__test(p, "-1.1", ucBool_false));
    ucTRUE(!ucTok_is_integer__test(p, "0.0", ucBool_false));
    ucTRUE(!ucTok_is_integer__test(p, "1.0", ucBool_false));
    ucTRUE(!ucTok_is_integer__test(p, "234.1", ucBool_false));
    ucPASS();
}

static ucTestErr ucTok_is_integer__returns_false_for_strings(ucTestGroup *p) {
    ucTRUE(!ucTok_is_integer__test(p, "a-4923", ucBool_false));
    ucTRUE(!ucTok_is_integer__test(p, "-11b", ucBool_false));
    ucTRUE(!ucTok_is_integer__test(p, "--00", ucBool_false));
    ucTRUE(!ucTok_is_integer__test(p, "56    -", ucBool_false));
    ucTRUE(!ucTok_is_integer__test(p, "NOT AN INT", ucBool_false));
    ucPASS();
}

static ucTestErr ucTok_try_parse_integer__test(ucTestGroup *p, const char *s, ucBool parsed, int result) {
    int r;
    ucTRUE(parsed == ucTok_try_parse_integer((ucTok*)s, &r));
    if (parsed) {
        ucTRUE(result == r);
    }
    ucPASS();
}

static ucTestErr ucTok_try_parse_integer__returns_false_if_not_parsed(ucTestGroup *p) {
    ucTRUE(!ucTok_try_parse_integer__test(p, "invalid", ucBool_false, 0));
    ucTRUE(!ucTok_try_parse_integer__test(p, "--4", ucBool_false, 0));
    ucTRUE(!ucTok_try_parse_integer__test(p, "-4a", ucBool_false, 0));
    ucTRUE(!ucTok_try_parse_integer__test(p, "1.0", ucBool_false, 0));
    ucPASS();
}

static ucTestErr ucTok_try_parse_integer__returns_true_if_parsed(ucTestGroup *p) {
    ucTRUE(!ucTok_try_parse_integer__test(p, "43", ucBool_true, 43));
    ucTRUE(!ucTok_try_parse_integer__test(p, "-211", ucBool_true, -211));
    ucTRUE(!ucTok_try_parse_integer__test(p, "0", ucBool_true, 0));
    ucTRUE(!ucTok_try_parse_integer__test(p, "1", ucBool_true, 1));
    ucPASS();
}

static ucTestErr ucTok_parse_integer__test(ucTestGroup *p, const char *s, int i) {
    ucTRUE(i == ucTok_parse_integer((ucTok*)s));
    ucPASS();
}

static ucTestErr ucTok_parse_integer__returns_0_for_invalid_strings(ucTestGroup *p) {
    ucTRUE(!ucTok_parse_integer__test(p, "--1", 0));
    ucTRUE(!ucTok_parse_integer__test(p, "2.1", 0));
    ucPASS();
}

static ucTestErr ucTok_parse_integer__returns_integer_value(ucTestGroup *p) {
    ucTRUE(!ucTok_parse_integer__test(p, "1", 1));
    ucTRUE(!ucTok_parse_integer__test(p, "21", 21));
    ucTRUE(!ucTok_parse_integer__test(p, "-3294821", -3294821));
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
        ucTok_equals_returns_true_for_empty_strings,
        ucTok_equals_returns_false_if_strings_are_different,
        ucTok_equals_returns_false_if_value_is_null,
        ucTok_is_numeric_returns_true_for_numbers,
        ucTok_is_numeric_returns_false_for_non_numbers,
        ucTok_is_numeric_returns_false_for_non_numbers,
        ucTok_is_switch_returns_true_for_switches,
        ucTok_is_switch_returns_false_for_non_switches,
        ucTok_get_next_returns_next_token,
        ucTok_get_next_returns_null_if_line_is_terminated,
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
        ucTok_try_parse_boolean__does_not_parse_numeric,
        ucTok_parse_numeric__parses_number,
        ucTok_parse_numeric__does_not_parse_invalid,
        ucTok_parse_boolean__parses_true,
        ucTok_parse_boolean__parses_false,
        ucTok_parse_boolean__does_not_parse_invalid,
        ucTok_is_integer__returns_true_for_int,
        ucTok_is_integer__returns_false_for_numeric,
        ucTok_is_integer__returns_false_for_strings,
        ucTok_try_parse_integer__returns_false_if_not_parsed,
        ucTok_try_parse_integer__returns_true_if_parsed,
        ucTok_parse_integer__returns_0_for_invalid_strings,
        ucTok_parse_integer__returns_integer_value,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
