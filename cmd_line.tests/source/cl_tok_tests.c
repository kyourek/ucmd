#include <stdlib.h>
#include <string.h>
#include "cl_tests.h"
#include "cl_tests_p.h"
#include "cl_tok.h"
#include "cl_tok_tests.h"

static CL_TESTS_ERR cl_tok_get_length_returns_string_length(void) {
    char s[] = { 'h', 'e', 'l', 'l', 'o', '\0' };
    cl_tok *p = s;
    CL_TESTS_ASSERT(cl_tok_get_length(p) == strlen(s));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_tok_get_length_returns_zero_for_empty_token(void) {
    CL_TESTS_ASSERT(0 == cl_tok_get_length((cl_tok*)""));
    CL_TESTS_ASSERT(0 == cl_tok_get_length((cl_tok*)NULL));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_tok_get_length_ignores_chars_trailing_separator(void) {
    char s[] = { '1', '2', '3', '\0', '4', '5', '\0' };
    cl_tok *p = s;
    CL_TESTS_ASSERT(3 == cl_tok_get_length(p));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_tok_get_value_returns_string_value(void) {
    char s[] = { 'g', 'o', 'o', 'd', 'b', 'y', 'e', '\0' };
    cl_tok *p = s;
    CL_TESTS_ASSERT(cl_tok_get_value(p));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_tok_get_value_ignores_chars_after_separator(void) {
    char s[] = { 'g', 'o', 'o', 'd', '\0', 'b', 'y', 'e', '\0' };
    cl_tok *p = s;
    CL_TESTS_ASSERT(cl_tok_get_value(p));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_tok_equals_returns_true_if_strings_are_equal(void) {
    char *value1 = "value";
    char value2[] = { 'v', 'a', 'l', 'u', 'e', '\0', 'n', 'a', '\0' };
    cl_tok *p = "value";
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_equals(p, value1));
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_equals(p, value2));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_tok_equals_returns_true_for_nulls(void) {
    char  *value = NULL;
    cl_tok *p = NULL;
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_equals(p, value));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_tok_equals_returns_false_if_strings_are_different(void) {
    char *value1 = "value";
    char value2[] = { 'v', 'a', 'l', 'u', '\0', 'u', '\0' };
    cl_tok *p = "valuu";
    CL_TESTS_ASSERT(CL_FALSE == cl_tok_equals(p, value1));
    CL_TESTS_ASSERT(CL_FALSE == cl_tok_equals(p, value2));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_tok_equals_returns_false_if_either_arg_is_null(void) {
    char *value = "value";
    cl_tok *p = NULL;
    CL_TESTS_ASSERT(CL_FALSE == cl_tok_equals(p, value));

    p = value;
    value = NULL;
    CL_TESTS_ASSERT(CL_FALSE == cl_tok_equals(p, value));

    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_tok_is_numeric_returns_true_for_numbers(void) {
    char n1[] = { '4', '8', '.', '9', '1', '\0', 'n', 'a', '\0' };
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_is_numeric((cl_tok*)"1"));
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_is_numeric((cl_tok*)"1.2"));
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_is_numeric((cl_tok*)"87"));
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_is_numeric((cl_tok*)"-1"));
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_is_numeric((cl_tok*)n1));
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_is_numeric((cl_tok*)"-543.234610"));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_tok_is_numeric_returns_false_for_non_numbers(void) {
    char c1[] = { '4', '8', '.', '9', '1', 'n', 'a', '\0' };
    CL_TESTS_ASSERT(CL_FALSE == cl_tok_is_numeric((cl_tok*)"a"));
    CL_TESTS_ASSERT(CL_FALSE == cl_tok_is_numeric((cl_tok*)"1234_"));
    CL_TESTS_ASSERT(CL_FALSE == cl_tok_is_numeric((cl_tok*)"1_234"));
    CL_TESTS_ASSERT(CL_FALSE == cl_tok_is_numeric((cl_tok*)c1));
    CL_TESTS_ASSERT(CL_FALSE == cl_tok_is_numeric((cl_tok*)NULL));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_tok_is_switch_returns_true_for_switches(void) {
    char s1[] = { '-', 's', '1', '\0', 'n', 'a', '\0' };
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_is_switch((cl_tok*)"-switch"));
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_is_switch((cl_tok*)"--double"));
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_is_switch((cl_tok*)"-em-bed"));
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_is_switch((cl_tok*)s1));
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_is_switch((cl_tok*)"-1.23.4"));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_tok_is_switch_returns_false_for_non_switches(void) {
    char a1[] = { 'a', 'r', 'g', '-', 'd', 'a', 's', 'h', '\0' };
    CL_TESTS_ASSERT(CL_FALSE == cl_tok_is_switch((cl_tok*)"arg"));
    CL_TESTS_ASSERT(CL_FALSE == cl_tok_is_switch((cl_tok*)"e-mbed"));
    CL_TESTS_ASSERT(CL_FALSE == cl_tok_is_switch((cl_tok*)"-1"));
    CL_TESTS_ASSERT(CL_FALSE == cl_tok_is_switch((cl_tok*)"-1.234"));
    CL_TESTS_ASSERT(CL_FALSE == cl_tok_is_switch((cl_tok*)a1));
    CL_TESTS_ASSERT(CL_FALSE == cl_tok_is_switch((cl_tok*)NULL));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_tok_get_next_returns_next_token(void) {
    CL_TESTS_ASSERT(cl_tok_equals(cl_tok_get_next((cl_tok*)"t1\0t2"), "t2"));
    CL_TESTS_ASSERT(cl_tok_equals(cl_tok_get_next((cl_tok*)"t1\0-s1\0t2"), "-s1"));
    CL_TESTS_ASSERT(cl_tok_equals(cl_tok_get_next((cl_tok*)"a1\0\na2"), NULL));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_tok_count_counts_tokens(void) {
    CL_TESTS_ASSERT(0 == cl_tok_count((cl_tok*)NULL));
    CL_TESTS_ASSERT(0 == cl_tok_count((cl_tok*)"\n"));
    CL_TESTS_ASSERT(0 == cl_tok_count((cl_tok*)"\0\0\0\n"));
    CL_TESTS_ASSERT(1 == cl_tok_count((cl_tok*)"a1\0\n"));
    CL_TESTS_ASSERT(1 == cl_tok_count((cl_tok*)"-s1\0\n"));
    CL_TESTS_ASSERT(3 == cl_tok_count((cl_tok*)"tok en\0a1\0-s1\0\nNA"));
    return CL_TESTS_NO_ERR;
}

CL_TESTS_ERR cl_tok_tests(void) {
    CL_TESTS_RUN(cl_tok_get_length_returns_string_length);
    CL_TESTS_RUN(cl_tok_get_length_returns_zero_for_empty_token);
    CL_TESTS_RUN(cl_tok_get_length_ignores_chars_trailing_separator);
    CL_TESTS_RUN(cl_tok_get_value_returns_string_value);
    CL_TESTS_RUN(cl_tok_get_value_ignores_chars_after_separator);
    CL_TESTS_RUN(cl_tok_equals_returns_true_if_strings_are_equal);
    CL_TESTS_RUN(cl_tok_equals_returns_true_for_nulls);
    CL_TESTS_RUN(cl_tok_equals_returns_false_if_strings_are_different);
    CL_TESTS_RUN(cl_tok_equals_returns_false_if_either_arg_is_null);
    CL_TESTS_RUN(cl_tok_is_numeric_returns_true_for_numbers);
    CL_TESTS_RUN(cl_tok_is_numeric_returns_false_for_non_numbers);
    CL_TESTS_RUN(cl_tok_is_numeric_returns_false_for_non_numbers);
    CL_TESTS_RUN(cl_tok_is_switch_returns_true_for_switches);
    CL_TESTS_RUN(cl_tok_is_switch_returns_false_for_non_switches);
    CL_TESTS_RUN(cl_tok_get_next_returns_next_token);
    CL_TESTS_RUN(cl_tok_count_counts_tokens);
    return CL_TESTS_NO_ERR;
}
