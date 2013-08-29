#include <stdio.h>
#include <string.h>
#include "cl_tests.h"
#include "cl_cmd_parser.h"
#include "cl_cmd_parser_p.h"
#include "cl_cmd_tok.h"
#include "cl_common.h"
#include "cl_arg_opt.h"
#include "cl_cmd_tok.h"
#include "cl_switch_opt.h"
#include "cl_switch_tok.h"
#include "cl_cmd_line_opt.h"
#include "cl_tok.h"
#include "cl_cmd_line.h"
#include "cl_cmd_line_p.h"
#include "cl_cmd_line_tests.h"
#include "cl_cmd_line_toks.h"

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

static char *cl_cmd_line_get_cmd_tok_returns_cmd_tok(void) {
    cl_cmd_line c;
    cl_cmd_tok *t = "token";
    c.cmd_tok = t;
    cl_assert("cl_cmd_line_get_cmd_tok did not return cmd_tok.", t == cl_cmd_line_get_cmd_tok(&c));
    return 0;
}

static char *cl_cmd_line_set_cmd_tok_sets_cmd_tok(void) {
    cl_cmd_line c;
    cl_cmd_tok *t = "cmd_tok";
    cl_cmd_line_set_cmd_tok(&c, t);
    cl_assert("cl_cmd_line_set_cmd_tok did not set cmd_tok.", t == c.cmd_tok);
    return 0;
}

static char *cl_cmd_line_get_cmd_toks_returns_toks(void) {
    cl_cmd_line_toks toks = { 0 };
    cl_cmd_line *cmd = cl_cmd_line_get_instance();
    char c[30] = "cmd arg -s";

    cl_cmd_line_set_cmd_tok(cmd, parse_cmd(c));
    cl_cmd_line_get_cmd_toks(cmd, &toks);

    cl_assert("cl_cmd_line_get_cmd_toks did not get command token.", cl_tok_equals(toks.cmd_tok, "cmd"));
    cl_assert("cl_cmd_line_get_cmd_toks did not get argument token.", cl_tok_equals(toks.arg_tok, "arg"));
    cl_assert("cl_cmd_line_get_cmd_toks did not get switch token.", cl_tok_equals(toks.switch_tok, "-s"));

    return 0;
}

static char *cl_cmd_line_get_cmd_toks_sets_null_tok_values(void) {
    cl_cmd_line_toks toks = { 0 };
    cl_cmd_line *cmd = cl_cmd_line_get_instance();
    char c[12] = "command";

    cl_cmd_line_set_cmd_tok(cmd, parse_cmd(c));
    cl_cmd_line_get_cmd_toks(cmd, &toks);

    cl_assert("cl_cmd_line_get_cmd_toks did not get command token.", cl_tok_equals(toks.cmd_tok, "command"));
    cl_assert("cl_cmd_line_get_cmd_toks did not set null argument token.", NULL == toks.arg_tok);
    cl_assert("cl_cmd_line_get_cmd_toks did not set null switch token.", NULL == toks.switch_tok);

    return 0;
}

static char *cl_cmd_line_get_instance_is_not_null(void) {
    cl_assert("cl_cmd_line_get_instance did not return instance.", NULL != cl_cmd_line_get_instance());
    return 0;
}

static char *cl_cmd_line_format_response_sets_response_string(void) {
    cl_cmd_line *p = cl_cmd_line_get_instance();
    cl_cmd_line_format_response(p, "formatted %s %d %3.2f fin", "string", 10, 5.1234);
    cl_assert("cl_cmd_line_format_response did not set string.", 0 == strcmp(p->response, "formatted string 10 5.12 fin"));
    return 0;
}

static char *cl_cmd_line_set_transmit_sets_transmit(void) {
    cl_cmd_line c;
    cl_cmd_line_set_transmit(&c, transmit_1);
    cl_assert("cl_cmd_line_set_transmit did not set value.", transmit_1 == c.transmit);
    return 0;
}

static char *cl_cmd_line_get_transmit_returns_transmit(void) {
    cl_cmd_line c;
    c.transmit = transmit_2;
    cl_assert("cl_cmd_line_get_transmit did not return transmit.", transmit_2 == cl_cmd_line_get_transmit(&c));
    return 0;
}

static char *cl_cmd_line_respond_uses_transmit(void) {
    cl_cmd_line c;
    c.transmit = transmit_1;

    transmitted_1 = NULL;
    c.transmit = transmit_1;
    cl_cmd_line_respond(&c, "expected response");
    cl_assert("cl_cmd_line_respond did not use transmit.", 0 == strcmp(transmitted_1, "expected response"));

    return 0;
}

static char *cl_cmd_line_get_is_cancelled_returns_value(void) {
    cl_cmd_line c;
    c.is_cancelled = is_cancelled_1;
    cl_assert("cl_cmd_line_get_is_cancelled did not return value.", is_cancelled_1 == cl_cmd_line_get_is_cancelled(&c));
    return 0;
}

static char *cl_cmd_line_set_is_cancelled_sets_value(void) {
    cl_cmd_line c;
    cl_cmd_line_set_is_cancelled(&c, is_cancelled_2);
    cl_assert("cl_cmd_line_set_is_cancelled did not set value.", is_cancelled_2 == c.is_cancelled);
    return 0;
}

static char *cl_cmd_line_is_cancelled_calls_is_cancelled(void) {
    cl_cmd_line c;
    char *message = "cl_cmd_line_is_cancelled did not call is_cancelled.";
    c.is_cancelled = is_cancelled_1;

    cancel_1 = CL_TRUE;
    cl_assert(message, CL_TRUE == cl_cmd_line_is_cancelled(&c));

    cancel_1 = CL_FALSE;
    cl_assert(message, CL_FALSE == cl_cmd_line_is_cancelled(&c));

    return 0;
}

char *cl_cmd_line_tests(void) {
    cl_run_test(cl_cmd_line_get_cmd_toks_returns_toks);
    cl_run_test(cl_cmd_line_get_cmd_toks_sets_null_tok_values);
    cl_run_test(cl_cmd_line_set_cmd_tok_sets_cmd_tok);
    cl_run_test(cl_cmd_line_get_cmd_tok_returns_cmd_tok);
    cl_run_test(cl_cmd_line_get_instance_is_not_null);
    cl_run_test(cl_cmd_line_format_response_sets_response_string);
    cl_run_test(cl_cmd_line_set_transmit_sets_transmit);
    cl_run_test(cl_cmd_line_get_transmit_returns_transmit);
    cl_run_test(cl_cmd_line_respond_uses_transmit);
    cl_run_test(cl_cmd_line_get_is_cancelled_returns_value);
    cl_run_test(cl_cmd_line_set_is_cancelled_sets_value);
    cl_run_test(cl_cmd_line_is_cancelled_calls_is_cancelled);
    return 0;
}
