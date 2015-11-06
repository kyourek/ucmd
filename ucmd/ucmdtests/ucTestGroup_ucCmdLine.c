#include <stdio.h>
#include <stdarg.h>
#include "ucmdtests.h"

static ucCmdLine *subject;

static ucCmdTok *parse_cmd(char *cmd) {
    ucCmdParser *cmd_parser = ucCmdParser_create();
    ucCmdTok *cmd_tok = ucCmdParser_parse(cmd_parser, cmd);
    ucCmdParser_destroy(cmd_parser);
    return cmd_tok;
}

static void *transmit_1_state;
static const char *transmit_1_response;
static void transmit_1(const char *response, void *state) {
    transmit_1_state = state;
    transmit_1_response = response;
}

static void transmit_2(const char *response, void *state) {
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
    subject = ucCmdLine_create();
    assert(subject);
uc_PASS

uc_TEST(after)
    ucCmdLine_destroy(subject);
uc_PASS

uc_TEST(setup)
    ucTestGroup_setup_test(p, prior, after);
uc_PASS

uc_TEST(ucCmdLine_get_cmd_tok_returns_cmd_tok)
    ucCmdLine c;
    ucCmdTok *t = "token";
    c.cmd_tok = t;
    uc_TRUE(t == ucCmdLine_get_cmd_tok(&c));
uc_PASS

uc_TEST(ucCmdLine_set_cmd_tok_sets_cmd_tok)
    ucCmdLine c;
    ucCmdTok *t = "cmd_tok";
    ucCmdLine_set_cmd_tok(&c, t);
    uc_TRUE(t == c.cmd_tok);
uc_PASS

uc_TEST(ucCmdLine_get_cmd_toks_returns_toks)
    ucCmdLineToks toks = { 0 };
    ucCmdLine *cmd = subject;
    char c[30] = "cmd arg -s";

    ucCmdLine_set_cmd_tok(cmd, parse_cmd(c));
    ucCmdLine_get_cmd_toks(cmd, &toks);

    uc_TRUE(ucTok_equals(toks.cmd_tok, "cmd"));
    uc_TRUE(ucTok_equals(toks.arg_tok, "arg"));
    uc_TRUE(ucTok_equals(toks.switch_tok, "-s"));
uc_PASS

uc_TEST(ucCmdLine_get_cmd_toks_sets_null_tok_values)
    ucCmdLineToks toks = { 0 };
    ucCmdLine *cmd = subject;
    char c[12] = "command";

    ucCmdLine_set_cmd_tok(cmd, parse_cmd(c));
    ucCmdLine_get_cmd_toks(cmd, &toks);

    uc_TRUE(ucTok_equals(toks.cmd_tok, "command"));
    uc_TRUE(NULL == toks.arg_tok);
    uc_TRUE(NULL == toks.switch_tok);
uc_PASS

uc_TEST(ucCmdLine_get_cmd_toks_returns_null_if_buffer_is_null)
    uc_TRUE(NULL == ucCmdLine_get_cmd_toks(subject, NULL));
uc_PASS

uc_TEST(ucCmdLine_create_does_not_return_null)
    uc_TRUE(NULL != subject);
uc_PASS

uc_TEST(ucCmdLine_format_response_sets_response_string)
    char expected[50];
    ucCmdLine *ptr = subject;
    ucCmdLine_format_response(ptr, "formatted %s %d %3.2f fin", "string", 10, 5.1234);
    sprintf(expected, "formatted %s %d %3.2f fin", "string", 10, 5.1234);
    uc_TRUE(0 == strcmp(ptr->response, expected));
uc_PASS

static const char *ucCmdLine_format_response_va_sets_response_string_helper(ucCmdLine *p, const char *format, ...) {
    va_list arg_list;
    const char *response;
    va_start(arg_list, format);
    response = ucCmdLine_format_response_va(p, format, arg_list);
    va_end(arg_list);
    return response;
}

uc_TEST(ucCmdLine_format_response_va_sets_response_string)
    char expected[50];
    const char *actual;
    ucCmdLine *ptr = subject;
    
    actual = ucCmdLine_format_response_va_sets_response_string_helper(ptr, "This %d is %s formatted.", 1, "well");
    sprintf(expected, "This %d is %s formatted.", 1, "well");
    uc_TRUE(0 == strcmp(actual, expected));
    uc_TRUE(0 == strcmp(ptr->response, expected));
uc_PASS

uc_TEST(ucCmdLine_set_transmit_sets_transmit)
    ucCmdLine c;
    ucCmdLine_set_transmit(&c, transmit_1);
    uc_TRUE(transmit_1 == c.transmit);
uc_PASS

uc_TEST(ucCmdLine_get_transmit_returns_transmit)
    ucCmdLine c;
    c.transmit = transmit_2;
    uc_TRUE(transmit_2 == ucCmdLine_get_transmit(&c));
uc_PASS

uc_TEST(ucCmdLine_respond_uses_transmit)
    ucCmdLine c;
    c.is_quiet = ucBool_false;
    c.transmit = transmit_1;

    transmit_1_response = NULL;
    c.transmit = transmit_1;
    ucCmdLine_respond(&c, "expected response");
    uc_TRUE(0 == strcmp(transmit_1_response, "expected response"));
uc_PASS

uc_TEST(ucCmdLine_get_is_canceled_returns_value)
    ucCmdLine c;
    c.is_canceled = is_canceled_1;
    uc_TRUE(is_canceled_1 == ucCmdLine_get_is_canceled(&c));
uc_PASS

uc_TEST(ucCmdLine_set_is_canceled_sets_value)
    ucCmdLine c;
    ucCmdLine_set_is_canceled(&c, is_canceled_2);
    uc_TRUE(is_canceled_2 == c.is_canceled);
uc_PASS

uc_TEST(ucCmdLine_is_canceled_calls_is_canceled)
    ucCmdLine c;
    c.is_canceled = is_canceled_1;

    is_canceled_1_returned_value = ucBool_true;
    uc_TRUE(ucBool_true == ucCmdLine_is_canceled(&c));

    is_canceled_1_returned_value = ucBool_false;
    uc_TRUE(ucBool_false == ucCmdLine_is_canceled(&c));
uc_PASS

uc_TEST(ucCmdLine_set_transmit_state_sets_value)
    int state;
    ucCmdLine *ptr = subject;
    void *prev_value = ptr->transmit_state;

    ucCmdLine_set_transmit_state(ptr, &state);
    uc_TRUE(&state == ptr->transmit_state);

    ptr->transmit_state = prev_value;
uc_PASS

uc_TEST(ucCmdLine_get_transmit_state_gets_value)
    char *state = "state";
    ucCmdLine *ptr = subject;
    void *prev_value = ptr->transmit_state;

    ptr->transmit_state = &state;
    uc_TRUE(&state == ucCmdLine_get_transmit_state(ptr));

    ptr->transmit_state = prev_value;
uc_PASS

uc_TEST(ucCmdLine_transmit_uses_state)
    double state;
    ucCmdLine *ptr = subject;
    ucCmdLine_TransmitFunc *prev_func = ptr->transmit;
    void *prev_state = ptr->transmit_state;

    ucCmdLine_set_transmit(ptr, transmit_1);
    ucCmdLine_set_transmit_state(ptr, &state);

    ucCmdLine_respond(ptr, "resp");

    uc_TRUE(&state == transmit_1_state);

    ptr->transmit = prev_func;
    ptr->transmit_state = prev_state;
uc_PASS

uc_TEST(ucCmdLine_set_is_canceled_state_sets_value)
    int state;
    ucCmdLine *ptr = subject;
    void *prev_value = ptr->is_canceled_state;

    ucCmdLine_set_is_canceled_state(ptr, &state);
    uc_TRUE(&state == ptr->is_canceled_state);

    ptr->is_canceled_state = prev_value;
uc_PASS

uc_TEST(ucCmdLine_get_is_canceled_state_returns_value)
    char *state = "st";
    ucCmdLine *ptr = subject;
    void *prev_value = ptr->is_canceled_state;

    ptr->is_canceled_state = &state;
    uc_TRUE(&state == ucCmdLine_get_is_canceled_state(ptr));

    ptr->is_canceled_state = prev_value;
uc_PASS

uc_TEST(ucCmdLine_is_canceled_uses_state)
    double state;
    ucCmdLine *ptr = subject;
    void *prev_value = ptr->is_canceled_state;

    ucCmdLine_set_is_canceled(ptr, is_canceled_1);
    ucCmdLine_set_is_canceled_state(ptr, &state);

    ucCmdLine_is_canceled(ptr);

    uc_TRUE(&state == is_canceled_1_state);

    ptr->is_canceled_state = prev_value;
uc_PASS

uc_TEST(ucCmdLine_set_is_quiet_sets_value)
    ucCmdLine *ptr = subject;
    
    ucBool prev_val = ucCmdLine_get_is_quiet(ptr);
    uc_TRUE(ucBool_false == prev_val);

    ucCmdLine_set_is_quiet(ptr, ucBool_true);
    uc_TRUE(ucCmdLine_get_is_quiet(ptr) == ucBool_true);

    ucCmdLine_set_is_quiet(ptr, prev_val);
    uc_TRUE(ucCmdLine_get_is_quiet(ptr) == prev_val);
uc_PASS

uc_TEST(ucCmdLine_respond_does_nothing_if_is_quiet)
    ucCmdLine *ptr = subject;
    ucBool pre_val = ucCmdLine_get_is_quiet(ptr);

    ucCmdLine_set_is_quiet(ptr, ucBool_true);
    ucCmdLine_set_transmit(ptr, transmit_1);

    transmit_1_response = NULL;
    ucCmdLine_respond(ptr, "expected response");
    uc_TRUE(NULL == transmit_1_response);

    ucCmdLine_set_is_quiet(ptr, pre_val);
uc_PASS

uc_TEST(ucCmdLine_set_handle_invalid_command_sets_callback)
    int state;
    ucBool ret_val;
    ucCmdLine *ptr = subject;
    ucCmdLine_set_handle_invalid_command(ptr, handle_invalid_command_1);
    ucCmdLine_set_handle_invalid_command_state(ptr, &state);

    handle_invalid_command_1_return = ucBool_true;
    ret_val = ucCmdLine_handle_invalid_command(ptr, "n-a");
    uc_TRUE(handle_invalid_command_1_state == &state);
    uc_TRUE(0 == strcmp("n-a", handle_invalid_command_1_invalid_command));
    uc_TRUE(ucBool_true == ret_val);
uc_PASS

uc_TEST(ucCmdLine_get_response_size_max_returns_size)
    ucCmdLine *ptr = subject;
    size_t size = ucCmdLine_get_response_size_max(ptr);
    uc_TRUE(ucCmdLine_RESPONSE_SIZE == size);
uc_PASS

uc_TEST(ucCmdLine_get_response_terminator_is_initially_null)
    uc_TRUE(NULL == ucCmdLine_get_response_terminator(subject));
uc_PASS

uc_TEST(ucCmdLine_get_response_terminator_returns_set_value)
    const char *expected, *actual, *values[] = { "EOT", "\x1b", "We're DONE!" };
    int i;
    for (i = 0; i < 3; i++) {
        expected = values[i];
        ucCmdLine_set_response_terminator(subject, expected);
        actual = ucCmdLine_get_response_terminator(subject);
        uc_TRUE(0 == strcmp(expected, actual));
        uc_TRUE(expected == actual);
    }
uc_PASS

uc_TEST(ucCmdLine_get_command_acknowledgment_is_initially_null)
    uc_TRUE(NULL == ucCmdLine_get_command_acknowledgment(subject));
uc_PASS

uc_TEST(ucCmdLine_get_command_acknoledgment_returns_set_value)
    const char *expected, *actual, *values[] = { "Got it!", "\x1b", "***" };
    int i;
    for (i = 0; i < 3; i++) {
        expected = values[i];
        ucCmdLine_set_command_acknowledgment(subject, expected);
        actual = ucCmdLine_get_command_acknowledgment(subject);
        uc_TRUE(0 == strcmp(expected, actual));
        uc_TRUE(expected == actual);
    }
uc_PASS

uc_TEST(ucCmdLine_get_arg_returns_arg_tok)
    char c[20] = "cmd argz -s argb";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    uc_TRUE(uc_STR_EQ("argz", ucCmdLine_get_arg(subject)));
uc_PASS

uc_TEST(ucCmdLine_get_arg_returns_null_if_no_arg_exists)
    char c[15] = "cmd -s argb";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    uc_TRUE(NULL == ucCmdLine_get_arg(subject));
uc_PASS

uc_TEST(ucCmdLine_get_switch_returns_switch_tok)
    char c[20] = "cmd argz -s argb";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    uc_TRUE(uc_STR_EQ("-s", ucCmdLine_get_switch(subject)));
uc_PASS

uc_TEST(ucCmdLine_get_switch_returns_null_if_no_switch_exists)
    char c[20] = "cmd argz argb";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    uc_TRUE(NULL == ucCmdLine_get_switch(subject));
uc_PASS

uc_TEST(ucCmdLine_get_switch_arg_returns_arg_tok_of_switch)
    char c[30] = "cmd argz -s1 argb -s2 argc";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    uc_TRUE(uc_STR_EQ("argc", ucCmdLine_get_switch_arg(subject, "-s2")));
uc_PASS

uc_TEST(ucCmdLine_get_switch_arg_returns_null_if_arg_does_not_exist)
    char c[30] = "cmd argz -s1 argb -s2";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    uc_TRUE(NULL == ucCmdLine_get_switch_arg(subject, "-s2"));
uc_PASS

uc_TEST(ucCmdLine_get_switch_arg_returns_null_if_switch_does_not_exist)
    char c[30] = "cmd argz -s1 argb";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    uc_TRUE(NULL == ucCmdLine_get_switch_arg(subject, "-s2"));
uc_PASS

uc_TEST(ucCmdLine_find_switch_returns_switch_if_it_exists)
    char c[30] = "cmd argz -s1 argb -s2 c";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    uc_TRUE(ucTok_equals(ucCmdLine_find_switch(subject, "-s2"), "-s2"));
uc_PASS

uc_TEST(ucCmdLine_find_switch_returns_null_if_it_does_not_exist)
    char c[30] = "cmd argz -s1 argb";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    uc_TRUE(NULL == ucCmdLine_find_switch(subject, "-s2"));
uc_PASS

uc_TEST(ucCmdLine_get_switch_arg_d_returns_argument_as_integer)
    char c[30] = "cmd argz -s1 argb -s2 42";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    uc_TRUE(42 == ucCmdLine_get_switch_arg_d(subject, "-s2", -1));
uc_PASS

uc_TEST(ucCmdLine_get_switch_arg_d_returns_default_value_if_argument_is_not_an_integer)
    char c[30] = "cmd argz -s1 argb -s2 7z";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    uc_TRUE(-386 == ucCmdLine_get_switch_arg_d(subject, "-s2", -386));
uc_PASS

uc_TEST(ucCmdLine_get_switch_arg_f_returns_argument_as_double)
    char c[30] = "cmd argz -s1 argb -s2 42.1";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    uc_TRUE(42.1 == ucCmdLine_get_switch_arg_f(subject, "-s2", -1));
uc_PASS

uc_TEST(ucCmdLine_get_switch_arg_f_returns_default_value_if_argument_is_not_numeric)
    char c[30] = "cmd argz -s1 argb -s2 4.2.1";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    uc_TRUE(-1.1 == ucCmdLine_get_switch_arg_f(subject, "-s2", -1.1));
uc_PASS

uc_TEST(ucCmdLine_get_switch_arg_b_returns_argument_as_boolean_test, const char *a, ucBool b)
    char c[35];
    sprintf(c, "cmd argz -s1 %s -s2 4.2.1", a);
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    uc_TRUE(b == ucCmdLine_get_switch_arg_b(subject, "-s1", !b));
uc_PASS
uc_CASE(ucCmdLine_get_switch_arg_b_returns_argument_as_boolean_test, true, "true", ucBool_true)
uc_CASE(ucCmdLine_get_switch_arg_b_returns_argument_as_boolean_test, false, "false", ucBool_false)
uc_CASE(ucCmdLine_get_switch_arg_b_returns_argument_as_boolean_test, yes, "yes", ucBool_true)
uc_CASE(ucCmdLine_get_switch_arg_b_returns_argument_as_boolean_test, no, "no", ucBool_false)
uc_CASE(ucCmdLine_get_switch_arg_b_returns_argument_as_boolean_test, 1, "1", ucBool_true)
uc_CASE(ucCmdLine_get_switch_arg_b_returns_argument_as_boolean_test, 0, "0", ucBool_false)
uc_CASE(ucCmdLine_get_switch_arg_b_returns_argument_as_boolean_test, on, "on", ucBool_true)
uc_CASE(ucCmdLine_get_switch_arg_b_returns_argument_as_boolean_test, off, "off", ucBool_false)

uc_TEST(ucCmdLine_get_switch_arg_b_returns_default_value_if_argument_is_not_boolean)
    char c[35];
    sprintf(c, "cmd argz -s1 %s -s2 4.2.1", "tru");
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    uc_TRUE(ucBool_true == ucCmdLine_get_switch_arg_b(subject, "-s1", ucBool_true));
    uc_TRUE(ucBool_false == ucCmdLine_get_switch_arg_b(subject, "-s1", ucBool_false));
uc_PASS

uc_TEST(ucCmdLine_get_switch_arg_x_b_test, const char *command, const char *switch_name, int arg_index, ucBool expected)
    char c[35];
    sprintf(c, "%s", command);
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    uc_TRUE(expected == ucCmdLine_get_switch_arg_x_b(subject, switch_name, arg_index, !expected));
uc_PASS
uc_CASE(ucCmdLine_get_switch_arg_x_b_test, yes, "do -some -thing cool yes", "-thing", 1, ucBool_true)
uc_CASE(ucCmdLine_get_switch_arg_x_b_test, off, "do -some -thing cool yes off", "-thing", 2, ucBool_false)
uc_CASE(ucCmdLine_get_switch_arg_x_b_test, 1, "do -some 1 -thing cool yes", "-some", 0, ucBool_true)

uc_TEST(ucCmdLine_get_switch_arg_x_returns_arg_at_index)
    char c[40] = "this -is -the command that was sent";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    uc_TRUE(uc_STR_EQ("command", ucCmdLine_get_switch_arg_x(subject, "-the", 0)));
    uc_TRUE(uc_STR_EQ("that", ucCmdLine_get_switch_arg_x(subject, "-the", 1)));
    uc_TRUE(NULL == ucCmdLine_get_switch_arg_x(subject, "-the", 4));
uc_PASS

uc_TEST_GROUP(ucCmdLine, setup,
    ucCmdLine_create_does_not_return_null,
    ucCmdLine_find_switch_returns_null_if_it_does_not_exist,
    ucCmdLine_find_switch_returns_switch_if_it_exists,
    ucCmdLine_format_response_sets_response_string,
    ucCmdLine_format_response_va_sets_response_string,
    ucCmdLine_get_arg_returns_arg_tok,
    ucCmdLine_get_arg_returns_null_if_no_arg_exists,
    ucCmdLine_get_cmd_toks_returns_toks,
    ucCmdLine_get_cmd_toks_sets_null_tok_values,
    ucCmdLine_set_cmd_tok_sets_cmd_tok,
    ucCmdLine_get_cmd_tok_returns_cmd_tok,
    ucCmdLine_get_cmd_toks_returns_null_if_buffer_is_null,
    ucCmdLine_get_switch_arg_returns_arg_tok_of_switch,
    ucCmdLine_get_switch_arg_returns_null_if_arg_does_not_exist,
    ucCmdLine_get_switch_arg_returns_null_if_switch_does_not_exist,
    ucCmdLine_get_switch_arg_b_returns_argument_as_boolean_test_true,
    ucCmdLine_get_switch_arg_b_returns_argument_as_boolean_test_false,
    ucCmdLine_get_switch_arg_b_returns_argument_as_boolean_test_yes,
    ucCmdLine_get_switch_arg_b_returns_argument_as_boolean_test_no,
    ucCmdLine_get_switch_arg_b_returns_argument_as_boolean_test_1,
    ucCmdLine_get_switch_arg_b_returns_argument_as_boolean_test_0,
    ucCmdLine_get_switch_arg_b_returns_argument_as_boolean_test_on,
    ucCmdLine_get_switch_arg_b_returns_argument_as_boolean_test_off,
    ucCmdLine_get_switch_arg_b_returns_default_value_if_argument_is_not_boolean,
    ucCmdLine_get_switch_arg_d_returns_argument_as_integer,
    ucCmdLine_get_switch_arg_d_returns_default_value_if_argument_is_not_an_integer,
    ucCmdLine_get_switch_arg_f_returns_argument_as_double,
    ucCmdLine_get_switch_arg_f_returns_default_value_if_argument_is_not_numeric,
    ucCmdLine_get_switch_arg_x_b_test_1,
    ucCmdLine_get_switch_arg_x_b_test_off,
    ucCmdLine_get_switch_arg_x_b_test_yes,
    ucCmdLine_get_switch_arg_x_returns_arg_at_index,
    ucCmdLine_get_switch_returns_null_if_no_switch_exists,
    ucCmdLine_get_switch_returns_switch_tok,
    ucCmdLine_set_transmit_sets_transmit,
    ucCmdLine_get_transmit_returns_transmit,
    ucCmdLine_respond_uses_transmit,
    ucCmdLine_get_is_canceled_returns_value,
    ucCmdLine_set_is_canceled_sets_value,
    ucCmdLine_is_canceled_calls_is_canceled,
    ucCmdLine_set_transmit_state_sets_value,
    ucCmdLine_get_transmit_state_gets_value,
    ucCmdLine_transmit_uses_state,
    ucCmdLine_set_is_canceled_state_sets_value,
    ucCmdLine_get_is_canceled_state_returns_value,
    ucCmdLine_is_canceled_uses_state,
    ucCmdLine_set_is_quiet_sets_value,
    ucCmdLine_respond_does_nothing_if_is_quiet,
    ucCmdLine_set_handle_invalid_command_sets_callback,
    ucCmdLine_get_response_size_max_returns_size,
    ucCmdLine_get_response_terminator_is_initially_null,
    ucCmdLine_get_response_terminator_returns_set_value,
    ucCmdLine_get_command_acknowledgment_is_initially_null,
    ucCmdLine_get_command_acknoledgment_returns_set_value)
