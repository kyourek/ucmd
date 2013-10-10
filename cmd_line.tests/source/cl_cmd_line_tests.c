#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "cl_cmd_line_p.h"
#include "cl_cmd_line_tests.h"
#include "cl_cmd_line_toks.h"
#include "cl_cmd_parser_p.h"
#include "cl_cmd_tok.h"
#include "cl_switch_tok.h"
#include "cl_test.h"
#include "cl_tok.h"

static cl_cmd_tok *parse_cmd(char *cmd) {
    return cl_cmd_parser_parse(cl_cmd_parser_get_instance(), cmd);
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
static cl_bool is_cancelled_1_returned_value = CL_FALSE;
static cl_bool is_cancelled_1(void *state) {
    is_cancelled_1_state = state;
    return is_cancelled_1_returned_value;
}

static cl_bool is_cancelled_2(void *state) {
    return CL_TRUE;
}

static cl_test_err cl_cmd_line_get_cmd_tok_returns_cmd_tok(cl_test_group *p) {
    cl_cmd_line c;
    cl_cmd_tok *t = "token";
    c.cmd_tok = t;
    CL_TEST_ASSERT(t == cl_cmd_line_get_cmd_tok(&c));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_set_cmd_tok_sets_cmd_tok(cl_test_group *p) {
    cl_cmd_line c;
    cl_cmd_tok *t = "cmd_tok";
    cl_cmd_line_set_cmd_tok(&c, t);
    CL_TEST_ASSERT(t == c.cmd_tok);
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_get_cmd_toks_returns_toks(cl_test_group *p) {
    cl_cmd_line_toks toks = { 0 };
    cl_cmd_line *cmd = cl_cmd_line_get_instance();
    char c[30] = "cmd arg -s";

    cl_cmd_line_set_cmd_tok(cmd, parse_cmd(c));
    cl_cmd_line_get_cmd_toks(cmd, &toks);

    CL_TEST_ASSERT(cl_tok_equals(toks.cmd_tok, "cmd"));
    CL_TEST_ASSERT(cl_tok_equals(toks.arg_tok, "arg"));
    CL_TEST_ASSERT(cl_tok_equals(toks.switch_tok, "-s"));

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_get_cmd_toks_sets_null_tok_values(cl_test_group *p) {
    cl_cmd_line_toks toks = { 0 };
    cl_cmd_line *cmd = cl_cmd_line_get_instance();
    char c[12] = "command";

    cl_cmd_line_set_cmd_tok(cmd, parse_cmd(c));
    cl_cmd_line_get_cmd_toks(cmd, &toks);

    CL_TEST_ASSERT(cl_tok_equals(toks.cmd_tok, "command"));
    CL_TEST_ASSERT(NULL == toks.arg_tok);
    CL_TEST_ASSERT(NULL == toks.switch_tok);

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_get_instance_is_not_null(cl_test_group *p) {
    CL_TEST_ASSERT(NULL != cl_cmd_line_get_instance());
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_format_response_sets_response_string(cl_test_group *p) {
    char expected[50];
    cl_cmd_line *ptr = cl_cmd_line_get_instance();
    cl_cmd_line_format_response(ptr, "formatted %s %d %3.2f fin", "string", 10, 5.1234);
    sprintf(expected, "formatted %s %d %3.2f fin", "string", 10, 5.1234);
    CL_TEST_ASSERT(0 == strcmp(ptr->response, expected));
    return CL_TEST_ERR_NONE;
}

static const char *cl_cmd_line_format_response_va_sets_response_string_helper(cl_cmd_line *p, const char *format, ...) {
    va_list arg_list;
    const char *response;
    va_start(arg_list, format);
    response = cl_cmd_line_format_response_va(p, format, arg_list);
    va_end(arg_list);
    return response;
}

static cl_test_err cl_cmd_line_format_response_va_sets_response_string(cl_test_group *p) {
    char expected[50];
    const char *actual;
    cl_cmd_line *ptr = cl_cmd_line_get_instance();
    
    actual = cl_cmd_line_format_response_va_sets_response_string_helper(ptr, "This %d is %s formatted.", 1, "well");
    sprintf(expected, "This %d is %s formatted.", 1, "well");
    CL_TEST_ASSERT(0 == strcmp(actual, expected));
    CL_TEST_ASSERT(0 == strcmp(ptr->response, expected));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_set_transmit_sets_transmit(cl_test_group *p) {
    cl_cmd_line c;
    cl_cmd_line_set_transmit(&c, transmit_1);
    CL_TEST_ASSERT(transmit_1 == c.transmit);
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_get_transmit_returns_transmit(cl_test_group *p) {
    cl_cmd_line c;
    c.transmit = transmit_2;
    CL_TEST_ASSERT(transmit_2 == cl_cmd_line_get_transmit(&c));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_respond_uses_transmit(cl_test_group *p) {
    cl_cmd_line c;
    c.is_quiet = CL_FALSE;
    c.transmit = transmit_1;

    transmit_1_response = NULL;
    c.transmit = transmit_1;
    cl_cmd_line_respond(&c, "expected response");
    CL_TEST_ASSERT(0 == strcmp(transmit_1_response, "expected response"));

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_get_is_cancelled_returns_value(cl_test_group *p) {
    cl_cmd_line c;
    c.is_cancelled = is_cancelled_1;
    CL_TEST_ASSERT(is_cancelled_1 == cl_cmd_line_get_is_cancelled(&c));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_set_is_cancelled_sets_value(cl_test_group *p) {
    cl_cmd_line c;
    cl_cmd_line_set_is_cancelled(&c, is_cancelled_2);
    CL_TEST_ASSERT(is_cancelled_2 == c.is_cancelled);
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_is_cancelled_calls_is_cancelled(cl_test_group *p) {
    cl_cmd_line c;
    c.is_cancelled = is_cancelled_1;

    is_cancelled_1_returned_value = CL_TRUE;
    CL_TEST_ASSERT(CL_TRUE == cl_cmd_line_is_cancelled(&c));

    is_cancelled_1_returned_value = CL_FALSE;
    CL_TEST_ASSERT(CL_FALSE == cl_cmd_line_is_cancelled(&c));

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_set_transmit_state_sets_value(cl_test_group *p) {
    int state;
    cl_cmd_line *ptr = cl_cmd_line_get_instance();
    void *prev_value = ptr->transmit_state;

    cl_cmd_line_set_transmit_state(ptr, &state);
    CL_TEST_ASSERT(&state == ptr->transmit_state);

    ptr->transmit_state = prev_value;
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_get_transmit_state_gets_value(cl_test_group *p) {
    char *state = "state";
    cl_cmd_line *ptr = cl_cmd_line_get_instance();
    void *prev_value = ptr->transmit_state;

    ptr->transmit_state = &state;
    CL_TEST_ASSERT(&state == cl_cmd_line_get_transmit_state(ptr));

    ptr->transmit_state = prev_value;
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_transmit_uses_state(cl_test_group *p) {
    double state;
    cl_cmd_line *ptr = cl_cmd_line_get_instance();
    cl_cmd_line_transmit_func *prev_func = ptr->transmit;
    void *prev_state = ptr->transmit_state;

    cl_cmd_line_set_transmit(ptr, transmit_1);
    cl_cmd_line_set_transmit_state(ptr, &state);

    cl_cmd_line_respond(ptr, "resp");

    CL_TEST_ASSERT(&state == transmit_1_state);

    ptr->transmit = prev_func;
    ptr->transmit_state = prev_state;
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_set_is_cancelled_state_sets_value(cl_test_group *p) {
    int state;
    cl_cmd_line *ptr = cl_cmd_line_get_instance();
    void *prev_value = ptr->is_cancelled_state;

    cl_cmd_line_set_is_cancelled_state(ptr, &state);
    CL_TEST_ASSERT(&state == ptr->is_cancelled_state);

    ptr->is_cancelled_state = prev_value;
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_get_is_cancelled_state_returns_value(cl_test_group *p) {
    char *state = "st";
    cl_cmd_line *ptr = cl_cmd_line_get_instance();
    void *prev_value = ptr->is_cancelled_state;

    ptr->is_cancelled_state = &state;
    CL_TEST_ASSERT(&state == cl_cmd_line_get_is_cancelled_state(ptr));

    ptr->is_cancelled_state = prev_value;
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_is_cancelled_uses_state(cl_test_group *p) {
    double state;
    cl_cmd_line *ptr = cl_cmd_line_get_instance();
    void *prev_value = ptr->is_cancelled_state;

    cl_cmd_line_set_is_cancelled(ptr, is_cancelled_1);
    cl_cmd_line_set_is_cancelled_state(ptr, &state);

    cl_cmd_line_is_cancelled(ptr);

    CL_TEST_ASSERT(&state == is_cancelled_1_state);

    ptr->is_cancelled_state = prev_value;
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_set_is_quiet_sets_value(cl_test_group *p) {
    cl_cmd_line *ptr = cl_cmd_line_get_instance();
    
    cl_bool prev_val = cl_cmd_line_get_is_quiet(ptr);
    CL_TEST_ASSERT(CL_FALSE == prev_val);

    cl_cmd_line_set_is_quiet(ptr, CL_TRUE);
    CL_TEST_ASSERT(cl_cmd_line_get_is_quiet(ptr) == CL_TRUE);

    cl_cmd_line_set_is_quiet(ptr, prev_val);
    CL_TEST_ASSERT(cl_cmd_line_get_is_quiet(ptr) == prev_val);

    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_respond_does_nothing_if_is_quiet(cl_test_group *p) {
    cl_cmd_line *ptr = cl_cmd_line_get_instance();
    cl_bool pre_val = cl_cmd_line_get_is_quiet(ptr);

    cl_cmd_line_set_is_quiet(ptr, CL_TRUE);
    cl_cmd_line_set_transmit(ptr, transmit_1);

    transmit_1_response = NULL;
    cl_cmd_line_respond(ptr, "expected response");
    CL_TEST_ASSERT(NULL == transmit_1_response);

    cl_cmd_line_set_is_quiet(ptr, pre_val);
    return CL_TEST_ERR_NONE;
}

cl_test_group *cl_cmd_line_tests_get_group(void) {
    static cl_test_group group;
    static cl_test_group_test_func *tests[] = {
        cl_cmd_line_get_cmd_toks_returns_toks,
        cl_cmd_line_get_cmd_toks_sets_null_tok_values,
        cl_cmd_line_set_cmd_tok_sets_cmd_tok,
        cl_cmd_line_get_cmd_tok_returns_cmd_tok,
        cl_cmd_line_get_instance_is_not_null,
        cl_cmd_line_format_response_sets_response_string,
        cl_cmd_line_format_response_va_sets_response_string,
        cl_cmd_line_set_transmit_sets_transmit,
        cl_cmd_line_get_transmit_returns_transmit,
        cl_cmd_line_respond_uses_transmit,
        cl_cmd_line_get_is_cancelled_returns_value,
        cl_cmd_line_set_is_cancelled_sets_value,
        cl_cmd_line_is_cancelled_calls_is_cancelled,
        cl_cmd_line_set_transmit_state_sets_value,
        cl_cmd_line_get_transmit_state_gets_value,
        cl_cmd_line_transmit_uses_state,
        cl_cmd_line_set_is_cancelled_state_sets_value,
        cl_cmd_line_get_is_cancelled_state_returns_value,
        cl_cmd_line_is_cancelled_uses_state,
        cl_cmd_line_set_is_quiet_sets_value,
        cl_cmd_line_respond_does_nothing_if_is_quiet,
        NULL
    };

    return cl_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
