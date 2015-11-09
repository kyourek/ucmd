#include <stdlib.h>
#include "ucmdtests.h"

uc_TEST(ucTok_get_length_returns_string_length)
    char s[] = { 'h', 'e', 'l', 'l', 'o', '\0' };
    ucTok *ptr = s;
    uc_TRUE(ucTok_get_length(ptr) == strlen(s));
uc_PASS

uc_TEST(ucTok_get_length_returns_zero_for_empty_token)
    uc_TRUE(0 == ucTok_get_length((ucTok*)""));
uc_PASS

uc_TEST(ucTok_get_length_ignores_chars_trailing_separator)
    char s[] = { '1', '2', '3', '\0', '4', '5', '\0' };
    ucTok *ptr = s;
    uc_TRUE(3 == ucTok_get_length(ptr));
uc_PASS

uc_TEST(ucTok_equals_returns_true_if_strings_are_equal)
    char *value1 = "value";
    char value2[] = { 'v', 'a', 'l', 'u', 'e', '\0', 'n', 'a', '\0' };
    ucTok *ptr = "value";
    uc_TRUE(ucBool_true == ucTok_equals(ptr, value1));
    uc_TRUE(ucBool_true == ucTok_equals(ptr, value2));
uc_PASS

uc_TEST(ucTok_equals_returns_true_for_empty_strings)
    ucTok *ptr = "";
    char empty_string[1] = { "" };
    uc_TRUE(ucBool_true == ucTok_equals(ptr, ""));
    uc_TRUE(ucBool_true == ucTok_equals(ptr, empty_string));
uc_PASS

uc_TEST(ucTok_equals_returns_false_if_strings_are_different)
    char *value1 = "value";
    char value2[] = { 'v', 'a', 'l', 'u', '\0', 'u', '\0' };
    ucTok *ptr = "valuu";
    uc_TRUE(ucBool_false == ucTok_equals(ptr, value1));
    uc_TRUE(ucBool_false == ucTok_equals(ptr, value2));
uc_PASS

uc_TEST(ucTok_equals_returns_false_if_value_is_null)
    char *value = NULL;
    ucTok *ptr = "value";
    uc_TRUE(ucBool_false == ucTok_equals(ptr, value));
uc_PASS

uc_TEST(ucTok_is_numeric_returns_true_for_numbers)
    char n1[] = { '4', '8', '.', '9', '1', '\0', 'n', 'a', '\0' };
    uc_TRUE(ucBool_true == ucTok_is_numeric((ucTok*)"1"));
    uc_TRUE(ucBool_true == ucTok_is_numeric((ucTok*)"1.2"));
    uc_TRUE(ucBool_true == ucTok_is_numeric((ucTok*)"87"));
    uc_TRUE(ucBool_true == ucTok_is_numeric((ucTok*)"-1"));
    uc_TRUE(ucBool_true == ucTok_is_numeric((ucTok*)n1));
    uc_TRUE(ucBool_true == ucTok_is_numeric((ucTok*)"-543.234610"));
uc_PASS

uc_TEST(ucTok_is_numeric_returns_false_for_non_numbers)
    char c1[] = { '4', '8', '.', '9', '1', 'n', 'a', '\0' };
    uc_TRUE(ucBool_false == ucTok_is_numeric((ucTok*)"a"));
    uc_TRUE(ucBool_false == ucTok_is_numeric((ucTok*)"1234_"));
    uc_TRUE(ucBool_false == ucTok_is_numeric((ucTok*)"1_234"));
    uc_TRUE(ucBool_false == ucTok_is_numeric((ucTok*)c1));
uc_PASS

uc_TEST(ucTok_is_switch_returns_true_for_switches)
    char s1[] = { '-', 's', '1', '\0', 'n', 'a', '\0' };
    uc_TRUE(ucBool_true == ucTok_is_switch((ucTok*)"-switch"));
    uc_TRUE(ucBool_true == ucTok_is_switch((ucTok*)"--double"));
    uc_TRUE(ucBool_true == ucTok_is_switch((ucTok*)"-em-bed"));
    uc_TRUE(ucBool_true == ucTok_is_switch((ucTok*)s1));
    uc_TRUE(ucBool_true == ucTok_is_switch((ucTok*)"-1.23.4"));
uc_PASS

uc_TEST(ucTok_is_switch_returns_false_for_non_switches)
    char a1[] = { 'a', 'r', 'g', '-', 'd', 'a', 's', 'h', '\0' };
    uc_TRUE(ucBool_false == ucTok_is_switch((ucTok*)"arg"));
    uc_TRUE(ucBool_false == ucTok_is_switch((ucTok*)"e-mbed"));
    uc_TRUE(ucBool_false == ucTok_is_switch((ucTok*)"-1"));
    uc_TRUE(ucBool_false == ucTok_is_switch((ucTok*)"-1.234"));
    uc_TRUE(ucBool_false == ucTok_is_switch((ucTok*)a1));
uc_PASS

uc_TEST(ucTok_get_next_returns_next_token)
    uc_TRUE(ucTok_equals(ucTok_get_next((ucTok*)"t1\0t2"), "t2"));
    uc_TRUE(ucTok_equals(ucTok_get_next((ucTok*)"t1\0-s1\0t2"), "-s1"));
uc_PASS

uc_TEST(ucTok_get_next_returns_null_if_line_is_terminated)
    uc_TRUE(NULL == ucTok_get_next((ucTok*)"a1\0\na2"));
uc_PASS

uc_TEST(ucTok_try_parse_numeric_test, const char *s, ucBool parsed)
    double expected, actual;
    uc_TRUE(parsed == ucTok_try_parse_numeric((ucTok*)s, &actual));
    if (parsed) {
        expected = atof(s);
        uc_TRUE(actual == expected);
    }
uc_PASS
uc_CASE(ucTok_try_parse_numeric_test, does_not_parse_string, "not numeric", ucBool_false)
uc_CASE(ucTok_try_parse_numeric_test, does_not_parse_invalid_number, "1.34.0", ucBool_false)
uc_CASE(ucTok_try_parse_numeric_test, parses_number, "1.34", ucBool_true)
uc_CASE(ucTok_try_parse_numeric_test, parses_negative_number, "-349.104", ucBool_true)
uc_CASE(ucTok_try_parse_numeric_test, parses_zero_integer, "0", ucBool_true)
uc_CASE(ucTok_try_parse_numeric_test, parses_zero_with_decimal, "0.000", ucBool_true)

uc_TEST(ucTok_is_boolean_test, const char *s, ucBool is_boolean)
    uc_TRUE(is_boolean == ucTok_is_boolean((ucTok*)s));
uc_PASS
uc_CASE(ucTok_is_boolean_test, returns_true_for_1, "1", ucBool_true)
uc_CASE(ucTok_is_boolean_test, returns_true_for_on, "on", ucBool_true)
uc_CASE(ucTok_is_boolean_test, returns_true_for_yes, "yes", ucBool_true)
uc_CASE(ucTok_is_boolean_test, returns_true_for_true, "true", ucBool_true)
uc_CASE(ucTok_is_boolean_test, returns_true_for_0, "0", ucBool_true)
uc_CASE(ucTok_is_boolean_test, returns_true_for_off, "off", ucBool_true)
uc_CASE(ucTok_is_boolean_test, returns_true_for_no, "no", ucBool_true)
uc_CASE(ucTok_is_boolean_test, returns_true_for_false, "false", ucBool_true)
uc_CASE(ucTok_is_boolean_test, returns_false_for_numeric, "1.0", ucBool_false)
uc_CASE(ucTok_is_boolean_test, returns_false_for_string, "not a bool", ucBool_false)

uc_TEST(ucTok_try_parse_boolean_test, const char *s, ucBool is_boolean, ucBool expected)
    ucBool actual;
    uc_TRUE(is_boolean == ucTok_try_parse_boolean((ucTok*)s, &actual));
    if (is_boolean) {
        uc_TRUE(expected == actual);
    }
uc_PASS
uc_CASE(ucTok_try_parse_boolean_test, parses_1, "1", ucBool_true, ucBool_true)
uc_CASE(ucTok_try_parse_boolean_test, parses_on, "on", ucBool_true, ucBool_true)
uc_CASE(ucTok_try_parse_boolean_test, parses_yes, "yes", ucBool_true, ucBool_true)
uc_CASE(ucTok_try_parse_boolean_test, parses_true, "true", ucBool_true, ucBool_true)
uc_CASE(ucTok_try_parse_boolean_test, parses_0, "0", ucBool_true, ucBool_false)
uc_CASE(ucTok_try_parse_boolean_test, parses_off, "off", ucBool_true, ucBool_false)
uc_CASE(ucTok_try_parse_boolean_test, parses_no, "no", ucBool_true, ucBool_false)
uc_CASE(ucTok_try_parse_boolean_test, parses_false, "false", ucBool_true, ucBool_false)
uc_CASE(ucTok_try_parse_boolean_test, does_not_parse_invalid_string, "invalid", ucBool_false, ucBool_false)
uc_CASE(ucTok_try_parse_boolean_test, does_not_parse_numeric, "0.0", ucBool_false, ucBool_false)

uc_TEST(ucTok_parse_numeric_test, const char *s, double expected)
    uc_TRUE(expected == ucTok_parse_numeric((ucTok*)s));
uc_PASS
uc_CASE(ucTok_parse_numeric_test, parses_number, "5.43210", 5.43210)
uc_CASE(ucTok_parse_numeric_test, does_not_parse_invalid, "invalid", 0)

uc_TEST(ucTok_parse_boolean_test, const char *s, ucBool expected)
    uc_TRUE(expected == ucTok_parse_boolean((ucTok*)s));
uc_PASS
uc_CASE(ucTok_parse_boolean_test, parses_1, "1", ucBool_true)
uc_CASE(ucTok_parse_boolean_test, parses_on, "on", ucBool_true)
uc_CASE(ucTok_parse_boolean_test, parses_yes, "yes", ucBool_true)
uc_CASE(ucTok_parse_boolean_test, parses_true, "true", ucBool_true)
uc_CASE(ucTok_parse_boolean_test, parses_0, "0", ucBool_false)
uc_CASE(ucTok_parse_boolean_test, parses_off, "off", ucBool_false)
uc_CASE(ucTok_parse_boolean_test, parses_no, "no", ucBool_false)
uc_CASE(ucTok_parse_boolean_test, parses_false, "false", ucBool_false)
uc_CASE(ucTok_parse_boolean_test, does_not_parses_truthy, "truthy", ucBool_false)
uc_CASE(ucTok_parse_boolean_test, does_not_parses_yup, "yup", ucBool_false)
uc_CASE(ucTok_parse_boolean_test, does_not_parses_bzzz, "bzzz", ucBool_false)
uc_CASE(ucTok_parse_boolean_test, does_not_parses_1_0, "1.0", ucBool_false)

uc_TEST(ucTok_is_integer_test, const char *s, ucBool expected)
    uc_TRUE(expected == ucTok_is_integer((ucTok*)s));
uc_PASS
uc_CASE(ucTok_is_integer_test, returns_true_for_int_1, "-4923", ucBool_true)
uc_CASE(ucTok_is_integer_test, returns_true_for_int_2, "-1", ucBool_true)
uc_CASE(ucTok_is_integer_test, returns_true_for_int_3, "0", ucBool_true)
uc_CASE(ucTok_is_integer_test, returns_true_for_int_4, "1", ucBool_true)
uc_CASE(ucTok_is_integer_test, returns_true_for_int_5, "2341", ucBool_true)
uc_CASE(ucTok_is_integer_test, returns_false_for_numeric_1, "-49.23", ucBool_false)
uc_CASE(ucTok_is_integer_test, returns_false_for_numeric_2, "-1.1", ucBool_false)
uc_CASE(ucTok_is_integer_test, returns_false_for_numeric_3, "0.0", ucBool_false)
uc_CASE(ucTok_is_integer_test, returns_false_for_numeric_4, "1.0", ucBool_false)
uc_CASE(ucTok_is_integer_test, returns_false_for_numeric_5, "234.1", ucBool_false)
uc_CASE(ucTok_is_integer_test, returns_false_for_string_1, "a-4923", ucBool_false)
uc_CASE(ucTok_is_integer_test, returns_false_for_string_2, "-11b", ucBool_false)
uc_CASE(ucTok_is_integer_test, returns_false_for_string_3, "--00", ucBool_false)
uc_CASE(ucTok_is_integer_test, returns_false_for_string_4, "56    -", ucBool_false)
uc_CASE(ucTok_is_integer_test, returns_false_for_string_5, "NOT AN INT", ucBool_false)

uc_TEST(ucTok_try_parse_integer_test, const char *s, ucBool parsed, int result)
    int r;
    uc_TRUE(parsed == ucTok_try_parse_integer((ucTok*)s, &r));
    if (parsed) {
        uc_TRUE(result == r);
    }
uc_PASS
uc_CASE(ucTok_try_parse_integer_test, returns_false_if_not_parsed_1, "invalid", ucBool_false, 0)
uc_CASE(ucTok_try_parse_integer_test, returns_false_if_not_parsed_2, "--4", ucBool_false, 0)
uc_CASE(ucTok_try_parse_integer_test, returns_false_if_not_parsed_3, "-4a", ucBool_false, 0)
uc_CASE(ucTok_try_parse_integer_test, returns_false_if_not_parsed_4, "1.0", ucBool_false, 0)
uc_CASE(ucTok_try_parse_integer_test, returns_true_if_parsed_1, "43", ucBool_true, 43)
uc_CASE(ucTok_try_parse_integer_test, returns_true_if_parsed_2, "-211", ucBool_true, -211)
uc_CASE(ucTok_try_parse_integer_test, returns_true_if_parsed_3, "0", ucBool_true, 0)
uc_CASE(ucTok_try_parse_integer_test, returns_true_if_parsed_4, "1", ucBool_true, 1)

uc_TEST(ucTok_parse_integer_test, const char *s, int i)
    uc_TRUE(i == ucTok_parse_integer((ucTok*)s));
uc_PASS
uc_CASE(ucTok_parse_integer_test, returns_0_for_invalid_string_1, "--1", 0)
uc_CASE(ucTok_parse_integer_test, returns_0_for_invalid_string_2, "2.1", 0)

uc_CASE(ucTok_parse_integer_test, returns_integer_value_1, "1", 1)
uc_CASE(ucTok_parse_integer_test, returns_integer_value_2, "21", 21)
uc_CASE(ucTok_parse_integer_test, returns_integer_value_3, "-3294821", -3294821)

uc_TEST_GROUP(ucTok, NULL,
    ucTok_get_length_returns_string_length,
    ucTok_get_length_returns_zero_for_empty_token,
    ucTok_get_length_ignores_chars_trailing_separator,
    ucTok_equals_returns_true_if_strings_are_equal,
    ucTok_equals_returns_true_for_empty_strings,
    ucTok_equals_returns_false_if_strings_are_different,
    ucTok_equals_returns_false_if_value_is_null,
    ucTok_is_numeric_returns_true_for_numbers,
    ucTok_is_numeric_returns_false_for_non_numbers,
    ucTok_is_switch_returns_true_for_switches,
    ucTok_is_switch_returns_false_for_non_switches,
    ucTok_get_next_returns_next_token,
    ucTok_get_next_returns_null_if_line_is_terminated,
    ucTok_try_parse_numeric_test_does_not_parse_string,
    ucTok_try_parse_numeric_test_does_not_parse_invalid_number,
    ucTok_try_parse_numeric_test_parses_number,
    ucTok_try_parse_numeric_test_parses_negative_number,
    ucTok_try_parse_numeric_test_parses_zero_integer,
    ucTok_try_parse_numeric_test_parses_zero_with_decimal,
    ucTok_is_boolean_test_returns_true_for_1,
    ucTok_is_boolean_test_returns_true_for_on,
    ucTok_is_boolean_test_returns_true_for_yes,
    ucTok_is_boolean_test_returns_true_for_true,
    ucTok_is_boolean_test_returns_true_for_0,
    ucTok_is_boolean_test_returns_true_for_off,
    ucTok_is_boolean_test_returns_true_for_no,
    ucTok_is_boolean_test_returns_true_for_false,
    ucTok_is_boolean_test_returns_false_for_numeric,
    ucTok_is_boolean_test_returns_false_for_string,
    ucTok_try_parse_boolean_test_parses_1,
    ucTok_try_parse_boolean_test_parses_on,
    ucTok_try_parse_boolean_test_parses_yes,
    ucTok_try_parse_boolean_test_parses_true,
    ucTok_try_parse_boolean_test_parses_0,
    ucTok_try_parse_boolean_test_parses_off,
    ucTok_try_parse_boolean_test_parses_no,
    ucTok_try_parse_boolean_test_parses_false,
    ucTok_try_parse_boolean_test_does_not_parse_invalid_string,
    ucTok_try_parse_boolean_test_does_not_parse_numeric,
    ucTok_parse_numeric_test_parses_number,
    ucTok_parse_numeric_test_does_not_parse_invalid,
    ucTok_parse_boolean_test_parses_1,
    ucTok_parse_boolean_test_parses_on,
    ucTok_parse_boolean_test_parses_yes,
    ucTok_parse_boolean_test_parses_true,
    ucTok_parse_boolean_test_parses_0,
    ucTok_parse_boolean_test_parses_off,
    ucTok_parse_boolean_test_parses_no,
    ucTok_parse_boolean_test_parses_false,
    ucTok_parse_boolean_test_does_not_parses_truthy,
    ucTok_parse_boolean_test_does_not_parses_yup,
    ucTok_parse_boolean_test_does_not_parses_bzzz,
    ucTok_parse_boolean_test_does_not_parses_1_0,
    ucTok_is_integer_test_returns_true_for_int_1,
    ucTok_is_integer_test_returns_true_for_int_2,
    ucTok_is_integer_test_returns_true_for_int_3,
    ucTok_is_integer_test_returns_true_for_int_4,
    ucTok_is_integer_test_returns_true_for_int_5,
    ucTok_is_integer_test_returns_false_for_numeric_1,
    ucTok_is_integer_test_returns_false_for_numeric_2,
    ucTok_is_integer_test_returns_false_for_numeric_3,
    ucTok_is_integer_test_returns_false_for_numeric_4,
    ucTok_is_integer_test_returns_false_for_numeric_5,
    ucTok_is_integer_test_returns_false_for_string_1,
    ucTok_is_integer_test_returns_false_for_string_2,
    ucTok_is_integer_test_returns_false_for_string_3,
    ucTok_is_integer_test_returns_false_for_string_4,
    ucTok_is_integer_test_returns_false_for_string_5,
    ucTok_try_parse_integer_test_returns_false_if_not_parsed_1,
    ucTok_try_parse_integer_test_returns_false_if_not_parsed_2,
    ucTok_try_parse_integer_test_returns_false_if_not_parsed_3,
    ucTok_try_parse_integer_test_returns_false_if_not_parsed_4,
    ucTok_try_parse_integer_test_returns_true_if_parsed_1,
    ucTok_try_parse_integer_test_returns_true_if_parsed_2,
    ucTok_try_parse_integer_test_returns_true_if_parsed_3,
    ucTok_try_parse_integer_test_returns_true_if_parsed_4,
    ucTok_parse_integer_test_returns_0_for_invalid_string_1,
    ucTok_parse_integer_test_returns_0_for_invalid_string_2,
    ucTok_parse_integer_test_returns_integer_value_1,
    ucTok_parse_integer_test_returns_integer_value_2,
    ucTok_parse_integer_test_returns_integer_value_3)
