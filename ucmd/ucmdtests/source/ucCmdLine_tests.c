#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "ucTest.h"

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

static void *is_cancelled_1_state;
static ucBool is_cancelled_1_returned_value = ucBool_FALSE;
static ucBool is_cancelled_1(void *state) {
    is_cancelled_1_state = state;
    return is_cancelled_1_returned_value;
}

static ucBool is_cancelled_2(void *state) {
    return ucBool_TRUE;
}

static ucTestErr ucCmdLine_get_cmd_tok_returns_cmd_tok(ucTestGroup *p) {
    ucCmdLine c;
    ucCmdTok *t = "token";
    c.cmd_tok = t;
    ucTest_ASSERT(t == ucCmdLine_get_cmd_tok(&c));
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLine_set_cmd_tok_sets_cmd_tok(ucTestGroup *p) {
    ucCmdLine c;
    ucCmdTok *t = "cmd_tok";
    ucCmdLine_set_cmd_tok(&c, t);
    ucTest_ASSERT(t == c.cmd_tok);
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLine_get_cmd_toks_returns_toks(ucTestGroup *p) {
    ucCmdLineToks toks = { 0 };
    ucCmdLine *cmd = ucCmdLine_get_instance();
    char c[30] = "cmd arg -s";

    ucCmdLine_set_cmd_tok(cmd, parse_cmd(c));
    ucCmdLine_get_cmd_toks(cmd, &toks);

    ucTest_ASSERT(ucTok_equals(toks.cmd_tok, "cmd"));
    ucTest_ASSERT(ucTok_equals(toks.arg_tok, "arg"));
    ucTest_ASSERT(ucTok_equals(toks.switch_tok, "-s"));

    return ucTestErr_NONE;
}

static ucTestErr ucCmdLine_get_cmd_toks_sets_null_tok_values(ucTestGroup *p) {
    ucCmdLineToks toks = { 0 };
    ucCmdLine *cmd = ucCmdLine_get_instance();
    char c[12] = "command";

    ucCmdLine_set_cmd_tok(cmd, parse_cmd(c));
    ucCmdLine_get_cmd_toks(cmd, &toks);

    ucTest_ASSERT(ucTok_equals(toks.cmd_tok, "command"));
    ucTest_ASSERT(NULL == toks.arg_tok);
    ucTest_ASSERT(NULL == toks.switch_tok);

    return ucTestErr_NONE;
}

static ucTestErr ucCmdLine_get_instance_is_not_null(ucTestGroup *p) {
    ucTest_ASSERT(NULL != ucCmdLine_get_instance());
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLine_format_response_sets_response_string(ucTestGroup *p) {
    char expected[50];
    ucCmdLine *ptr = ucCmdLine_get_instance();
    ucCmdLine_format_response(ptr, "formatted %s %d %3.2f fin", "string", 10, 5.1234);
    sprintf(expected, "formatted %s %d %3.2f fin", "string", 10, 5.1234);
    ucTest_ASSERT(0 == strcmp(ptr->response, expected));
    return ucTestErr_NONE;
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
    ucTest_ASSERT(0 == strcmp(actual, expected));
    ucTest_ASSERT(0 == strcmp(ptr->response, expected));
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLine_set_transmit_sets_transmit(ucTestGroup *p) {
    ucCmdLine c;
    ucCmdLine_set_transmit(&c, transmit_1);
    ucTest_ASSERT(transmit_1 == c.transmit);
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLine_get_transmit_returns_transmit(ucTestGroup *p) {
    ucCmdLine c;
    c.transmit = transmit_2;
    ucTest_ASSERT(transmit_2 == ucCmdLine_get_transmit(&c));
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLine_respond_uses_transmit(ucTestGroup *p) {
    ucCmdLine c;
    c.is_quiet = ucBool_FALSE;
    c.transmit = transmit_1;

    transmit_1_response = NULL;
    c.transmit = transmit_1;
    ucCmdLine_respond(&c, "expected response");
    ucTest_ASSERT(0 == strcmp(transmit_1_response, "expected response"));

    return ucTestErr_NONE;
}

static ucTestErr ucCmdLine_get_is_cancelled_returns_value(ucTestGroup *p) {
    ucCmdLine c;
    c.is_cancelled = is_cancelled_1;
    ucTest_ASSERT(is_cancelled_1 == ucCmdLine_get_is_cancelled(&c));
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLine_set_is_cancelled_sets_value(ucTestGroup *p) {
    ucCmdLine c;
    ucCmdLine_set_is_cancelled(&c, is_cancelled_2);
    ucTest_ASSERT(is_cancelled_2 == c.is_cancelled);
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLine_is_cancelled_calls_is_cancelled(ucTestGroup *p) {
    ucCmdLine c;
    c.is_cancelled = is_cancelled_1;

    is_cancelled_1_returned_value = ucBool_TRUE;
    ucTest_ASSERT(ucBool_TRUE == ucCmdLine_is_cancelled(&c));

    is_cancelled_1_returned_value = ucBool_FALSE;
    ucTest_ASSERT(ucBool_FALSE == ucCmdLine_is_cancelled(&c));

    return ucTestErr_NONE;
}

static ucTestErr ucCmdLine_set_transmit_state_sets_value(ucTestGroup *p) {
    int state;
    ucCmdLine *ptr = ucCmdLine_get_instance();
    void *prev_value = ptr->transmit_state;

    ucCmdLine_set_transmit_state(ptr, &state);
    ucTest_ASSERT(&state == ptr->transmit_state);

    ptr->transmit_state = prev_value;
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLine_get_transmit_state_gets_value(ucTestGroup *p) {
    char *state = "state";
    ucCmdLine *ptr = ucCmdLine_get_instance();
    void *prev_value = ptr->transmit_state;

    ptr->transmit_state = &state;
    ucTest_ASSERT(&state == ucCmdLine_get_transmit_state(ptr));

    ptr->transmit_state = prev_value;
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLine_transmit_uses_state(ucTestGroup *p) {
    double state;
    ucCmdLine *ptr = ucCmdLine_get_instance();
    ucCmdLine_TransmitFunc *prev_func = ptr->transmit;
    void *prev_state = ptr->transmit_state;

    ucCmdLine_set_transmit(ptr, transmit_1);
    ucCmdLine_set_transmit_state(ptr, &state);

    ucCmdLine_respond(ptr, "resp");

    ucTest_ASSERT(&state == transmit_1_state);

    ptr->transmit = prev_func;
    ptr->transmit_state = prev_state;
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLine_set_is_cancelled_state_sets_value(ucTestGroup *p) {
    int state;
    ucCmdLine *ptr = ucCmdLine_get_instance();
    void *prev_value = ptr->is_cancelled_state;

    ucCmdLine_set_is_cancelled_state(ptr, &state);
    ucTest_ASSERT(&state == ptr->is_cancelled_state);

    ptr->is_cancelled_state = prev_value;
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLine_get_is_cancelled_state_returns_value(ucTestGroup *p) {
    char *state = "st";
    ucCmdLine *ptr = ucCmdLine_get_instance();
    void *prev_value = ptr->is_cancelled_state;

    ptr->is_cancelled_state = &state;
    ucTest_ASSERT(&state == ucCmdLine_get_is_cancelled_state(ptr));

    ptr->is_cancelled_state = prev_value;
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLine_is_cancelled_uses_state(ucTestGroup *p) {
    double state;
    ucCmdLine *ptr = ucCmdLine_get_instance();
    void *prev_value = ptr->is_cancelled_state;

    ucCmdLine_set_is_cancelled(ptr, is_cancelled_1);
    ucCmdLine_set_is_cancelled_state(ptr, &state);

    ucCmdLine_is_cancelled(ptr);

    ucTest_ASSERT(&state == is_cancelled_1_state);

    ptr->is_cancelled_state = prev_value;
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLine_set_is_quiet_sets_value(ucTestGroup *p) {
    ucCmdLine *ptr = ucCmdLine_get_instance();
    
    ucBool prev_val = ucCmdLine_get_is_quiet(ptr);
    ucTest_ASSERT(ucBool_FALSE == prev_val);

    ucCmdLine_set_is_quiet(ptr, ucBool_TRUE);
    ucTest_ASSERT(ucCmdLine_get_is_quiet(ptr) == ucBool_TRUE);

    ucCmdLine_set_is_quiet(ptr, prev_val);
    ucTest_ASSERT(ucCmdLine_get_is_quiet(ptr) == prev_val);

    return ucTestErr_NONE;
}

static ucTestErr ucCmdLine_respond_does_nothing_if_is_quiet(ucTestGroup *p) {
    ucCmdLine *ptr = ucCmdLine_get_instance();
    ucBool pre_val = ucCmdLine_get_is_quiet(ptr);

    ucCmdLine_set_is_quiet(ptr, ucBool_TRUE);
    ucCmdLine_set_transmit(ptr, transmit_1);

    transmit_1_response = NULL;
    ucCmdLine_respond(ptr, "expected response");
    ucTest_ASSERT(NULL == transmit_1_response);

    ucCmdLine_set_is_quiet(ptr, pre_val);
    return ucTestErr_NONE;
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
        ucCmdLine_get_is_cancelled_returns_value,
        ucCmdLine_set_is_cancelled_sets_value,
        ucCmdLine_is_cancelled_calls_is_cancelled,
        ucCmdLine_set_transmit_state_sets_value,
        ucCmdLine_get_transmit_state_gets_value,
        ucCmdLine_transmit_uses_state,
        ucCmdLine_set_is_cancelled_state_sets_value,
        ucCmdLine_get_is_cancelled_state_returns_value,
        ucCmdLine_is_cancelled_uses_state,
        ucCmdLine_set_is_quiet_sets_value,
        ucCmdLine_respond_does_nothing_if_is_quiet,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
