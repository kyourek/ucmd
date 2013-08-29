#include <stdlib.h>
#include <string.h>
#include "cl_tests.h"
#include "cl_tok.h"
#include "cl_tok_tests.h"

char *cl_tok_get_length_returns_string_length(void) {
    char s[] = { 'h', 'e', 'l', 'l', 'o', '\0' };
    cl_tok *p = s;
    cl_assert("cl_tok_get_length did not count characters.", cl_tok_get_length(p) == strlen(s));
    return 0;
}

char *cl_tok_get_length_returns_zero_for_empty_token(void) {
    char *message = "cl_tok_get_length should have returned 0.";
    cl_assert(message, 0 == cl_tok_get_length((cl_tok*)""));
    cl_assert(message, 0 == cl_tok_get_length((cl_tok*)NULL));
    return 0;
}

char *cl_tok_get_length_ignores_chars_trailing_separator(void) {
    char s[] = { '1', '2', '3', '\0', '4', '5', '\0' };
    cl_tok *p = s;
    cl_assert("cl_tok_get_length counted chars after separator.", 3 == cl_tok_get_length(p));
    return 0;
}

char *cl_tok_get_value_returns_string_value(void) {
    char s[] = { 'g', 'o', 'o', 'd', 'b', 'y', 'e', '\0' };
    cl_tok *p = s;
    cl_assert("cl_tok_get_value did not return string value.", 0 == strcmp("goodbye", cl_tok_get_value(p)));
    return 0;
}

char *cl_tok_get_value_ignores_chars_after_separator(void) {
    char s[] = { 'g', 'o', 'o', 'd', '\0', 'b', 'y', 'e', '\0' };
    cl_tok *p = s;
    cl_assert("cl_tok_get_value did not return string value.", 0 == strcmp("good", cl_tok_get_value(p)));
    return 0;
}

char *cl_tok_equals_returns_true_if_strings_are_equal(void) {
    char *value1 = "value";
    char value2[] = { 'v', 'a', 'l', 'u', 'e', '\0', 'n', 'a', '\0' };
    cl_tok *p = "value";
    cl_assert("cl_tok_equals returned false for equal string.", CL_TRUE == cl_tok_equals(p, value1));
    cl_assert("cl_tok_equals returned false for equal string.", CL_TRUE == cl_tok_equals(p, value2));
    return 0;
}

char *cl_tok_equals_returns_true_for_nulls(void) {
    char *value = NULL;
    cl_tok *p = NULL;
    cl_assert("cl_tok_equals returned false for two null arguments.", CL_TRUE == cl_tok_equals(p, value));
    return 0;
}

char *cl_tok_equals_returns_false_if_strings_are_different(void) {
    char *value1 = "value";
    char value2[] = { 'v', 'a', 'l', 'u', '\0', 'u', '\0' };
    cl_tok *p = "valuu";
    cl_assert("cl_tok_equals returned true for non-equal strings.", CL_FALSE == cl_tok_equals(p, value1));
    cl_assert("cl_tok_equals returned true for non-equal strings.", CL_FALSE == cl_tok_equals(p, value2));
    return 0;
}

char *cl_tok_equals_returns_false_if_either_arg_is_null(void) {
    char *value = "value";
    cl_tok *p = NULL;
    cl_assert("cl_tok_equals returned true for null arg.", CL_FALSE == cl_tok_equals(p, value));

    p = value;
    value = NULL;
    cl_assert("cl_tok_equals returned true for null arg.", CL_FALSE == cl_tok_equals(p, value));

    return 0;
}

char *cl_tok_is_numeric_returns_true_for_numbers(void) {
    char *message = "cl_tok_is_numeric returned false for number.";
    char n1[] = { '4', '8', '.', '9', '1', '\0', 'n', 'a', '\0' };
    cl_assert(message, CL_TRUE == cl_tok_is_numeric((cl_tok*)"1"));
    cl_assert(message, CL_TRUE == cl_tok_is_numeric((cl_tok*)"1.2"));
    cl_assert(message, CL_TRUE == cl_tok_is_numeric((cl_tok*)"87"));
    cl_assert(message, CL_TRUE == cl_tok_is_numeric((cl_tok*)"-1"));
    cl_assert(message, CL_TRUE == cl_tok_is_numeric((cl_tok*)n1));
    cl_assert(message, CL_TRUE == cl_tok_is_numeric((cl_tok*)"-543.234610"));
    return 0;
}

char *cl_tok_is_numeric_returns_false_for_non_numbers(void) {
    char *message = "cl_tok_is_numeric returned true for non-number.";
    char c1[] = { '4', '8', '.', '9', '1', 'n', 'a', '\0' };
    cl_assert(message, CL_FALSE == cl_tok_is_numeric((cl_tok*)"a"));
    cl_assert(message, CL_FALSE == cl_tok_is_numeric((cl_tok*)"1234_"));
    cl_assert(message, CL_FALSE == cl_tok_is_numeric((cl_tok*)"1_234"));
    cl_assert(message, CL_FALSE == cl_tok_is_numeric((cl_tok*)c1));
    cl_assert(message, CL_FALSE == cl_tok_is_numeric((cl_tok*)NULL));
    return 0;
}

char *cl_tok_is_switch_returns_true_for_switches(void) {
    char *message = "cl_tok_is_switch returned false for switch.";
    char s1[] = { '-', 's', '1', '\0', 'n', 'a', '\0' };
    cl_assert(message, CL_TRUE == cl_tok_is_switch((cl_tok*)"-switch"));
    cl_assert(message, CL_TRUE == cl_tok_is_switch((cl_tok*)"--double"));
    cl_assert(message, CL_TRUE == cl_tok_is_switch((cl_tok*)"-em-bed"));
    cl_assert(message, CL_TRUE == cl_tok_is_switch((cl_tok*)s1));
    cl_assert(message, CL_TRUE == cl_tok_is_switch((cl_tok*)"-1.23.4"));
    return 0;
}

char *cl_tok_is_switch_returns_false_for_non_switches(void) {
    char *message = "cl_tok_is_switch returned true for non-switch.";
    char a1[] = { 'a', 'r', 'g', '-', 'd', 'a', 's', 'h', '\0' };
    cl_assert(message, CL_FALSE == cl_tok_is_switch((cl_tok*)"arg"));
    cl_assert(message, CL_FALSE == cl_tok_is_switch((cl_tok*)"e-mbed"));
    cl_assert(message, CL_FALSE == cl_tok_is_switch((cl_tok*)"-1"));
    cl_assert(message, CL_FALSE == cl_tok_is_switch((cl_tok*)"-1.234"));
    cl_assert(message, CL_FALSE == cl_tok_is_switch((cl_tok*)a1));
    cl_assert(message, CL_FALSE == cl_tok_is_switch((cl_tok*)NULL));
    return 0;
}

char *cl_tok_get_next_returns_next_token(void) {
    char *message = "cl_tok_get_next did not return next token.";
    cl_assert(message, cl_tok_equals(cl_tok_get_next((cl_tok*)"t1\0t2"), "t2"));
    cl_assert(message, cl_tok_equals(cl_tok_get_next((cl_tok*)"t1\0-s1\0t2"), "-s1"));
    cl_assert(message, cl_tok_equals(cl_tok_get_next((cl_tok*)"a1\0\na2"), NULL));
    return 0;
}

char *cl_tok_count_counts_tokens(void) {
    char *message = "cl_tok_count did not count tokens.";
    cl_assert(message, 0 == cl_tok_count((cl_tok*)NULL));
    cl_assert(message, 0 == cl_tok_count((cl_tok*)"\n"));
    cl_assert(message, 0 == cl_tok_count((cl_tok*)"\0\0\0\n"));
    cl_assert(message, 1 == cl_tok_count((cl_tok*)"a1\0\n"));
    cl_assert(message, 1 == cl_tok_count((cl_tok*)"-s1\0\n"));
    cl_assert(message, 3 == cl_tok_count((cl_tok*)"tok en\0a1\0-s1\0\nNA"));
    return 0;
}

char *cl_tok_tests(void) {
    cl_run_test(cl_tok_get_length_returns_string_length);
    cl_run_test(cl_tok_get_length_returns_zero_for_empty_token);
    cl_run_test(cl_tok_get_length_ignores_chars_trailing_separator);
    cl_run_test(cl_tok_get_value_returns_string_value);
    cl_run_test(cl_tok_get_value_ignores_chars_after_separator);
    cl_run_test(cl_tok_equals_returns_true_if_strings_are_equal);
    cl_run_test(cl_tok_equals_returns_true_for_nulls);
    cl_run_test(cl_tok_equals_returns_false_if_strings_are_different);
    cl_run_test(cl_tok_equals_returns_false_if_either_arg_is_null);
    cl_run_test(cl_tok_is_numeric_returns_true_for_numbers);
    cl_run_test(cl_tok_is_numeric_returns_false_for_non_numbers);
    cl_run_test(cl_tok_is_numeric_returns_false_for_non_numbers);
    cl_run_test(cl_tok_is_switch_returns_true_for_switches);
    cl_run_test(cl_tok_is_switch_returns_false_for_non_switches);
    cl_run_test(cl_tok_get_next_returns_next_token);
    cl_run_test(cl_tok_count_counts_tokens);
    return 0;
}
