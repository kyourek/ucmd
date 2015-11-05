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
    ucTRUE(t == ucCmdLine_get_cmd_tok(&c));
uc_PASS

uc_TEST(ucCmdLine_set_cmd_tok_sets_cmd_tok)
    ucCmdLine c;
    ucCmdTok *t = "cmd_tok";
    ucCmdLine_set_cmd_tok(&c, t);
    ucTRUE(t == c.cmd_tok);
uc_PASS

uc_TEST(ucCmdLine_get_cmd_toks_returns_toks)
    ucCmdLineToks toks = { 0 };
    ucCmdLine *cmd = subject;
    char c[30] = "cmd arg -s";

    ucCmdLine_set_cmd_tok(cmd, parse_cmd(c));
    ucCmdLine_get_cmd_toks(cmd, &toks);

    ucTRUE(ucTok_equals(toks.cmd_tok, "cmd"));
    ucTRUE(ucTok_equals(toks.arg_tok, "arg"));
    ucTRUE(ucTok_equals(toks.switch_tok, "-s"));
uc_PASS

uc_TEST(ucCmdLine_get_cmd_toks_sets_null_tok_values)
    ucCmdLineToks toks = { 0 };
    ucCmdLine *cmd = subject;
    char c[12] = "command";

    ucCmdLine_set_cmd_tok(cmd, parse_cmd(c));
    ucCmdLine_get_cmd_toks(cmd, &toks);

    ucTRUE(ucTok_equals(toks.cmd_tok, "command"));
    ucTRUE(NULL == toks.arg_tok);
    ucTRUE(NULL == toks.switch_tok);
uc_PASS

uc_TEST(ucCmdLine_get_cmd_toks_returns_null_if_buffer_is_null)
    ucTRUE(NULL == ucCmdLine_get_cmd_toks(subject, NULL));
uc_PASS

uc_TEST(ucCmdLine_create_does_not_return_null)
    ucTRUE(NULL != subject);
uc_PASS

uc_TEST(ucCmdLine_format_response_sets_response_string)
    char expected[50];
    ucCmdLine *ptr = subject;
    ucCmdLine_format_response(ptr, "formatted %s %d %3.2f fin", "string", 10, 5.1234);
    sprintf(expected, "formatted %s %d %3.2f fin", "string", 10, 5.1234);
    ucTRUE(0 == strcmp(ptr->response, expected));
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
    ucTRUE(0 == strcmp(actual, expected));
    ucTRUE(0 == strcmp(ptr->response, expected));
uc_PASS

uc_TEST(ucCmdLine_set_transmit_sets_transmit)
    ucCmdLine c;
    ucCmdLine_set_transmit(&c, transmit_1);
    ucTRUE(transmit_1 == c.transmit);
uc_PASS

uc_TEST(ucCmdLine_get_transmit_returns_transmit)
    ucCmdLine c;
    c.transmit = transmit_2;
    ucTRUE(transmit_2 == ucCmdLine_get_transmit(&c));
uc_PASS

uc_TEST(ucCmdLine_respond_uses_transmit)
    ucCmdLine c;
    c.is_quiet = ucBool_false;
    c.transmit = transmit_1;

    transmit_1_response = NULL;
    c.transmit = transmit_1;
    ucCmdLine_respond(&c, "expected response");
    ucTRUE(0 == strcmp(transmit_1_response, "expected response"));
uc_PASS

uc_TEST(ucCmdLine_get_is_canceled_returns_value)
    ucCmdLine c;
    c.is_canceled = is_canceled_1;
    ucTRUE(is_canceled_1 == ucCmdLine_get_is_canceled(&c));
uc_PASS

uc_TEST(ucCmdLine_set_is_canceled_sets_value)
    ucCmdLine c;
    ucCmdLine_set_is_canceled(&c, is_canceled_2);
    ucTRUE(is_canceled_2 == c.is_canceled);
uc_PASS

uc_TEST(ucCmdLine_is_canceled_calls_is_canceled)
    ucCmdLine c;
    c.is_canceled = is_canceled_1;

    is_canceled_1_returned_value = ucBool_true;
    ucTRUE(ucBool_true == ucCmdLine_is_canceled(&c));

    is_canceled_1_returned_value = ucBool_false;
    ucTRUE(ucBool_false == ucCmdLine_is_canceled(&c));
uc_PASS

uc_TEST(ucCmdLine_set_transmit_state_sets_value)
    int state;
    ucCmdLine *ptr = subject;
    void *prev_value = ptr->transmit_state;

    ucCmdLine_set_transmit_state(ptr, &state);
    ucTRUE(&state == ptr->transmit_state);

    ptr->transmit_state = prev_value;
uc_PASS

uc_TEST(ucCmdLine_get_transmit_state_gets_value)
    char *state = "state";
    ucCmdLine *ptr = subject;
    void *prev_value = ptr->transmit_state;

    ptr->transmit_state = &state;
    ucTRUE(&state == ucCmdLine_get_transmit_state(ptr));

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

    ucTRUE(&state == transmit_1_state);

    ptr->transmit = prev_func;
    ptr->transmit_state = prev_state;
uc_PASS

uc_TEST(ucCmdLine_set_is_canceled_state_sets_value)
    int state;
    ucCmdLine *ptr = subject;
    void *prev_value = ptr->is_canceled_state;

    ucCmdLine_set_is_canceled_state(ptr, &state);
    ucTRUE(&state == ptr->is_canceled_state);

    ptr->is_canceled_state = prev_value;
uc_PASS

uc_TEST(ucCmdLine_get_is_canceled_state_returns_value)
    char *state = "st";
    ucCmdLine *ptr = subject;
    void *prev_value = ptr->is_canceled_state;

    ptr->is_canceled_state = &state;
    ucTRUE(&state == ucCmdLine_get_is_canceled_state(ptr));

    ptr->is_canceled_state = prev_value;
uc_PASS

uc_TEST(ucCmdLine_is_canceled_uses_state)
    double state;
    ucCmdLine *ptr = subject;
    void *prev_value = ptr->is_canceled_state;

    ucCmdLine_set_is_canceled(ptr, is_canceled_1);
    ucCmdLine_set_is_canceled_state(ptr, &state);

    ucCmdLine_is_canceled(ptr);

    ucTRUE(&state == is_canceled_1_state);

    ptr->is_canceled_state = prev_value;
uc_PASS

uc_TEST(ucCmdLine_set_is_quiet_sets_value)
    ucCmdLine *ptr = subject;
    
    ucBool prev_val = ucCmdLine_get_is_quiet(ptr);
    ucTRUE(ucBool_false == prev_val);

    ucCmdLine_set_is_quiet(ptr, ucBool_true);
    ucTRUE(ucCmdLine_get_is_quiet(ptr) == ucBool_true);

    ucCmdLine_set_is_quiet(ptr, prev_val);
    ucTRUE(ucCmdLine_get_is_quiet(ptr) == prev_val);
uc_PASS

uc_TEST(ucCmdLine_respond_does_nothing_if_is_quiet)
    ucCmdLine *ptr = subject;
    ucBool pre_val = ucCmdLine_get_is_quiet(ptr);

    ucCmdLine_set_is_quiet(ptr, ucBool_true);
    ucCmdLine_set_transmit(ptr, transmit_1);

    transmit_1_response = NULL;
    ucCmdLine_respond(ptr, "expected response");
    ucTRUE(NULL == transmit_1_response);

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
    ucTRUE(handle_invalid_command_1_state == &state);
    ucTRUE(0 == strcmp("n-a", handle_invalid_command_1_invalid_command));
    ucTRUE(ucBool_true == ret_val);
uc_PASS

uc_TEST(ucCmdLine_get_response_size_max_returns_size)
    ucCmdLine *ptr = subject;
    size_t size = ucCmdLine_get_response_size_max(ptr);
    ucTRUE(ucCmdLine_RESPONSE_SIZE == size);
uc_PASS

uc_TEST(ucCmdLine_get_response_terminator_is_initially_null)
    ucTRUE(NULL == ucCmdLine_get_response_terminator(subject));
uc_PASS

uc_TEST(ucCmdLine_get_response_terminator_returns_set_value)
    const char *expected, *actual, *values[] = { "EOT", "\x1b", "We're DONE!" };
    int i;
    for (i = 0; i < 3; i++) {
        expected = values[i];
        ucCmdLine_set_response_terminator(subject, expected);
        actual = ucCmdLine_get_response_terminator(subject);
        ucTRUE(0 == strcmp(expected, actual));
        ucTRUE(expected == actual);
    }
uc_PASS

uc_TEST(ucCmdLine_get_command_acknowledgment_is_initially_null)
    ucTRUE(NULL == ucCmdLine_get_command_acknowledgment(subject));
uc_PASS

uc_TEST(ucCmdLine_get_command_acknoledgment_returns_set_value)
    const char *expected, *actual, *values[] = { "Got it!", "\x1b", "***" };
    int i;
    for (i = 0; i < 3; i++) {
        expected = values[i];
        ucCmdLine_set_command_acknowledgment(subject, expected);
        actual = ucCmdLine_get_command_acknowledgment(subject);
        ucTRUE(0 == strcmp(expected, actual));
        ucTRUE(expected == actual);
    }
uc_PASS

uc_TEST(ucCmdLine_get_arg_returns_arg_tok)
    char c[20] = "cmd argz -s argb";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    ucTRUE(uc_STR_EQ("argz", ucCmdLine_get_arg(subject)));
uc_PASS

uc_TEST(ucCmdLine_get_arg_returns_null_if_no_arg_exists)
    char c[15] = "cmd -s argb";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    ucTRUE(NULL == ucCmdLine_get_arg(subject));
uc_PASS

uc_TEST(ucCmdLine_get_switch_returns_switch_tok)
    char c[20] = "cmd argz -s argb";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    ucTRUE(uc_STR_EQ("-s", ucCmdLine_get_switch(subject)));
uc_PASS

uc_TEST(ucCmdLine_get_switch_returns_null_if_no_switch_exists)
    char c[20] = "cmd argz argb";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    ucTRUE(NULL == ucCmdLine_get_switch(subject));
uc_PASS

uc_TEST(ucCmdLine_get_switch_arg_returns_arg_tok_of_switch)
    char c[30] = "cmd argz -s1 argb -s2 argc";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    ucTRUE(uc_STR_EQ("argc", ucCmdLine_get_switch_arg(subject, "-s2")));
uc_PASS

uc_TEST(ucCmdLine_get_switch_arg_returns_null_if_arg_does_not_exist)
    char c[30] = "cmd argz -s1 argb -s2";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    ucTRUE(NULL == ucCmdLine_get_switch_arg(subject, "-s2"));
uc_PASS

uc_TEST(ucCmdLine_get_switch_arg_returns_null_if_switch_does_not_exist)
    char c[30] = "cmd argz -s1 argb";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    ucTRUE(NULL == ucCmdLine_get_switch_arg(subject, "-s2"));
uc_PASS

uc_TEST(ucCmdLine_find_switch_returns_switch_if_it_exists)
    char c[30] = "cmd argz -s1 argb -s2 c";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    ucTRUE(ucTok_equals(ucCmdLine_find_switch(subject, "-s2"), "-s2"));
uc_PASS

uc_TEST(ucCmdLine_find_switch_returns_null_if_it_does_not_exist)
    char c[30] = "cmd argz -s1 argb";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    ucTRUE(NULL == ucCmdLine_find_switch(subject, "-s2"));
uc_PASS

uc_TEST(ucCmdLine_get_switch_arg_d_returns_argument_as_integer)
    char c[30] = "cmd argz -s1 argb -s2 42";
    ucCmdLine_set_cmd_tok(subject, parse_cmd(c));
    uc_TRUE(42 == ucCmdLine_get_switch_arg_d(subject, "-s2", -1));
uc_PASS

static                                              
ucTestGroup_TestFunc*
junk1[] = { 
    ucCmdLine_create_does_not_return_null,
    ucCmdLine_find_switch_returns_null_if_it_does_not_exist,
    NULL };                                         
ucTestGroup                                        
junk2 = {                            
    "ucCmdLine",                                          
    junk1,                  
    NULL,                                          
    NULL,                                           
    NULL                                            
};       

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
    ucCmdLine_get_switch_arg_d_returns_argument_as_integer,
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
