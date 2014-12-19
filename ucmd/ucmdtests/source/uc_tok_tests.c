#include <stdlib.h>
#include <string.h>
#include "uc_test.h"
#include "uc_tok.h"
#include "uc_tok_tests.h"

static uc_test_err uc_tok_get_length_returns_string_length(uc_test_group *p) {
    char s[] = { 'h', 'e', 'l', 'l', 'o', '\0' };
    uc_tok *ptr = s;
    UC_TEST_ASSERT(uc_tok_get_length(ptr) == strlen(s));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_tok_get_length_returns_zero_for_empty_token(uc_test_group *p) {
    UC_TEST_ASSERT(0 == uc_tok_get_length((uc_tok*)""));
    UC_TEST_ASSERT(0 == uc_tok_get_length((uc_tok*)NULL));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_tok_get_length_ignores_chars_trailing_separator(uc_test_group *p) {
    char s[] = { '1', '2', '3', '\0', '4', '5', '\0' };
    uc_tok *ptr = s;
    UC_TEST_ASSERT(3 == uc_tok_get_length(ptr));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_tok_get_value_returns_string_value(uc_test_group *p) {
    char s[] = { 'g', 'o', 'o', 'd', 'b', 'y', 'e', '\0' };
    uc_tok *ptr = s;
    UC_TEST_ASSERT(uc_tok_get_value(ptr));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_tok_get_value_ignores_chars_after_separator(uc_test_group *p) {
    char s[] = { 'g', 'o', 'o', 'd', '\0', 'b', 'y', 'e', '\0' };
    uc_tok *ptr = s;
    UC_TEST_ASSERT(uc_tok_get_value(ptr));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_tok_equals_returns_true_if_strings_are_equal(uc_test_group *p) {
    char *value1 = "value";
    char value2[] = { 'v', 'a', 'l', 'u', 'e', '\0', 'n', 'a', '\0' };
    uc_tok *ptr = "value";
    UC_TEST_ASSERT(ucBool_true == uc_tok_equals(ptr, value1));
    UC_TEST_ASSERT(ucBool_true == uc_tok_equals(ptr, value2));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_tok_equals_returns_true_for_nulls(uc_test_group *p) {
    char  *value = NULL;
    uc_tok *ptr = NULL;
    UC_TEST_ASSERT(ucBool_true == uc_tok_equals(ptr, value));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_tok_equals_returns_false_if_strings_are_different(uc_test_group *p) {
    char *value1 = "value";
    char value2[] = { 'v', 'a', 'l', 'u', '\0', 'u', '\0' };
    uc_tok *ptr = "valuu";
    UC_TEST_ASSERT(ucBool_false == uc_tok_equals(ptr, value1));
    UC_TEST_ASSERT(ucBool_false == uc_tok_equals(ptr, value2));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_tok_equals_returns_false_if_either_arg_is_null(uc_test_group *p) {
    char *value = "value";
    uc_tok *ptr = NULL;
    UC_TEST_ASSERT(ucBool_false == uc_tok_equals(ptr, value));

    ptr = value;
    value = NULL;
    UC_TEST_ASSERT(ucBool_false == uc_tok_equals(ptr, value));

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_tok_is_numeric_returns_true_for_numbers(uc_test_group *p) {
    char n1[] = { '4', '8', '.', '9', '1', '\0', 'n', 'a', '\0' };
    UC_TEST_ASSERT(ucBool_true == uc_tok_is_numeric((uc_tok*)"1"));
    UC_TEST_ASSERT(ucBool_true == uc_tok_is_numeric((uc_tok*)"1.2"));
    UC_TEST_ASSERT(ucBool_true == uc_tok_is_numeric((uc_tok*)"87"));
    UC_TEST_ASSERT(ucBool_true == uc_tok_is_numeric((uc_tok*)"-1"));
    UC_TEST_ASSERT(ucBool_true == uc_tok_is_numeric((uc_tok*)n1));
    UC_TEST_ASSERT(ucBool_true == uc_tok_is_numeric((uc_tok*)"-543.234610"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_tok_is_numeric_returns_false_for_non_numbers(uc_test_group *p) {
    char c1[] = { '4', '8', '.', '9', '1', 'n', 'a', '\0' };
    UC_TEST_ASSERT(ucBool_false == uc_tok_is_numeric((uc_tok*)"a"));
    UC_TEST_ASSERT(ucBool_false == uc_tok_is_numeric((uc_tok*)"1234_"));
    UC_TEST_ASSERT(ucBool_false == uc_tok_is_numeric((uc_tok*)"1_234"));
    UC_TEST_ASSERT(ucBool_false == uc_tok_is_numeric((uc_tok*)c1));
    UC_TEST_ASSERT(ucBool_false == uc_tok_is_numeric((uc_tok*)NULL));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_tok_is_switch_returns_true_for_switches(uc_test_group *p) {
    char s1[] = { '-', 's', '1', '\0', 'n', 'a', '\0' };
    UC_TEST_ASSERT(ucBool_true == uc_tok_is_switch((uc_tok*)"-switch"));
    UC_TEST_ASSERT(ucBool_true == uc_tok_is_switch((uc_tok*)"--double"));
    UC_TEST_ASSERT(ucBool_true == uc_tok_is_switch((uc_tok*)"-em-bed"));
    UC_TEST_ASSERT(ucBool_true == uc_tok_is_switch((uc_tok*)s1));
    UC_TEST_ASSERT(ucBool_true == uc_tok_is_switch((uc_tok*)"-1.23.4"));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_tok_is_switch_returns_false_for_non_switches(uc_test_group *p) {
    char a1[] = { 'a', 'r', 'g', '-', 'd', 'a', 's', 'h', '\0' };
    UC_TEST_ASSERT(ucBool_false == uc_tok_is_switch((uc_tok*)"arg"));
    UC_TEST_ASSERT(ucBool_false == uc_tok_is_switch((uc_tok*)"e-mbed"));
    UC_TEST_ASSERT(ucBool_false == uc_tok_is_switch((uc_tok*)"-1"));
    UC_TEST_ASSERT(ucBool_false == uc_tok_is_switch((uc_tok*)"-1.234"));
    UC_TEST_ASSERT(ucBool_false == uc_tok_is_switch((uc_tok*)a1));
    UC_TEST_ASSERT(ucBool_false == uc_tok_is_switch((uc_tok*)NULL));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_tok_get_next_returns_next_token(uc_test_group *p) {
    UC_TEST_ASSERT(uc_tok_equals(uc_tok_get_next((uc_tok*)"t1\0t2"), "t2"));
    UC_TEST_ASSERT(uc_tok_equals(uc_tok_get_next((uc_tok*)"t1\0-s1\0t2"), "-s1"));
    UC_TEST_ASSERT(uc_tok_equals(uc_tok_get_next((uc_tok*)"a1\0\na2"), NULL));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_tok_count_counts_tokens(uc_test_group *p) {
    UC_TEST_ASSERT(0 == uc_tok_count((uc_tok*)NULL));
    UC_TEST_ASSERT(0 == uc_tok_count((uc_tok*)"\n"));
    UC_TEST_ASSERT(0 == uc_tok_count((uc_tok*)"\0\0\0\n"));
    UC_TEST_ASSERT(1 == uc_tok_count((uc_tok*)"a1\0\n"));
    UC_TEST_ASSERT(1 == uc_tok_count((uc_tok*)"-s1\0\n"));
    UC_TEST_ASSERT(3 == uc_tok_count((uc_tok*)"tok en\0a1\0-s1\0\nNA"));
    return UC_TEST_ERR_NONE;
}

uc_test_group *uc_tok_tests_get_group(void) {
    static uc_test_group group;
    static uc_test_group_test_func *tests[] = {
        uc_tok_get_length_returns_string_length,
        uc_tok_get_length_returns_zero_for_empty_token,
        uc_tok_get_length_ignores_chars_trailing_separator,
        uc_tok_get_value_returns_string_value,
        uc_tok_get_value_ignores_chars_after_separator,
        uc_tok_equals_returns_true_if_strings_are_equal,
        uc_tok_equals_returns_true_for_nulls,
        uc_tok_equals_returns_false_if_strings_are_different,
        uc_tok_equals_returns_false_if_either_arg_is_null,
        uc_tok_is_numeric_returns_true_for_numbers,
        uc_tok_is_numeric_returns_false_for_non_numbers,
        uc_tok_is_numeric_returns_false_for_non_numbers,
        uc_tok_is_switch_returns_true_for_switches,
        uc_tok_is_switch_returns_false_for_non_switches,
        uc_tok_get_next_returns_next_token,
        uc_tok_count_counts_tokens,
        NULL
    };

    return uc_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
