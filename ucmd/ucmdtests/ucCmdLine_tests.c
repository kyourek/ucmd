#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "ucmdtests.h"

static ucCmdLine *init_subject(void) {
    return ucCmdLine_init(ucCmdLine_get_instance());
}

static ucCmdTok *parse_cmd(char *cmd) {
    return ucCmdParser_parse(ucCmdParser_get_instance(), cmd);
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
static ucBool is_canceled_1_returned_value = ucBool_FALSE;
static ucBool is_canceled_1(void *state) {
    is_canceled_1_state = state;
    return is_canceled_1_returned_value;
}

static ucBool is_canceled_2(void *state) {
    return ucBool_TRUE;
}

static void *handle_invalid_command_1_state;
static const char *handle_invalid_command_1_invalid_command;
static ucBool handle_invalid_command_1_return = ucBool_FALSE;
static ucBool handle_invalid_command_1(const char *invalid_command, void *state) {
    handle_invalid_command_1_state = state;
    handle_invalid_command_1_invalid_command = invalid_command;
    return handle_invalid_command_1_return;
}

static ucTestErr ucCmdLine_get_cmd_tok_returns_cmd_tok(ucTestGroup *p) {
    ucCmdLine c;
    ucCmdTok *t = "token";
    c.cmd_tok = t;
    ucTEST(t == ucCmdLine_get_cmd_tok(&c));
    ucPASS();
}

static ucTestErr ucCmdLine_set_cmd_tok_sets_cmd_tok(ucTestGroup *p) {
    ucCmdLine c;
    ucCmdTok *t = "cmd_tok";
    ucCmdLine_set_cmd_tok(&c, t);
    ucTEST(t == c.cmd_tok);
    ucPASS();
}

static ucTestErr ucCmdLine_get_cmd_toks_returns_toks(ucTestGroup *p) {
    ucCmdLineToks toks = { 0 };
    ucCmdLine *cmd = ucCmdLine_get_instance();
    char c[30] = "cmd arg -s";

    ucCmdLine_set_cmd_tok(cmd, parse_cmd(c));
    ucCmdLine_get_cmd_toks(cmd, &toks);

    ucTEST(ucTok_equals(toks.cmd_tok, "cmd"));
    ucTEST(ucTok_equals(toks.arg_tok, "arg"));
    ucTEST(ucTok_equals(toks.switch_tok, "-s"));
    ucPASS();
}

static ucTestErr ucCmdLine_get_cmd_toks_sets_null_tok_values(ucTestGroup *p) {
    ucCmdLineToks toks = { 0 };
    ucCmdLine *cmd = ucCmdLine_get_instance();
    char c[12] = "command";

    ucCmdLine_set_cmd_tok(cmd, parse_cmd(c));
    ucCmdLine_get_cmd_toks(cmd, &toks);

    ucTEST(ucTok_equals(toks.cmd_tok, "command"));
    ucTEST(NULL == toks.arg_tok);
    ucTEST(NULL == toks.switch_tok);
    ucPASS();
}

static ucTestErr ucCmdLine_get_instance_is_not_null(ucTestGroup *p) {
    ucTEST(NULL != ucCmdLine_get_instance());
    ucPASS();
}

static ucTestErr ucCmdLine_format_response_sets_response_string(ucTestGroup *p) {
    char expected[50];
    ucCmdLine *ptr = ucCmdLine_get_instance();
    ucCmdLine_format_response(ptr, "formatted %s %d %3.2f fin", "string", 10, 5.1234);
    sprintf(expected, "formatted %s %d %3.2f fin", "string", 10, 5.1234);
    ucTEST(0 == strcmp(ptr->response, expected));
    ucPASS();
}

static const char *ucCmdLine_format_response_va_sets_response_string_helper(ucCmdLine *p, const char *format, ...) {
    va_list arg_list;
    const char *response;
    va_start(arg_list, format);
    response = ucCmdLine_format_response_va(p, format, arg_list);
    va_end(arg_list);
    return response;
}

static ucTestErr ucCmdLine_format_response_va_sets_response_string(ucTestGroup *p) {
    char expected[50];
    const char *actual;
    ucCmdLine *ptr = ucCmdLine_get_instance();
    
    actual = ucCmdLine_format_response_va_sets_response_string_helper(ptr, "This %d is %s formatted.", 1, "well");
    sprintf(expected, "This %d is %s formatted.", 1, "well");
    ucTEST(0 == strcmp(actual, expected));
    ucTEST(0 == strcmp(ptr->response, expected));
    ucPASS();
}

static ucTestErr ucCmdLine_set_transmit_sets_transmit(ucTestGroup *p) {
    ucCmdLine c;
    ucCmdLine_set_transmit(&c, transmit_1);
    ucTEST(transmit_1 == c.transmit);
    ucPASS();
}

static ucTestErr ucCmdLine_get_transmit_returns_transmit(ucTestGroup *p) {
    ucCmdLine c;
    c.transmit = transmit_2;
    ucTEST(transmit_2 == ucCmdLine_get_transmit(&c));
    ucPASS();
}

static ucTestErr ucCmdLine_respond_uses_transmit(ucTestGroup *p) {
    ucCmdLine c;
    c.is_quiet = ucBool_FALSE;
    c.transmit = transmit_1;

    transmit_1_response = NULL;
    c.transmit = transmit_1;
    ucCmdLine_respond(&c, "expected response");
    ucTEST(0 == strcmp(transmit_1_response, "expected response"));

    ucPASS();
}

static ucTestErr ucCmdLine_get_is_canceled_returns_value(ucTestGroup *p) {
    ucCmdLine c;
    c.is_canceled = is_canceled_1;
    ucTEST(is_canceled_1 == ucCmdLine_get_is_canceled(&c));
    ucPASS();
}

static ucTestErr ucCmdLine_set_is_canceled_sets_value(ucTestGroup *p) {
    ucCmdLine c;
    ucCmdLine_set_is_canceled(&c, is_canceled_2);
    ucTEST(is_canceled_2 == c.is_canceled);
    ucPASS();
}

static ucTestErr ucCmdLine_is_canceled_calls_is_canceled(ucTestGroup *p) {
    ucCmdLine c;
    c.is_canceled = is_canceled_1;

    is_canceled_1_returned_value = ucBool_TRUE;
    ucTEST(ucBool_TRUE == ucCmdLine_is_canceled(&c));

    is_canceled_1_returned_value = ucBool_FALSE;
    ucTEST(ucBool_FALSE == ucCmdLine_is_canceled(&c));

    ucPASS();
}

static ucTestErr ucCmdLine_set_transmit_state_sets_value(ucTestGroup *p) {
    int state;
    ucCmdLine *ptr = ucCmdLine_get_instance();
    void *prev_value = ptr->transmit_state;

    ucCmdLine_set_transmit_state(ptr, &state);
    ucTEST(&state == ptr->transmit_state);

    ptr->transmit_state = prev_value;
    ucPASS();
}

static ucTestErr ucCmdLine_get_transmit_state_gets_value(ucTestGroup *p) {
    char *state = "state";
    ucCmdLine *ptr = ucCmdLine_get_instance();
    void *prev_value = ptr->transmit_state;

    ptr->transmit_state = &state;
    ucTEST(&state == ucCmdLine_get_transmit_state(ptr));

    ptr->transmit_state = prev_value;
    ucPASS();
}

static ucTestErr ucCmdLine_transmit_uses_state(ucTestGroup *p) {
    double state;
    ucCmdLine *ptr = ucCmdLine_get_instance();
    ucCmdLine_TransmitFunc *prev_func = ptr->transmit;
    void *prev_state = ptr->transmit_state;

    ucCmdLine_set_transmit(ptr, transmit_1);
    ucCmdLine_set_transmit_state(ptr, &state);

    ucCmdLine_respond(ptr, "resp");

    ucTEST(&state == transmit_1_state);

    ptr->transmit = prev_func;
    ptr->transmit_state = prev_state;
    ucPASS();
}

static ucTestErr ucCmdLine_set_is_canceled_state_sets_value(ucTestGroup *p) {
    int state;
    ucCmdLine *ptr = ucCmdLine_get_instance();
    void *prev_value = ptr->is_canceled_state;

    ucCmdLine_set_is_canceled_state(ptr, &state);
    ucTEST(&state == ptr->is_canceled_state);

    ptr->is_canceled_state = prev_value;
    ucPASS();
}

static ucTestErr ucCmdLine_get_is_canceled_state_returns_value(ucTestGroup *p) {
    char *state = "st";
    ucCmdLine *ptr = ucCmdLine_get_instance();
    void *prev_value = ptr->is_canceled_state;

    ptr->is_canceled_state = &state;
    ucTEST(&state == ucCmdLine_get_is_canceled_state(ptr));

    ptr->is_canceled_state = prev_value;
    ucPASS();
}

static ucTestErr ucCmdLine_is_canceled_uses_state(ucTestGroup *p) {
    double state;
    ucCmdLine *ptr = ucCmdLine_get_instance();
    void *prev_value = ptr->is_canceled_state;

    ucCmdLine_set_is_canceled(ptr, is_canceled_1);
    ucCmdLine_set_is_canceled_state(ptr, &state);

    ucCmdLine_is_canceled(ptr);

    ucTEST(&state == is_canceled_1_state);

    ptr->is_canceled_state = prev_value;
    ucPASS();
}

static ucTestErr ucCmdLine_set_is_quiet_sets_value(ucTestGroup *p) {
    ucCmdLine *ptr = ucCmdLine_get_instance();
    
    ucBool prev_val = ucCmdLine_get_is_quiet(ptr);
    ucTEST(ucBool_FALSE == prev_val);

    ucCmdLine_set_is_quiet(ptr, ucBool_TRUE);
    ucTEST(ucCmdLine_get_is_quiet(ptr) == ucBool_TRUE);

    ucCmdLine_set_is_quiet(ptr, prev_val);
    ucTEST(ucCmdLine_get_is_quiet(ptr) == prev_val);

    ucPASS();
}

static ucTestErr ucCmdLine_respond_does_nothing_if_is_quiet(ucTestGroup *p) {
    ucCmdLine *ptr = ucCmdLine_get_instance();
    ucBool pre_val = ucCmdLine_get_is_quiet(ptr);

    ucCmdLine_set_is_quiet(ptr, ucBool_TRUE);
    ucCmdLine_set_transmit(ptr, transmit_1);

    transmit_1_response = NULL;
    ucCmdLine_respond(ptr, "expected response");
    ucTEST(NULL == transmit_1_response);

    ucCmdLine_set_is_quiet(ptr, pre_val);
    ucPASS();
}

static ucTestErr ucCmdLine_set_handle_invalid_command_sets_callback(ucTestGroup *p) {
    int state;
    ucBool ret_val;
    ucCmdLine *ptr = ucCmdLine_get_instance();
    ucCmdLine_set_handle_invalid_command(ptr, handle_invalid_command_1);
    ucCmdLine_set_handle_invalid_command_state(ptr, &state);

    handle_invalid_command_1_return = ucBool_TRUE;
    ret_val = ucCmdLine_handle_invalid_command(ptr, "n-a");
    ucTEST(handle_invalid_command_1_state == &state);
    ucTEST(0 == strcmp("n-a", handle_invalid_command_1_invalid_command));
    ucTEST(ucBool_TRUE == ret_val);

    ucPASS();
}

static ucTestErr ucCmdLine_get_response_size_max_returns_size(ucTestGroup *p) {
    ucCmdLine *ptr = ucCmdLine_get_instance();
    size_t size = ucCmdLine_get_response_size_max(ptr);
    ucTEST(ucCmdLine_RESPONSE_SIZE == size);
    ucTEST(0 == ucCmdLine_get_response_size_max(NULL));
    ucPASS();
}

static ucTestErr ucCmdLine_get_response_terminator_is_initially_null(ucTestGroup *p) {
    ucCmdLine *subject = init_subject();
    ucTEST(NULL == ucCmdLine_get_response_terminator(subject));
    ucPASS();
}

static ucTestErr ucCmdLine_get_response_terminator_returns_set_value(ucTestGroup *p) {
    ucCmdLine *subject = init_subject();
    const char *expected, *actual, *values[] = { "EOT", "\x1b", "We're DONE!" };
    int i;
    for (i = 0; i < 3; i++) {
        expected = values[i];
        ucCmdLine_set_response_terminator(subject, expected);
        actual = ucCmdLine_get_response_terminator(subject);
        ucTEST(0 == strcmp(expected, actual));
        ucTEST(expected == actual);
    }
    ucPASS();
}

static ucTestErr ucCmdLine_get_command_acknowledgment_is_initially_null(ucTestGroup *p) {
    ucCmdLine *subject = init_subject();
    ucTEST(NULL == ucCmdLine_get_command_acknowledgment(subject));
    ucPASS();
}

static ucTestErr ucCmdLine_get_command_acknoledgment_returns_set_value(ucTestGroup *p) {
    ucCmdLine *subject = init_subject();
    const char *expected, *actual, *values[] = { "Got it!", "\x1b", "***" };
    int i;
    for (i = 0; i < 3; i++) {
        expected = values[i];
        ucCmdLine_set_command_acknowledgment(subject, expected);
        actual = ucCmdLine_get_command_acknowledgment(subject);
        ucTEST(0 == strcmp(expected, actual));
        ucTEST(expected == actual);
    }
    ucPASS();
}

ucTestGroup *ucCmdLine_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_TestFunc *tests[] = {
        ucCmdLine_get_cmd_toks_returns_toks,
        ucCmdLine_get_cmd_toks_sets_null_tok_values,
        ucCmdLine_set_cmd_tok_sets_cmd_tok,
        ucCmdLine_get_cmd_tok_returns_cmd_tok,
        ucCmdLine_get_instance_is_not_null,
        ucCmdLine_format_response_sets_response_string,
        ucCmdLine_format_response_va_sets_response_string,
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
        ucCmdLine_get_command_acknoledgment_returns_set_value,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
