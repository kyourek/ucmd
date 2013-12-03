#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "uc_cmd_line_p.h"
#include "uc_cmd_line_tests.h"
#include "uc_cmd_line_toks.h"
#include "uc_cmd_parser_p.h"
#include "uc_cmd_tok.h"
#include "uc_switch_tok.h"
#include "uc_test.h"
#include "uc_tok.h"

static uc_cmd_tok *parse_cmd(char *cmd) {
    return uc_cmd_parser_parse(uc_cmd_parser_get_instance(), cmd);
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
static uc_bool is_cancelled_1_returned_value = UC_FALSE;
static uc_bool is_cancelled_1(void *state) {
    is_cancelled_1_state = state;
    return is_cancelled_1_returned_value;
}

static uc_bool is_cancelled_2(void *state) {
    return UC_TRUE;
}

static uc_test_err uc_cmd_line_get_cmd_tok_returns_cmd_tok(uc_test_group *p) {
    uc_cmd_line c;
    uc_cmd_tok *t = "token";
    c.cmd_tok = t;
    UC_TEST_ASSERT(t == uc_cmd_line_get_cmd_tok(&c));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_set_cmd_tok_sets_cmd_tok(uc_test_group *p) {
    uc_cmd_line c;
    uc_cmd_tok *t = "cmd_tok";
    uc_cmd_line_set_cmd_tok(&c, t);
    UC_TEST_ASSERT(t == c.cmd_tok);
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_get_cmd_toks_returns_toks(uc_test_group *p) {
    uc_cmd_line_toks toks = { 0 };
    uc_cmd_line *cmd = uc_cmd_line_get_instance();
    char c[30] = "cmd arg -s";

    uc_cmd_line_set_cmd_tok(cmd, parse_cmd(c));
    uc_cmd_line_get_cmd_toks(cmd, &toks);

    UC_TEST_ASSERT(uc_tok_equals(toks.cmd_tok, "cmd"));
    UC_TEST_ASSERT(uc_tok_equals(toks.arg_tok, "arg"));
    UC_TEST_ASSERT(uc_tok_equals(toks.switch_tok, "-s"));

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_get_cmd_toks_sets_null_tok_values(uc_test_group *p) {
    uc_cmd_line_toks toks = { 0 };
    uc_cmd_line *cmd = uc_cmd_line_get_instance();
    char c[12] = "command";

    uc_cmd_line_set_cmd_tok(cmd, parse_cmd(c));
    uc_cmd_line_get_cmd_toks(cmd, &toks);

    UC_TEST_ASSERT(uc_tok_equals(toks.cmd_tok, "command"));
    UC_TEST_ASSERT(NULL == toks.arg_tok);
    UC_TEST_ASSERT(NULL == toks.switch_tok);

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_get_instance_is_not_null(uc_test_group *p) {
    UC_TEST_ASSERT(NULL != uc_cmd_line_get_instance());
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_format_response_sets_response_string(uc_test_group *p) {
    char expected[50];
    uc_cmd_line *ptr = uc_cmd_line_get_instance();
    uc_cmd_line_format_response(ptr, "formatted %s %d %3.2f fin", "string", 10, 5.1234);
    sprintf(expected, "formatted %s %d %3.2f fin", "string", 10, 5.1234);
    UC_TEST_ASSERT(0 == strcmp(ptr->response, expected));
    return UC_TEST_ERR_NONE;
}

static const char *uc_cmd_line_format_response_va_sets_response_string_helper(uc_cmd_line *p, const char *format, ...) {
    va_list arg_list;
    const char *response;
    va_start(arg_list, format);
    response = uc_cmd_line_format_response_va(p, format, arg_list);
    va_end(arg_list);
    return response;
}

static uc_test_err uc_cmd_line_format_response_va_sets_response_string(uc_test_group *p) {
    char expected[50];
    const char *actual;
    uc_cmd_line *ptr = uc_cmd_line_get_instance();
    
    actual = uc_cmd_line_format_response_va_sets_response_string_helper(ptr, "This %d is %s formatted.", 1, "well");
    sprintf(expected, "This %d is %s formatted.", 1, "well");
    UC_TEST_ASSERT(0 == strcmp(actual, expected));
    UC_TEST_ASSERT(0 == strcmp(ptr->response, expected));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_set_transmit_sets_transmit(uc_test_group *p) {
    uc_cmd_line c;
    uc_cmd_line_set_transmit(&c, transmit_1);
    UC_TEST_ASSERT(transmit_1 == c.transmit);
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_get_transmit_returns_transmit(uc_test_group *p) {
    uc_cmd_line c;
    c.transmit = transmit_2;
    UC_TEST_ASSERT(transmit_2 == uc_cmd_line_get_transmit(&c));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_respond_uses_transmit(uc_test_group *p) {
    uc_cmd_line c;
    c.is_quiet = UC_FALSE;
    c.transmit = transmit_1;

    transmit_1_response = NULL;
    c.transmit = transmit_1;
    uc_cmd_line_respond(&c, "expected response");
    UC_TEST_ASSERT(0 == strcmp(transmit_1_response, "expected response"));

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_get_is_cancelled_returns_value(uc_test_group *p) {
    uc_cmd_line c;
    c.is_cancelled = is_cancelled_1;
    UC_TEST_ASSERT(is_cancelled_1 == uc_cmd_line_get_is_cancelled(&c));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_set_is_cancelled_sets_value(uc_test_group *p) {
    uc_cmd_line c;
    uc_cmd_line_set_is_cancelled(&c, is_cancelled_2);
    UC_TEST_ASSERT(is_cancelled_2 == c.is_cancelled);
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_is_cancelled_calls_is_cancelled(uc_test_group *p) {
    uc_cmd_line c;
    c.is_cancelled = is_cancelled_1;

    is_cancelled_1_returned_value = UC_TRUE;
    UC_TEST_ASSERT(UC_TRUE == uc_cmd_line_is_cancelled(&c));

    is_cancelled_1_returned_value = UC_FALSE;
    UC_TEST_ASSERT(UC_FALSE == uc_cmd_line_is_cancelled(&c));

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_set_transmit_state_sets_value(uc_test_group *p) {
    int state;
    uc_cmd_line *ptr = uc_cmd_line_get_instance();
    void *prev_value = ptr->transmit_state;

    uc_cmd_line_set_transmit_state(ptr, &state);
    UC_TEST_ASSERT(&state == ptr->transmit_state);

    ptr->transmit_state = prev_value;
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_get_transmit_state_gets_value(uc_test_group *p) {
    char *state = "state";
    uc_cmd_line *ptr = uc_cmd_line_get_instance();
    void *prev_value = ptr->transmit_state;

    ptr->transmit_state = &state;
    UC_TEST_ASSERT(&state == uc_cmd_line_get_transmit_state(ptr));

    ptr->transmit_state = prev_value;
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_transmit_uses_state(uc_test_group *p) {
    double state;
    uc_cmd_line *ptr = uc_cmd_line_get_instance();
    uc_cmd_line_transmit_func *prev_func = ptr->transmit;
    void *prev_state = ptr->transmit_state;

    uc_cmd_line_set_transmit(ptr, transmit_1);
    uc_cmd_line_set_transmit_state(ptr, &state);

    uc_cmd_line_respond(ptr, "resp");

    UC_TEST_ASSERT(&state == transmit_1_state);

    ptr->transmit = prev_func;
    ptr->transmit_state = prev_state;
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_set_is_cancelled_state_sets_value(uc_test_group *p) {
    int state;
    uc_cmd_line *ptr = uc_cmd_line_get_instance();
    void *prev_value = ptr->is_cancelled_state;

    uc_cmd_line_set_is_cancelled_state(ptr, &state);
    UC_TEST_ASSERT(&state == ptr->is_cancelled_state);

    ptr->is_cancelled_state = prev_value;
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_get_is_cancelled_state_returns_value(uc_test_group *p) {
    char *state = "st";
    uc_cmd_line *ptr = uc_cmd_line_get_instance();
    void *prev_value = ptr->is_cancelled_state;

    ptr->is_cancelled_state = &state;
    UC_TEST_ASSERT(&state == uc_cmd_line_get_is_cancelled_state(ptr));

    ptr->is_cancelled_state = prev_value;
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_is_cancelled_uses_state(uc_test_group *p) {
    double state;
    uc_cmd_line *ptr = uc_cmd_line_get_instance();
    void *prev_value = ptr->is_cancelled_state;

    uc_cmd_line_set_is_cancelled(ptr, is_cancelled_1);
    uc_cmd_line_set_is_cancelled_state(ptr, &state);

    uc_cmd_line_is_cancelled(ptr);

    UC_TEST_ASSERT(&state == is_cancelled_1_state);

    ptr->is_cancelled_state = prev_value;
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_set_is_quiet_sets_value(uc_test_group *p) {
    uc_cmd_line *ptr = uc_cmd_line_get_instance();
    
    uc_bool prev_val = uc_cmd_line_get_is_quiet(ptr);
    UC_TEST_ASSERT(UC_FALSE == prev_val);

    uc_cmd_line_set_is_quiet(ptr, UC_TRUE);
    UC_TEST_ASSERT(uc_cmd_line_get_is_quiet(ptr) == UC_TRUE);

    uc_cmd_line_set_is_quiet(ptr, prev_val);
    UC_TEST_ASSERT(uc_cmd_line_get_is_quiet(ptr) == prev_val);

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_respond_does_nothing_if_is_quiet(uc_test_group *p) {
    uc_cmd_line *ptr = uc_cmd_line_get_instance();
    uc_bool pre_val = uc_cmd_line_get_is_quiet(ptr);

    uc_cmd_line_set_is_quiet(ptr, UC_TRUE);
    uc_cmd_line_set_transmit(ptr, transmit_1);

    transmit_1_response = NULL;
    uc_cmd_line_respond(ptr, "expected response");
    UC_TEST_ASSERT(NULL == transmit_1_response);

    uc_cmd_line_set_is_quiet(ptr, pre_val);
    return UC_TEST_ERR_NONE;
}

uc_test_group *uc_cmd_line_tests_get_group(void) {
    static uc_test_group group;
    static uc_test_group_test_func *tests[] = {
        uc_cmd_line_get_cmd_toks_returns_toks,
        uc_cmd_line_get_cmd_toks_sets_null_tok_values,
        uc_cmd_line_set_cmd_tok_sets_cmd_tok,
        uc_cmd_line_get_cmd_tok_returns_cmd_tok,
        uc_cmd_line_get_instance_is_not_null,
        uc_cmd_line_format_response_sets_response_string,
        uc_cmd_line_format_response_va_sets_response_string,
        uc_cmd_line_set_transmit_sets_transmit,
        uc_cmd_line_get_transmit_returns_transmit,
        uc_cmd_line_respond_uses_transmit,
        uc_cmd_line_get_is_cancelled_returns_value,
        uc_cmd_line_set_is_cancelled_sets_value,
        uc_cmd_line_is_cancelled_calls_is_cancelled,
        uc_cmd_line_set_transmit_state_sets_value,
        uc_cmd_line_get_transmit_state_gets_value,
        uc_cmd_line_transmit_uses_state,
        uc_cmd_line_set_is_cancelled_state_sets_value,
        uc_cmd_line_get_is_cancelled_state_returns_value,
        uc_cmd_line_is_cancelled_uses_state,
        uc_cmd_line_set_is_quiet_sets_value,
        uc_cmd_line_respond_does_nothing_if_is_quiet,
        NULL
    };

    return uc_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
