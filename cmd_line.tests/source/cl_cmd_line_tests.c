#include <stdio.h>
#include <string.h>
#include "cl_cmd_line.h"
#include "cl_cmd_line_p.h"
#include "cl_cmd_line_tests.h"
#include "cl_cmd_line_toks.h"
#include "cl_cmd_parser.h"
#include "cl_cmd_parser_p.h"
#include "cl_cmd_tok.h"
#include "cl_common.h"
#include "cl_cmd_tok.h"
#include "cl_switch_tok.h"
#include "cl_tests.h"
#include "cl_tests_p.h"
#include "cl_tok.h"

static cl_cmd_tok *parse_cmd(char *cmd) {
    return cl_cmd_parser_parse(cl_cmd_parser_get_instance(), cmd);
}

static const char *transmitted_1;

static void transmit_1(const char *response) {
    transmitted_1 = response;
}

static void transmit_2(const char *response) {
}

static CL_BOOL cancel_1 = CL_FALSE;

static CL_BOOL is_cancelled_1(void) {
    return cancel_1;
}

static CL_BOOL is_cancelled_2(void) {
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

    transmitted_1 = NULL;
    c.transmit = transmit_1;
    cl_cmd_line_respond(&c, "expected response");
    CL_TESTS_ASSERT(0 == strcmp(transmitted_1, "expected response"));

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

    cancel_1 = CL_TRUE;
    CL_TESTS_ASSERT(CL_TRUE == cl_cmd_line_is_cancelled(&c));

    cancel_1 = CL_FALSE;
    CL_TESTS_ASSERT(CL_FALSE == cl_cmd_line_is_cancelled(&c));

    return CL_TESTS_NO_ERR;
}

CL_TESTS_ERR cl_cmd_line_tests(void) {
    CL_TESTS_RUN(cl_cmd_line_get_cmd_toks_returns_toks);
    CL_TESTS_RUN(cl_cmd_line_get_cmd_toks_sets_null_tok_values);
    CL_TESTS_RUN(cl_cmd_line_set_cmd_tok_sets_cmd_tok);
    CL_TESTS_RUN(cl_cmd_line_get_cmd_tok_returns_cmd_tok);
    CL_TESTS_RUN(cl_cmd_line_get_instance_is_not_null);
    CL_TESTS_RUN(cl_cmd_line_format_response_sets_response_string);
    CL_TESTS_RUN(cl_cmd_line_set_transmit_sets_transmit);
    CL_TESTS_RUN(cl_cmd_line_get_transmit_returns_transmit);
    CL_TESTS_RUN(cl_cmd_line_respond_uses_transmit);
    CL_TESTS_RUN(cl_cmd_line_get_is_cancelled_returns_value);
    CL_TESTS_RUN(cl_cmd_line_set_is_cancelled_sets_value);
    CL_TESTS_RUN(cl_cmd_line_is_cancelled_calls_is_cancelled);
    return CL_TESTS_NO_ERR;
}
