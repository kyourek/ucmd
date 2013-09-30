#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "cl_cmd_line_p.h"
#include "cl_cmd_line_tests.h"
#include "cl_cmd_line_toks.h"
#include "cl_cmd_parser_p.h"
#include "cl_cmd_tok.h"
#include "cl_switch_tok.h"
#include "cl_tests_p.h"
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
static CL_BOOL is_cancelled_1_returned_value = CL_FALSE;
static CL_BOOL is_cancelled_1(void *state) {
    is_cancelled_1_state = state;
    return is_cancelled_1_returned_value;
}

static CL_BOOL is_cancelled_2(void *state) {
    return CL_TRUE;
}

static CL_TESTS_ERR cl_cmd_line_get_cmd_tok_returns_cmd_tok(void) {
    cl_cmd_line c;
    cl_cmd_tok *t = "token";
    c.cmd_tok = t;
    CL_TESTS_ASSERT(t == cl_cmd_line_get_cmd_tok(&c));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_set_cmd_tok_sets_cmd_tok(void) {
    cl_cmd_line c;
    cl_cmd_tok *t = "cmd_tok";
    cl_cmd_line_set_cmd_tok(&c, t);
    CL_TESTS_ASSERT(t == c.cmd_tok);
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_get_cmd_toks_returns_toks(void) {
    cl_cmd_line_toks toks = { 0 };
    cl_cmd_line *cmd = cl_cmd_line_get_instance();
    char c[30] = "cmd arg -s";

    cl_cmd_line_set_cmd_tok(cmd, parse_cmd(c));
    cl_cmd_line_get_cmd_toks(cmd, &toks);

    CL_TESTS_ASSERT(cl_tok_equals(toks.cmd_tok, "cmd"));
    CL_TESTS_ASSERT(cl_tok_equals(toks.arg_tok, "arg"));
    CL_TESTS_ASSERT(cl_tok_equals(toks.switch_tok, "-s"));

    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_get_cmd_toks_sets_null_tok_values(void) {
    cl_cmd_line_toks toks = { 0 };
    cl_cmd_line *cmd = cl_cmd_line_get_instance();
    char c[12] = "command";

    cl_cmd_line_set_cmd_tok(cmd, parse_cmd(c));
    cl_cmd_line_get_cmd_toks(cmd, &toks);

    CL_TESTS_ASSERT(cl_tok_equals(toks.cmd_tok, "command"));
    CL_TESTS_ASSERT(NULL == toks.arg_tok);
    CL_TESTS_ASSERT(NULL == toks.switch_tok);

    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_get_instance_is_not_null(void) {
    CL_TESTS_ASSERT(NULL != cl_cmd_line_get_instance());
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_format_response_sets_response_string(void) {
    char expected[50];
    cl_cmd_line *p = cl_cmd_line_get_instance();
    cl_cmd_line_format_response(p, "formatted %s %d %3.2f fin", "string", 10, 5.1234);
    sprintf(expected, "formatted %s %d %3.2f fin", "string", 10, 5.1234);
    CL_TESTS_ASSERT(0 == strcmp(p->response, expected));
    return CL_TESTS_NO_ERR;
}

static const char *cl_cmd_line_format_response_va_sets_response_string_helper(cl_cmd_line *p, const char *format, ...) {
    va_list arg_list;
    const char *response;
    va_start(arg_list, format);
    response = cl_cmd_line_format_response_va(p, format, arg_list);
    va_end(arg_list);
    return response;
}

static CL_TESTS_ERR cl_cmd_line_format_response_va_sets_response_string(void) {
    char expected[50];
    const char *actual;
    cl_cmd_line *p = cl_cmd_line_get_instance();
    
    actual = cl_cmd_line_format_response_va_sets_response_string_helper(p, "This %d is %s formatted.", 1, "well");
    sprintf(expected, "This %d is %s formatted.", 1, "well");
    CL_TESTS_ASSERT(0 == strcmp(actual, expected));
    CL_TESTS_ASSERT(0 == strcmp(p->response, expected));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_set_transmit_sets_transmit(void) {
    cl_cmd_line c;
    cl_cmd_line_set_transmit(&c, transmit_1);
    CL_TESTS_ASSERT(transmit_1 == c.transmit);
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_get_transmit_returns_transmit(void) {
    cl_cmd_line c;
    c.transmit = transmit_2;
    CL_TESTS_ASSERT(transmit_2 == cl_cmd_line_get_transmit(&c));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_respond_uses_transmit(void) {
    cl_cmd_line c;
    c.transmit = transmit_1;

    transmit_1_response = NULL;
    c.transmit = transmit_1;
    cl_cmd_line_respond(&c, "expected response");
    CL_TESTS_ASSERT(0 == strcmp(transmit_1_response, "expected response"));

    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_get_is_cancelled_returns_value(void) {
    cl_cmd_line c;
    c.is_cancelled = is_cancelled_1;
    CL_TESTS_ASSERT(is_cancelled_1 == cl_cmd_line_get_is_cancelled(&c));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_set_is_cancelled_sets_value(void) {
    cl_cmd_line c;
    cl_cmd_line_set_is_cancelled(&c, is_cancelled_2);
    CL_TESTS_ASSERT(is_cancelled_2 == c.is_cancelled);
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_is_cancelled_calls_is_cancelled(void) {
    cl_cmd_line c;
    c.is_cancelled = is_cancelled_1;

    is_cancelled_1_returned_value = CL_TRUE;
    CL_TESTS_ASSERT(CL_TRUE == cl_cmd_line_is_cancelled(&c));

    is_cancelled_1_returned_value = CL_FALSE;
    CL_TESTS_ASSERT(CL_FALSE == cl_cmd_line_is_cancelled(&c));

    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_set_transmit_state_sets_value(void) {
    int state;
    cl_cmd_line *p = cl_cmd_line_get_instance();
    void *prev_value = p->transmit_state;

    cl_cmd_line_set_transmit_state(p, &state);
    CL_TESTS_ASSERT(&state == p->transmit_state);

    p->transmit_state = prev_value;
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_get_transmit_state_gets_value(void) {
    char *state = "state";
    cl_cmd_line *p = cl_cmd_line_get_instance();
    void *prev_value = p->transmit_state;

    p->transmit_state = &state;
    CL_TESTS_ASSERT(&state == cl_cmd_line_get_transmit_state(p));

    p->transmit_state = prev_value;
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_transmit_uses_state(void) {
    double state;
    cl_cmd_line *p = cl_cmd_line_get_instance();
    cl_cmd_line_transmit_func *prev_func = p->transmit;
    void *prev_state = p->transmit_state;

    cl_cmd_line_set_transmit(p, transmit_1);
    cl_cmd_line_set_transmit_state(p, &state);

    cl_cmd_line_respond(p, "resp");

    CL_TESTS_ASSERT(&state == transmit_1_state);

    p->transmit = prev_func;
    p->transmit_state = prev_state;
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_set_is_cancelled_state_sets_value(void) {
    int state;
    cl_cmd_line *p = cl_cmd_line_get_instance();
    void *prev_value = p->is_cancelled_state;

    cl_cmd_line_set_is_cancelled_state(p, &state);
    CL_TESTS_ASSERT(&state == p->is_cancelled_state);

    p->is_cancelled_state = prev_value;
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_get_is_cancelled_state_returns_value(void) {
    char *state = "st";
    cl_cmd_line *p = cl_cmd_line_get_instance();
    void *prev_value = p->is_cancelled_state;

    p->is_cancelled_state = &state;
    CL_TESTS_ASSERT(&state == cl_cmd_line_get_is_cancelled_state(p));

    p->is_cancelled_state = prev_value;
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_is_cancelled_uses_state(void) {
    double state;
    cl_cmd_line *p = cl_cmd_line_get_instance();
    void *prev_value = p->is_cancelled_state;

    cl_cmd_line_set_is_cancelled(p, is_cancelled_1);
    cl_cmd_line_set_is_cancelled_state(p, &state);

    cl_cmd_line_is_cancelled(p);

    CL_TESTS_ASSERT(&state == is_cancelled_1_state);

    p->is_cancelled_state = prev_value;
    return CL_TESTS_NO_ERR;
}

CL_TESTS_ERR cl_cmd_line_tests(void) {
    CL_TESTS_RUN(cl_cmd_line_get_cmd_toks_returns_toks);
    CL_TESTS_RUN(cl_cmd_line_get_cmd_toks_sets_null_tok_values);
    CL_TESTS_RUN(cl_cmd_line_set_cmd_tok_sets_cmd_tok);
    CL_TESTS_RUN(cl_cmd_line_get_cmd_tok_returns_cmd_tok);
    CL_TESTS_RUN(cl_cmd_line_get_instance_is_not_null);
    CL_TESTS_RUN(cl_cmd_line_format_response_sets_response_string);
    CL_TESTS_RUN(cl_cmd_line_format_response_va_sets_response_string);
    CL_TESTS_RUN(cl_cmd_line_set_transmit_sets_transmit);
    CL_TESTS_RUN(cl_cmd_line_get_transmit_returns_transmit);
    CL_TESTS_RUN(cl_cmd_line_respond_uses_transmit);
    CL_TESTS_RUN(cl_cmd_line_get_is_cancelled_returns_value);
    CL_TESTS_RUN(cl_cmd_line_set_is_cancelled_sets_value);
    CL_TESTS_RUN(cl_cmd_line_is_cancelled_calls_is_cancelled);
    CL_TESTS_RUN(cl_cmd_line_set_transmit_state_sets_value);
    CL_TESTS_RUN(cl_cmd_line_get_transmit_state_gets_value);
    CL_TESTS_RUN(cl_cmd_line_transmit_uses_state);
    CL_TESTS_RUN(cl_cmd_line_set_is_cancelled_state_sets_value);
    CL_TESTS_RUN(cl_cmd_line_get_is_cancelled_state_returns_value);
    CL_TESTS_RUN(cl_cmd_line_is_cancelled_uses_state);
    return CL_TESTS_NO_ERR;
}
