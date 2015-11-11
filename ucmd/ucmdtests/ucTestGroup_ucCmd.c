#include <stdio.h>
#include <stdarg.h>
#include "ucmdtests.h"

static ucCmd *subject;

static void *transmit_1_state;
static const char *transmit_1_response;
static void transmit_1(const char *response, void *state) {
    transmit_1_state = state;
    transmit_1_response = response;
}

static void *is_canceled_1_state;
static ucBool is_canceled_1_returned_value = ucBool_false;
static ucBool is_canceled_1(void *state) {
    is_canceled_1_state = state;
    return is_canceled_1_returned_value;
}

static ucBool is_canceled_2(void *state) {
    return ucBool_true;
}

static void *handle_invalid_command_1_state;
static const char *handle_invalid_command_1_invalid_command;
static ucBool handle_invalid_command_1_return = ucBool_false;
static ucBool handle_invalid_command_1(const char *invalid_command, void *state) {
    handle_invalid_command_1_state = state;
    handle_invalid_command_1_invalid_command = invalid_command;
    return handle_invalid_command_1_return;
}

uc_TEST(prior)
    subject = ucCmd_create();
    assert(subject);
uc_PASS

uc_TEST(after)
    ucCmd_destroy(subject);
uc_PASS

uc_TEST(setup)
    ucTestGroup_setup_test(p, prior, after);
uc_PASS

uc_TEST(ucCmd_get_command_returns_cmd_tok)
    ucCmd c;
    ucCmdTok *t = "token";
    c.command = t;
    uc_TRUE(t == ucCmd_get_command(&c));
uc_PASS

uc_TEST(ucCmd_set_command_sets_cmd_tok)
    ucCmd c;
    ucCmdTok *t = "cmd_tok";
    ucCmd_set_command(&c, t);
    uc_TRUE(t == c.command);
uc_PASS

uc_TEST(ucCmd_create_does_not_return_null)
    uc_TRUE(NULL != subject);
uc_PASS

uc_TEST(ucCmd_format_response_sets_response_string)
    char expected[50];
    ucCmd *ptr = subject;
    ucCmd_format_response(ptr, "formatted %s %d %3.2f fin", "string", 10, 5.1234);
    sprintf(expected, "formatted %s %d %3.2f fin", "string", 10, 5.1234);
    uc_TRUE(0 == strcmp(ptr->response, expected));
uc_PASS

static const char *ucCmd_format_response_va_sets_response_string_helper(ucCmd *p, const char *format, ...) {
    va_list arg_list;
    const char *response;
    va_start(arg_list, format);
    response = ucCmd_format_response_va(p, format, arg_list);
    va_end(arg_list);
    return response;
}

uc_TEST(ucCmd_format_response_va_sets_response_string)
    char expected[50];
    const char *actual;
    ucCmd *ptr = subject;
    
    actual = ucCmd_format_response_va_sets_response_string_helper(ptr, "This %d is %s formatted.", 1, "well");
    sprintf(expected, "This %d is %s formatted.", 1, "well");
    uc_TRUE(0 == strcmp(actual, expected));
    uc_TRUE(0 == strcmp(ptr->response, expected));
uc_PASS

uc_TEST(ucCmd_set_transmit_sets_transmit)
    ucCmd c;
    ucCmd_set_transmit(&c, transmit_1);
    uc_TRUE(transmit_1 == c.transmit);
uc_PASS

uc_TEST(ucCmd_respond_uses_transmit)
    ucCmd c;
    c.is_quiet = ucBool_false;
    c.transmit = transmit_1;

    transmit_1_response = NULL;
    c.transmit = transmit_1;
    ucCmd_respond(&c, "expected response");
    uc_TRUE(0 == strcmp(transmit_1_response, "expected response"));
uc_PASS

uc_TEST(ucCmd_set_is_canceled_sets_value)
    ucCmd c;
    ucCmd_set_is_canceled(&c, is_canceled_2);
    uc_TRUE(is_canceled_2 == c.is_canceled);
uc_PASS

uc_TEST(ucCmd_is_canceled_calls_is_canceled)
    ucCmd c;
    c.is_canceled = is_canceled_1;

    is_canceled_1_returned_value = ucBool_true;
    uc_TRUE(ucBool_true == ucCmd_is_canceled(&c));

    is_canceled_1_returned_value = ucBool_false;
    uc_TRUE(ucBool_false == ucCmd_is_canceled(&c));
uc_PASS

uc_TEST(ucCmd_set_transmit_state_sets_value)
    int state;
    ucCmd *ptr = subject;
    void *prev_value = ptr->transmit_state;

    ucCmd_set_transmit_state(ptr, &state);
    uc_TRUE(&state == ptr->transmit_state);

    ptr->transmit_state = prev_value;
uc_PASS

uc_TEST(ucCmd_transmit_uses_state)
    double state;
    ucCmd *ptr = subject;
    ucCmd_TransmitFunc *prev_func = ptr->transmit;
    void *prev_state = ptr->transmit_state;

    ucCmd_set_transmit(ptr, transmit_1);
    ucCmd_set_transmit_state(ptr, &state);

    ucCmd_respond(ptr, "resp");

    uc_TRUE(&state == transmit_1_state);

    ptr->transmit = prev_func;
    ptr->transmit_state = prev_state;
uc_PASS

uc_TEST(ucCmd_set_is_canceled_state_sets_value)
    int state;
    ucCmd *ptr = subject;
    void *prev_value = ptr->is_canceled_state;

    ucCmd_set_is_canceled_state(ptr, &state);
    uc_TRUE(&state == ptr->is_canceled_state);

    ptr->is_canceled_state = prev_value;
uc_PASS

uc_TEST(ucCmd_is_canceled_uses_state)
    double state;
    ucCmd *ptr = subject;
    void *prev_value = ptr->is_canceled_state;

    ucCmd_set_is_canceled(ptr, is_canceled_1);
    ucCmd_set_is_canceled_state(ptr, &state);

    ucCmd_is_canceled(ptr);

    uc_TRUE(&state == is_canceled_1_state);

    ptr->is_canceled_state = prev_value;
uc_PASS

uc_TEST(ucCmd_set_is_quiet_sets_value)
    ucCmd *ptr = subject;
    
    ucBool prev_val = ucCmd_get_is_quiet(ptr);
    uc_TRUE(ucBool_false == prev_val);

    ucCmd_set_is_quiet(ptr, ucBool_true);
    uc_TRUE(ucCmd_get_is_quiet(ptr) == ucBool_true);

    ucCmd_set_is_quiet(ptr, prev_val);
    uc_TRUE(ucCmd_get_is_quiet(ptr) == prev_val);
uc_PASS

uc_TEST(ucCmd_respond_does_nothing_if_is_quiet)
    ucCmd *ptr = subject;
    ucBool pre_val = ucCmd_get_is_quiet(ptr);

    ucCmd_set_is_quiet(ptr, ucBool_true);
    ucCmd_set_transmit(ptr, transmit_1);

    transmit_1_response = NULL;
    ucCmd_respond(ptr, "expected response");
    uc_TRUE(NULL == transmit_1_response);

    ucCmd_set_is_quiet(ptr, pre_val);
uc_PASS

uc_TEST(ucCmd_set_handle_invalid_command_sets_callback)
    int state;
    ucBool ret_val;
    ucCmd *ptr = subject;
    ucCmd_set_handle_invalid_command(ptr, handle_invalid_command_1);
    ucCmd_set_handle_invalid_command_state(ptr, &state);

    handle_invalid_command_1_return = ucBool_true;
    ret_val = ucCmd_handle_invalid_command(ptr, "n-a");
    uc_TRUE(handle_invalid_command_1_state == &state);
    uc_TRUE(0 == strcmp("n-a", handle_invalid_command_1_invalid_command));
    uc_TRUE(ucBool_true == ret_val);
uc_PASS

uc_TEST(ucCmd_get_response_length_max_returns_size)
    ucCmd *ptr = subject;
    int size = ucCmd_get_response_length_max(ptr);
    uc_TRUE(ucCmd_RESPONSE_SIZE == size);
uc_PASS

uc_TEST(ucCmd_get_response_terminator_is_initially_null)
    uc_TRUE(NULL == ucCmd_get_response_terminator(subject));
uc_PASS

uc_TEST(ucCmd_get_response_terminator_returns_set_value)
    const char *expected, *actual, *values[] = { "EOT", "\x1b", "We're DONE!" };
    int i;
    for (i = 0; i < 3; i++) {
        expected = values[i];
        ucCmd_set_response_terminator(subject, expected);
        actual = ucCmd_get_response_terminator(subject);
        uc_TRUE(0 == strcmp(expected, actual));
        uc_TRUE(expected == actual);
    }
uc_PASS

uc_TEST(ucCmd_get_command_acknowledgment_is_initially_null)
    uc_TRUE(NULL == ucCmd_get_command_acknowledgment(subject));
uc_PASS

uc_TEST(ucCmd_get_command_acknowledgment_returns_set_value)
    const char *expected, *actual, *values[] = { "Got it!", "\x1b", "***" };
    int i;
    for (i = 0; i < 3; i++) {
        expected = values[i];
        ucCmd_set_command_acknowledgment(subject, expected);
        actual = ucCmd_get_command_acknowledgment(subject);
        uc_TRUE(0 == strcmp(expected, actual));
        uc_TRUE(expected == actual);
    }
uc_PASS

uc_TEST(ucCmd_get_arg_returns_arg_tok)
    ucCmd_parse_const(subject, "cmd argz -s argb");
    uc_TRUE(uc_STR_EQ("argz", ucCmd_get_arg(subject)));
uc_PASS

uc_TEST(ucCmd_get_arg_returns_null_if_no_arg_exists)
    ucCmd_parse_const(subject, "cmd -s argb");
    uc_TRUE(NULL == ucCmd_get_arg(subject));
uc_PASS

uc_TEST(ucCmd_get_switch_returns_switch_tok)
    ucCmd_parse_const(subject, "cmd argz -s argb");
    uc_TRUE(uc_STR_EQ("-s", ucCmd_get_switch(subject)));
uc_PASS

uc_TEST(ucCmd_get_switch_returns_null_if_no_switch_exists)
    ucCmd_parse_const(subject, "cmd argz argb");
    uc_TRUE(NULL == ucCmd_get_switch(subject));
uc_PASS

uc_TEST(ucCmd_get_switch_arg_returns_arg_tok_of_switch)
    ucCmd_parse_const(subject, "cmd argz -s1 argb -s2 argc");
    uc_TRUE(uc_STR_EQ("argc", ucCmd_get_switch_arg(subject, "-s2")));
uc_PASS

uc_TEST(ucCmd_get_switch_arg_returns_null_if_arg_does_not_exist)
    ucCmd_parse_const(subject, "cmd argz -s1 argb -s2");
    uc_TRUE(NULL == ucCmd_get_switch_arg(subject, "-s2"));
uc_PASS

uc_TEST(ucCmd_get_switch_arg_returns_null_if_switch_does_not_exist)
    ucCmd_parse_const(subject, "cmd argz -s1 argb");
    uc_TRUE(NULL == ucCmd_get_switch_arg(subject, "-s2"));
uc_PASS

uc_TEST(ucCmd_find_switch_returns_switch_if_it_exists)
    ucCmd_parse_const(subject, "cmd argz -s1 argb -s2 c");
    uc_TRUE(ucTok_equals(ucCmd_find_switch(subject, "-s2"), "-s2"));
uc_PASS

uc_TEST(ucCmd_find_switch_returns_null_if_it_does_not_exist)
    ucCmd_parse_const(subject, "cmd argz -s1 argb");
    uc_TRUE(NULL == ucCmd_find_switch(subject, "-s2"));
uc_PASS

uc_TEST(ucCmd_get_switch_arg_d_returns_argument_as_integer)
    ucCmd_parse_const(subject, "cmd argz -s1 argb -s2 42");
    uc_TRUE(42 == ucCmd_get_switch_arg_d(subject, "-s2", -1));
uc_PASS

uc_TEST(ucCmd_get_switch_arg_d_returns_default_value_if_argument_is_not_an_integer)
    ucCmd_parse_const(subject, "cmd argz -s1 argb -s2 7z");
    uc_TRUE(-386 == ucCmd_get_switch_arg_d(subject, "-s2", -386));
uc_PASS

uc_TEST(ucCmd_get_switch_arg_f_returns_argument_as_double)
    ucCmd_parse_const(subject, "cmd argz -s1 argb -s2 42.1");
    uc_TRUE(42.1 == ucCmd_get_switch_arg_f(subject, "-s2", -1));
uc_PASS

uc_TEST(ucCmd_get_switch_arg_f_returns_default_value_if_argument_is_not_numeric)
    ucCmd_parse_const(subject, "cmd argz -s1 argb -s2 4.2.1");
    uc_TRUE(-1.1 == ucCmd_get_switch_arg_f(subject, "-s2", -1.1));
uc_PASS

uc_TEST(ucCmd_get_switch_arg_b_returns_argument_as_boolean_test, const char *a, ucBool b)
    char c[35];
    sprintf(c, "cmd argz -s1 %s -s2 4.2.1", a);
    ucCmd_parse(subject, c);
    uc_TRUE(b == ucCmd_get_switch_arg_b(subject, "-s1", !b));
uc_PASS
uc_CASE(ucCmd_get_switch_arg_b_returns_argument_as_boolean_test, true, "true", ucBool_true)
uc_CASE(ucCmd_get_switch_arg_b_returns_argument_as_boolean_test, false, "false", ucBool_false)
uc_CASE(ucCmd_get_switch_arg_b_returns_argument_as_boolean_test, yes, "yes", ucBool_true)
uc_CASE(ucCmd_get_switch_arg_b_returns_argument_as_boolean_test, no, "no", ucBool_false)
uc_CASE(ucCmd_get_switch_arg_b_returns_argument_as_boolean_test, 1, "1", ucBool_true)
uc_CASE(ucCmd_get_switch_arg_b_returns_argument_as_boolean_test, 0, "0", ucBool_false)
uc_CASE(ucCmd_get_switch_arg_b_returns_argument_as_boolean_test, on, "on", ucBool_true)
uc_CASE(ucCmd_get_switch_arg_b_returns_argument_as_boolean_test, off, "off", ucBool_false)

uc_TEST(ucCmd_get_switch_arg_b_returns_default_value_if_argument_is_not_boolean)
    char c[35];
    sprintf(c, "cmd argz -s1 %s -s2 4.2.1", "tru");
    ucCmd_parse(subject, c);
    uc_TRUE(ucBool_true == ucCmd_get_switch_arg_b(subject, "-s1", ucBool_true));
    uc_TRUE(ucBool_false == ucCmd_get_switch_arg_b(subject, "-s1", ucBool_false));
uc_PASS

uc_TEST(ucCmd_get_switch_arg_x_b_test, const char *command, const char *switch_name, int arg_index, ucBool expected)
    char c[35];
    sprintf(c, "%s", command);
    ucCmd_parse(subject, c);
    uc_TRUE(expected == ucCmd_get_switch_arg_x_b(subject, switch_name, arg_index, !expected));
uc_PASS
uc_CASE(ucCmd_get_switch_arg_x_b_test, yes, "do -some -thing cool yes", "-thing", 1, ucBool_true)
uc_CASE(ucCmd_get_switch_arg_x_b_test, off, "do -some -thing cool yes off", "-thing", 2, ucBool_false)
uc_CASE(ucCmd_get_switch_arg_x_b_test, 1, "do -some 1 -thing cool yes", "-some", 0, ucBool_true)

uc_TEST(ucCmd_get_switch_arg_x_b_returns_default_value)
    ucCmd_parse_const(subject, "do -some -thing cool nope");
    uc_TRUE(ucBool_true == ucCmd_get_switch_arg_x_b(subject, "-thing", 1, ucBool_true));
uc_PASS

uc_TEST(ucCmd_get_switch_arg_x_returns_arg_at_index)
    ucCmd_parse_const(subject, "this -is -the command that was sent");
    uc_TRUE(uc_STR_EQ("command", ucCmd_get_switch_arg_x(subject, "-the", 0)));
    uc_TRUE(uc_STR_EQ("that", ucCmd_get_switch_arg_x(subject, "-the", 1)));
    uc_TRUE(NULL == ucCmd_get_switch_arg_x(subject, "-the", 4));
uc_PASS

uc_TEST(ucCmd_get_switch_arg_x_f_returns_arg_as_double, const char *command, const char *switch_name, int arg_index, double expected)
    char c[35];
    sprintf(c, "%s", command);
    ucCmd_parse(subject, c);
    uc_TRUE(expected == ucCmd_get_switch_arg_x_f(subject, switch_name, arg_index, -0.1 * expected));
uc_PASS
uc_CASE(ucCmd_get_switch_arg_x_f_returns_arg_as_double, 1, "parse -that 1", "-that", 0, 1.0)
uc_CASE(ucCmd_get_switch_arg_x_f_returns_arg_as_double, 50, "parse -that -or 1 a -50", "-or", 2, -50.0)
uc_CASE(ucCmd_get_switch_arg_x_f_returns_arg_as_double, 0_00432, "parse -that -or 1 .00432 -50", "-or", 1, 0.00432)

uc_TEST(ucCmd_get_switch_arg_x_f_returns_default_value_if_arg_is_not_numeric)
    ucCmd_parse_const(subject, "no -doubles here");
    uc_TRUE(-2.1 == ucCmd_get_switch_arg_x_f(subject, "-doubles", 0, -2.1));
uc_PASS

uc_TEST(ucCmd_get_switch_arg_x_f_returns_default_value_if_arg_does_not_exist)
    ucCmd_parse_const(subject, "no -doubles here");
    uc_TRUE(-3.2 == ucCmd_get_switch_arg_x_f(subject, "-doubles", 1, -3.2));
uc_PASS

uc_TEST(ucCmd_get_switch_arg_x_d_returns_integer_argument)
    ucCmd_parse_const(subject, "this -should be an int 32 right?");
    uc_TRUE(32 == ucCmd_get_switch_arg_x_d(subject, "-should", 3, -1));
uc_PASS

uc_TEST(ucCmd_get_switch_arg_x_d_returns_negative_argument)
    ucCmd_parse_const(subject, "this -should be an int -32 right?");
    uc_TRUE(-32 == ucCmd_get_switch_arg_x_d(subject, "-should", 3, -1));
uc_PASS

uc_TEST(ucCmd_get_switch_arg_x_d_returns_default_if_argument_doesnt_exist)
    ucCmd_parse_const(subject, "this -should be an int 32 right?");
    uc_TRUE(-55 == ucCmd_get_switch_arg_x_d(subject, "-should", 5, -55));
uc_PASS

uc_TEST(ucCmd_get_switch_arg_x_d_returns_default_if_argument_is_not_an_integer)
    ucCmd_parse_const(subject, "this -should be an int 32.1 right?");
    uc_TRUE(0 == ucCmd_get_switch_arg_x_d(subject, "-should", 3, 0));
uc_PASS

uc_TEST(ucCmd_get_arg_x_returns_argument_at_index)
    ucCmd_parse_const(subject, "here are multiple arguments for test");
    uc_TRUE(uc_STR_EQ("are", ucCmd_get_arg_x(subject, 0)));
    uc_TRUE(uc_STR_EQ("multiple", ucCmd_get_arg_x(subject, 1)));
    uc_TRUE(uc_STR_EQ("arguments", ucCmd_get_arg_x(subject, 2)));
    uc_TRUE(uc_STR_EQ("for", ucCmd_get_arg_x(subject, 3)));
    uc_TRUE(uc_STR_EQ("test", ucCmd_get_arg_x(subject, 4)));
uc_PASS

uc_TEST(ucCmd_get_arg_x_returns_null_if_it_doesnt_exist)
    ucCmd_parse_const(subject, "here are multiple ");
    uc_TRUE(NULL == ucCmd_get_arg_x(subject, 2));
    uc_TRUE(NULL == ucCmd_get_arg_x(subject, 3));
    uc_TRUE(NULL == ucCmd_get_arg_x(subject, 4));
uc_PASS

uc_TEST(ucCmd_get_arg_b_returns_boolean_value, const char *arg, ucBool expected)
    char c[25];
    sprintf(c, "command %s", arg);
    ucCmd_parse(subject, c);
    uc_TRUE(expected == ucCmd_get_arg_b(subject, !expected));
uc_PASS
uc_CASE(ucCmd_get_arg_b_returns_boolean_value, yes, "yes", ucBool_true)
uc_CASE(ucCmd_get_arg_b_returns_boolean_value, 0, "0", ucBool_false)

uc_TEST(ucCmd_get_arg_b_returns_default_value_if_arg_doesnt_exist)
    ucCmd_parse_const(subject, "hello -true");
    uc_TRUE(ucBool_false == ucCmd_get_arg_b(subject, ucBool_false));
uc_PASS

uc_TEST(ucCmd_get_arg_b_returns_default_value_if_arg_is_not_boolean)
    ucCmd_parse_const(subject, "hello tru");
    uc_TRUE(ucBool_false == ucCmd_get_arg_b(subject, ucBool_false));
uc_PASS

uc_TEST(ucCmd_get_arg_d_returns_integer_arg)
    ucCmd_parse_const(subject, "hello -455");
    uc_TRUE(-455 == ucCmd_get_arg_d(subject, 0));
uc_PASS

uc_TEST(ucCmd_get_arg_d_returns_default_value_if_arg_is_not_an_integer) 
    ucCmd_parse_const(subject, "hello -455.1");
    uc_TRUE(0 == ucCmd_get_arg_d(subject, 0));
uc_PASS

uc_TEST(ucCmd_get_arg_d_returns_default_value_if_arg_does_not_exist) 
    ucCmd_parse_const(subject, "hello -s455");
    uc_TRUE(2 == ucCmd_get_arg_d(subject, 2));
uc_PASS

uc_TEST(ucCmd_get_arg_f_returns_numeric_argument)
    ucCmd_parse_const(subject, "hello -455.4");
    uc_TRUE(-455.4 == ucCmd_get_arg_f(subject, -1));
uc_PASS

uc_TEST(ucCmd_get_arg_f_returns_default_value_if_arg_is_not_numeric)
    ucCmd_parse_const(subject, "hello -455.1.2");
    uc_TRUE(0.1 == ucCmd_get_arg_f(subject, 0.1));
uc_PASS

uc_TEST(ucCmd_get_arg_f_returns_default_value_if_arg_does_not_exist)
    ucCmd_parse_const(subject, "hello -s455.1");
    uc_TRUE(2.5 == ucCmd_get_arg_f(subject, 2.5));
uc_PASS

uc_TEST(ucCmd_get_arg_x_b_returns_boolean_arg)
    ucCmd_parse_const(subject, "cmd yes no false true 0 off 1 on");
    uc_TRUE(ucBool_true == ucCmd_get_arg_x_b(subject, 0, ucBool_false));
    uc_TRUE(ucBool_false == ucCmd_get_arg_x_b(subject, 1, ucBool_true));
    uc_TRUE(ucBool_false == ucCmd_get_arg_x_b(subject, 2, ucBool_true));
    uc_TRUE(ucBool_true == ucCmd_get_arg_x_b(subject, 3, ucBool_false));
    uc_TRUE(ucBool_false == ucCmd_get_arg_x_b(subject, 4, ucBool_true));
    uc_TRUE(ucBool_false == ucCmd_get_arg_x_b(subject, 5, ucBool_true));
    uc_TRUE(ucBool_true == ucCmd_get_arg_x_b(subject, 6, ucBool_false));
    uc_TRUE(ucBool_true == ucCmd_get_arg_x_b(subject, 7, ucBool_false));
uc_PASS

uc_TEST(ucCmd_get_arg_x_b_returns_default_value_if_arg_is_not_boolean)
    ucCmd_parse_const(subject, "cmd yup nope fals truu 0.0 of -1 onn");
    uc_TRUE(ucBool_false == ucCmd_get_arg_x_b(subject, 0, ucBool_false));
    uc_TRUE(ucBool_true == ucCmd_get_arg_x_b(subject, 1, ucBool_true));
    uc_TRUE(ucBool_true == ucCmd_get_arg_x_b(subject, 2, ucBool_true));
    uc_TRUE(ucBool_false == ucCmd_get_arg_x_b(subject, 3, ucBool_false));
    uc_TRUE(ucBool_true == ucCmd_get_arg_x_b(subject, 4, ucBool_true));
    uc_TRUE(ucBool_true == ucCmd_get_arg_x_b(subject, 5, ucBool_true));
    uc_TRUE(ucBool_false == ucCmd_get_arg_x_b(subject, 6, ucBool_false));
    uc_TRUE(ucBool_false == ucCmd_get_arg_x_b(subject, 7, ucBool_false));
uc_PASS

uc_TEST(ucCmd_get_arg_x_b_returns_default_value_if_arg_doesnt_exist)
    ucCmd_parse_const(subject, "cmd yup nope fals truu 0.0 of 1 -onn");
    uc_TRUE(ucBool_false == ucCmd_get_arg_x_b(subject, 8, ucBool_false));
    uc_TRUE(ucBool_true == ucCmd_get_arg_x_b(subject, 9, ucBool_true));
uc_PASS

uc_TEST(ucCmd_get_arg_x_d_returns_integer_arg)
    ucCmd_parse_const(subject, "command -11 22");
    uc_TRUE(-11 == ucCmd_get_arg_x_d(subject, 0, 0));
    uc_TRUE(22 == ucCmd_get_arg_x_d(subject, 1, 0));
uc_PASS

uc_TEST(ucCmd_get_arg_x_d_returns_default_value_if_arg_is_not_an_integer)
    ucCmd_parse_const(subject, "command -1.1 22a");
    uc_TRUE(0 == ucCmd_get_arg_x_d(subject, 0, 0));
    uc_TRUE(-5 == ucCmd_get_arg_x_d(subject, 1, -5));
uc_PASS

uc_TEST(ucCmd_get_arg_x_d_returns_default_value_if_arg_does_not_exist)
    ucCmd_parse_const(subject, "command -s1 2");
    uc_TRUE(5 == ucCmd_get_arg_x_d(subject, 0, 5));
    uc_TRUE(-5 == ucCmd_get_arg_x_d(subject, 1, -5));
uc_PASS

uc_TEST(ucCmd_get_arg_x_f_returns_numeric_arg)
    ucCmd_parse_const(subject, "command -1.1 2.2");
    uc_TRUE(-1.1 == ucCmd_get_arg_x_f(subject, 0, 0));
    uc_TRUE(2.2 == ucCmd_get_arg_x_f(subject, 1, 0));
uc_PASS

uc_TEST(ucCmd_get_arg_x_f_returns_default_value_if_arg_is_not_numeric)
    ucCmd_parse_const(subject, "command -1.1.1 2.2a");
    uc_TRUE(12.3 == ucCmd_get_arg_x_f(subject, 0, 12.3));
    uc_TRUE(-5 == ucCmd_get_arg_x_f(subject, 1, -5));
uc_PASS

uc_TEST(ucCmd_get_arg_x_f_returns_default_value_if_arg_does_not_exist)
    ucCmd_parse_const(subject, "command -s1.1 2.2");
    uc_TRUE(5 == ucCmd_get_arg_x_f(subject, 0, 5));
    uc_TRUE(-5.1 == ucCmd_get_arg_x_f(subject, 1, -5.1));
uc_PASS

uc_TEST(ucCmd_get_switch_x_returns_switch_at_index)
    ucCmd_parse_const(subject, "cmd -s1 -s2 -s3");
    uc_TRUE(uc_STR_EQ("-s1", ucCmd_get_switch_x(subject, 0)));
    uc_TRUE(uc_STR_EQ("-s2", ucCmd_get_switch_x(subject, 1)));
    uc_TRUE(uc_STR_EQ("-s3", ucCmd_get_switch_x(subject, 2)));
uc_PASS

uc_TEST(ucCmd_get_switch_x_returns_null_if_no_switch_exists)
    ucCmd_parse_const(subject, "cmd -s1 -s2");
    uc_TRUE(NULL == ucCmd_get_switch_x(subject, 2));
uc_PASS

static char *set_receive_sets_value(char *buf, size_t buf_size, void *state) { 
    return NULL;
}
uc_TEST(ucCmd_set_receive_sets_value)
    ucCmd_set_receive(subject, set_receive_sets_value);
    uc_TRUE(set_receive_sets_value == subject->receive);
uc_PASS

uc_TEST(ucCmd_set_receive_state_sets_value)
    char *state = "the state";
    ucCmd_set_receive_state(subject, state);
    uc_TRUE(state == subject->receive_state);
uc_PASS

static char *listen_uses_receive_buf;
static size_t listen_uses_receive_buf_size;
static void *listen_uses_receive_state;
static char *listen_uses_receive(char *buf, size_t buf_size, void *state) { 
    strcpy(buf, "check me");
    listen_uses_receive_buf = buf;
    listen_uses_receive_buf_size = buf_size;
    listen_uses_receive_state = state;
    return buf;
}
uc_TEST(ucCmd_listen_uses_receive)
    const char *cmd;
    int state = 399;
    ucCmd_set_receive(subject, listen_uses_receive);
    ucCmd_set_receive_state(subject, &state);
    cmd = ucCmd_listen(subject);
    uc_TRUE(uc_STR_EQ("check", cmd));
    uc_TRUE(&state == listen_uses_receive_state);
    uc_TRUE(cmd == listen_uses_receive_buf);
    uc_TRUE(ucCmd_get_command_length_max(subject) == listen_uses_receive_buf_size);
uc_PASS

uc_TEST(ucCmd_get_parser_returns_instance)
    uc_TRUE(NULL != ucCmd_get_parser(subject));
uc_PASS

uc_TEST(ucCmd_parse_parses_command)
    char c[30] = "command a -s sa";
    ucCmd_parse(subject, c);
    uc_TRUE(uc_STR_EQ("command", ucCmd_get_command(subject)));
    uc_TRUE(uc_STR_EQ("a", ucCmd_get_arg(subject)));
    uc_TRUE(uc_STR_EQ("-s", ucCmd_get_switch(subject)));
    uc_TRUE(uc_STR_EQ("sa", ucCmd_get_switch_arg(subject, "-s")));
uc_PASS

uc_TEST(ucCmd_parse_const_parses_const_string)
    const char *the_command = "the command";
    ucCmd_parse_const(subject, the_command);
    uc_TRUE(uc_STR_EQ("the", ucCmd_get_command(subject)));
    uc_TRUE(uc_STR_EQ("command", ucCmd_get_arg(subject)));
uc_PASS

uc_TEST(ucCmd_get_command_length_max_returns_max_length)
    uc_TRUE(ucCmd_COMMAND_SIZE == ucCmd_get_command_length_max(subject));
uc_PASS

static char *listen_parses_command(char *buf, size_t buf_size, void *state) {
    strcpy(buf, "rx -c a");
    return buf;
}
uc_TEST(ucCmd_listen_parses_command)
    ucTok *cmd_tok;
    ucCmd_set_receive(subject, listen_parses_command);
    cmd_tok = ucCmd_listen(subject);
    uc_TRUE(cmd_tok == ucCmd_get_command(subject));
    uc_TRUE(ucTok_equals(cmd_tok, "rx"));
    uc_TRUE(ucTok_equals(ucTok_get_next(cmd_tok), "-c"));
    uc_TRUE(ucTok_equals(ucTok_get_next(ucTok_get_next(cmd_tok)), "a"));
uc_PASS

uc_TEST(ucCmd_parse_const_test_too_long_of_string)
    int i;
    char *buffer;
    char command[ucCmd_COMMAND_SIZE + 10];
    for (i = 0; i < ucCmd_COMMAND_SIZE + 9; i++) {
        command[i] = 'c';
    }
    command[i] = '\0';
    ucCmd_parse_const(subject, command);
    buffer = ucCmd_get_command_buffer(subject);
    for (i = 0; i < (ucCmd_COMMAND_SIZE - 1); i++) {
        uc_TRUE(command[i] == buffer[i]);
    }
    uc_TRUE('\0' == buffer[i]);
    uc_TRUE(ucParser_CMD_TERMINATOR == buffer[i + 1]);
uc_PASS

uc_TEST(ucCmd_format_response_test_too_long_of_string)
    int i;
    char response[ucCmd_RESPONSE_SIZE + 10];
    for (i = 0; i < ucCmd_RESPONSE_SIZE + 9; i++) {
        response[i] = 'r';
    }
    response[i] = '\0';
    ucCmd_format_response(subject, "%s", response);
    for (i = 0; i < (ucCmd_RESPONSE_SIZE - 1); i++) {
        uc_TRUE(response[i] == subject->response[i]);
    }
    uc_TRUE('\0' == subject->response[i]);
    uc_TRUE(0 == strncmp(response, subject->response, ucCmd_RESPONSE_SIZE - 1));
uc_PASS

uc_TEST_GROUP(ucCmd, setup,
    ucCmd_create_does_not_return_null,
    ucCmd_find_switch_returns_null_if_it_does_not_exist,
    ucCmd_find_switch_returns_switch_if_it_exists,
    ucCmd_format_response_sets_response_string,
    ucCmd_format_response_test_too_long_of_string,
    ucCmd_format_response_va_sets_response_string,
    ucCmd_get_arg_returns_arg_tok,
    ucCmd_get_arg_returns_null_if_no_arg_exists,
    ucCmd_get_arg_b_returns_boolean_value_yes,
    ucCmd_get_arg_b_returns_boolean_value_0,
    ucCmd_get_arg_b_returns_default_value_if_arg_doesnt_exist,
    ucCmd_get_arg_b_returns_default_value_if_arg_is_not_boolean,
    ucCmd_get_arg_d_returns_integer_arg,
    ucCmd_get_arg_d_returns_default_value_if_arg_is_not_an_integer,
    ucCmd_get_arg_d_returns_default_value_if_arg_does_not_exist,
    ucCmd_get_arg_f_returns_numeric_argument,
    ucCmd_get_arg_f_returns_default_value_if_arg_is_not_numeric,
    ucCmd_get_arg_f_returns_default_value_if_arg_does_not_exist,
    ucCmd_get_arg_x_returns_argument_at_index,
    ucCmd_get_arg_x_returns_null_if_it_doesnt_exist,
    ucCmd_get_arg_x_b_returns_boolean_arg,
    ucCmd_get_arg_x_b_returns_default_value_if_arg_is_not_boolean,
    ucCmd_get_arg_x_b_returns_default_value_if_arg_doesnt_exist,
    ucCmd_get_arg_x_d_returns_integer_arg,
    ucCmd_get_arg_x_d_returns_default_value_if_arg_is_not_an_integer,
    ucCmd_get_arg_x_d_returns_default_value_if_arg_does_not_exist,
    ucCmd_get_arg_x_f_returns_numeric_arg,
    ucCmd_get_arg_x_f_returns_default_value_if_arg_is_not_numeric,
    ucCmd_get_arg_x_f_returns_default_value_if_arg_does_not_exist,
    ucCmd_get_command_acknowledgment_is_initially_null,
    ucCmd_get_command_acknowledgment_returns_set_value,
    ucCmd_get_command_returns_cmd_tok,
    ucCmd_get_command_length_max_returns_max_length,
    ucCmd_get_parser_returns_instance,
    ucCmd_get_response_length_max_returns_size,
    ucCmd_get_response_terminator_is_initially_null,
    ucCmd_get_response_terminator_returns_set_value,
    ucCmd_get_switch_arg_returns_arg_tok_of_switch,
    ucCmd_get_switch_arg_returns_null_if_arg_does_not_exist,
    ucCmd_get_switch_arg_returns_null_if_switch_does_not_exist,
    ucCmd_get_switch_arg_b_returns_argument_as_boolean_test_true,
    ucCmd_get_switch_arg_b_returns_argument_as_boolean_test_false,
    ucCmd_get_switch_arg_b_returns_argument_as_boolean_test_yes,
    ucCmd_get_switch_arg_b_returns_argument_as_boolean_test_no,
    ucCmd_get_switch_arg_b_returns_argument_as_boolean_test_1,
    ucCmd_get_switch_arg_b_returns_argument_as_boolean_test_0,
    ucCmd_get_switch_arg_b_returns_argument_as_boolean_test_on,
    ucCmd_get_switch_arg_b_returns_argument_as_boolean_test_off,
    ucCmd_get_switch_arg_b_returns_default_value_if_argument_is_not_boolean,
    ucCmd_get_switch_arg_d_returns_argument_as_integer,
    ucCmd_get_switch_arg_d_returns_default_value_if_argument_is_not_an_integer,
    ucCmd_get_switch_arg_f_returns_argument_as_double,
    ucCmd_get_switch_arg_f_returns_default_value_if_argument_is_not_numeric,
    ucCmd_get_switch_arg_x_b_returns_default_value,
    ucCmd_get_switch_arg_x_b_test_1,
    ucCmd_get_switch_arg_x_b_test_off,
    ucCmd_get_switch_arg_x_b_test_yes,
    ucCmd_get_switch_arg_x_d_returns_default_if_argument_doesnt_exist,
    ucCmd_get_switch_arg_x_d_returns_default_if_argument_is_not_an_integer,
    ucCmd_get_switch_arg_x_d_returns_integer_argument,
    ucCmd_get_switch_arg_x_d_returns_negative_argument,
    ucCmd_get_switch_arg_x_returns_arg_at_index,
    ucCmd_get_switch_arg_x_f_returns_arg_as_double_1,
    ucCmd_get_switch_arg_x_f_returns_arg_as_double_50,
    ucCmd_get_switch_arg_x_f_returns_arg_as_double_0_00432,
    ucCmd_get_switch_arg_x_f_returns_default_value_if_arg_does_not_exist,
    ucCmd_get_switch_arg_x_f_returns_default_value_if_arg_is_not_numeric,
    ucCmd_get_switch_returns_null_if_no_switch_exists,
    ucCmd_get_switch_returns_switch_tok,
    ucCmd_get_switch_x_returns_switch_at_index,
    ucCmd_get_switch_x_returns_null_if_no_switch_exists,
    ucCmd_is_canceled_calls_is_canceled,
    ucCmd_is_canceled_uses_state,
    ucCmd_listen_parses_command,
    ucCmd_listen_uses_receive,
    ucCmd_parse_parses_command,
    ucCmd_parse_const_parses_const_string,
    ucCmd_parse_const_test_too_long_of_string,
    ucCmd_respond_does_nothing_if_is_quiet,
    ucCmd_respond_uses_transmit,
    ucCmd_set_command_sets_cmd_tok,
    ucCmd_set_handle_invalid_command_sets_callback,
    ucCmd_set_is_canceled_sets_value,
    ucCmd_set_is_canceled_state_sets_value,
    ucCmd_set_is_quiet_sets_value,
    ucCmd_set_receive_sets_value,
    ucCmd_set_receive_state_sets_value,
    ucCmd_set_transmit_sets_transmit,
    ucCmd_set_transmit_state_sets_value,
    ucCmd_transmit_uses_state)
