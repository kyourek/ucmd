#include <stdlib.h>
#include <string.h>
#include "cl_test.h"
#include "cl_tok.h"
#include "cl_tok_tests.h"

static cl_test_err cl_tok_get_length_returns_string_length(cl_test_group *p) {
    char s[] = { 'h', 'e', 'l', 'l', 'o', '\0' };
    cl_tok *ptr = s;
    CL_TEST_ASSERT(cl_tok_get_length(ptr) == strlen(s));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_tok_get_length_returns_zero_for_empty_token(cl_test_group *p) {
    CL_TEST_ASSERT(0 == cl_tok_get_length((cl_tok*)""));
    CL_TEST_ASSERT(0 == cl_tok_get_length((cl_tok*)NULL));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_tok_get_length_ignores_chars_trailing_separator(cl_test_group *p) {
    char s[] = { '1', '2', '3', '\0', '4', '5', '\0' };
    cl_tok *ptr = s;
    CL_TEST_ASSERT(3 == cl_tok_get_length(ptr));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_tok_get_value_returns_string_value(cl_test_group *p) {
    char s[] = { 'g', 'o', 'o', 'd', 'b', 'y', 'e', '\0' };
    cl_tok *ptr = s;
    CL_TEST_ASSERT(cl_tok_get_value(ptr));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_tok_get_value_ignores_chars_after_separator(cl_test_group *p) {
    char s[] = { 'g', 'o', 'o', 'd', '\0', 'b', 'y', 'e', '\0' };
    cl_tok *ptr = s;
    CL_TEST_ASSERT(cl_tok_get_value(ptr));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_tok_equals_returns_true_if_strings_are_equal(cl_test_group *p) {
    char *value1 = "value";
    char value2[] = { 'v', 'a', 'l', 'u', 'e', '\0', 'n', 'a', '\0' };
    cl_tok *ptr = "value";
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals(ptr, value1));
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals(ptr, value2));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_tok_equals_returns_true_for_nulls(cl_test_group *p) {
    char  *value = NULL;
    cl_tok *ptr = NULL;
    CL_TEST_ASSERT(CL_TRUE == cl_tok_equals(ptr, value));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_tok_equals_returns_false_if_strings_are_different(cl_test_group *p) {
    char *value1 = "value";
    char value2[] = { 'v', 'a', 'l', 'u', '\0', 'u', '\0' };
    cl_tok *ptr = "valuu";
    CL_TEST_ASSERT(CL_FALSE == cl_tok_equals(ptr, value1));
    CL_TEST_ASSERT(CL_FALSE == cl_tok_equals(ptr, value2));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_tok_equals_returns_false_if_either_arg_is_null(cl_test_group *p) {
    char *value = "value";
    cl_tok *ptr = NULL;
    CL_TEST_ASSERT(CL_FALSE == cl_tok_equals(ptr, value));

    ptr = value;
    value = NULL;
    CL_TEST_ASSERT(CL_FALSE == cl_tok_equals(ptr, value));

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_tok_is_numeric_returns_true_for_numbers(cl_test_group *p) {
    char n1[] = { '4', '8', '.', '9', '1', '\0', 'n', 'a', '\0' };
    CL_TEST_ASSERT(CL_TRUE == cl_tok_is_numeric((cl_tok*)"1"));
    CL_TEST_ASSERT(CL_TRUE == cl_tok_is_numeric((cl_tok*)"1.2"));
    CL_TEST_ASSERT(CL_TRUE == cl_tok_is_numeric((cl_tok*)"87"));
    CL_TEST_ASSERT(CL_TRUE == cl_tok_is_numeric((cl_tok*)"-1"));
    CL_TEST_ASSERT(CL_TRUE == cl_tok_is_numeric((cl_tok*)n1));
    CL_TEST_ASSERT(CL_TRUE == cl_tok_is_numeric((cl_tok*)"-543.234610"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_tok_is_numeric_returns_false_for_non_numbers(cl_test_group *p) {
    char c1[] = { '4', '8', '.', '9', '1', 'n', 'a', '\0' };
    CL_TEST_ASSERT(CL_FALSE == cl_tok_is_numeric((cl_tok*)"a"));
    CL_TEST_ASSERT(CL_FALSE == cl_tok_is_numeric((cl_tok*)"1234_"));
    CL_TEST_ASSERT(CL_FALSE == cl_tok_is_numeric((cl_tok*)"1_234"));
    CL_TEST_ASSERT(CL_FALSE == cl_tok_is_numeric((cl_tok*)c1));
    CL_TEST_ASSERT(CL_FALSE == cl_tok_is_numeric((cl_tok*)NULL));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_tok_is_switch_returns_true_for_switches(cl_test_group *p) {
    char s1[] = { '-', 's', '1', '\0', 'n', 'a', '\0' };
    CL_TEST_ASSERT(CL_TRUE == cl_tok_is_switch((cl_tok*)"-switch"));
    CL_TEST_ASSERT(CL_TRUE == cl_tok_is_switch((cl_tok*)"--double"));
    CL_TEST_ASSERT(CL_TRUE == cl_tok_is_switch((cl_tok*)"-em-bed"));
    CL_TEST_ASSERT(CL_TRUE == cl_tok_is_switch((cl_tok*)s1));
    CL_TEST_ASSERT(CL_TRUE == cl_tok_is_switch((cl_tok*)"-1.23.4"));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_tok_is_switch_returns_false_for_non_switches(cl_test_group *p) {
    char a1[] = { 'a', 'r', 'g', '-', 'd', 'a', 's', 'h', '\0' };
    CL_TEST_ASSERT(CL_FALSE == cl_tok_is_switch((cl_tok*)"arg"));
    CL_TEST_ASSERT(CL_FALSE == cl_tok_is_switch((cl_tok*)"e-mbed"));
    CL_TEST_ASSERT(CL_FALSE == cl_tok_is_switch((cl_tok*)"-1"));
    CL_TEST_ASSERT(CL_FALSE == cl_tok_is_switch((cl_tok*)"-1.234"));
    CL_TEST_ASSERT(CL_FALSE == cl_tok_is_switch((cl_tok*)a1));
    CL_TEST_ASSERT(CL_FALSE == cl_tok_is_switch((cl_tok*)NULL));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_tok_get_next_returns_next_token(cl_test_group *p) {
    CL_TEST_ASSERT(cl_tok_equals(cl_tok_get_next((cl_tok*)"t1\0t2"), "t2"));
    CL_TEST_ASSERT(cl_tok_equals(cl_tok_get_next((cl_tok*)"t1\0-s1\0t2"), "-s1"));
    CL_TEST_ASSERT(cl_tok_equals(cl_tok_get_next((cl_tok*)"a1\0\na2"), NULL));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_tok_count_counts_tokens(cl_test_group *p) {
    CL_TEST_ASSERT(0 == cl_tok_count((cl_tok*)NULL));
    CL_TEST_ASSERT(0 == cl_tok_count((cl_tok*)"\n"));
    CL_TEST_ASSERT(0 == cl_tok_count((cl_tok*)"\0\0\0\n"));
    CL_TEST_ASSERT(1 == cl_tok_count((cl_tok*)"a1\0\n"));
    CL_TEST_ASSERT(1 == cl_tok_count((cl_tok*)"-s1\0\n"));
    CL_TEST_ASSERT(3 == cl_tok_count((cl_tok*)"tok en\0a1\0-s1\0\nNA"));
    return CL_TEST_ERR_NONE;
}

cl_test_group *cl_tok_tests_get_group(void) {
    static cl_test_group group;
    static cl_test_group_test_func *tests[] = {
        cl_tok_get_length_returns_string_length,
        cl_tok_get_length_returns_zero_for_empty_token,
        cl_tok_get_length_ignores_chars_trailing_separator,
        cl_tok_get_value_returns_string_value,
        cl_tok_get_value_ignores_chars_after_separator,
        cl_tok_equals_returns_true_if_strings_are_equal,
        cl_tok_equals_returns_true_for_nulls,
        cl_tok_equals_returns_false_if_strings_are_different,
        cl_tok_equals_returns_false_if_either_arg_is_null,
        cl_tok_is_numeric_returns_true_for_numbers,
        cl_tok_is_numeric_returns_false_for_non_numbers,
        cl_tok_is_numeric_returns_false_for_non_numbers,
        cl_tok_is_switch_returns_true_for_switches,
        cl_tok_is_switch_returns_false_for_non_switches,
        cl_tok_get_next_returns_next_token,
        cl_tok_count_counts_tokens,
        NULL
    };

    return cl_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
