#include <stdlib.h>
#include "cl_cmd_line_toks.h"
#include "cl_cmd_line_toks_tests.h"
#include "cl_tests_p.h"

static CL_TESTS_ERR cl_cmd_line_toks_get_cmd_tok_returns_value(void) {
    cl_cmd_line_toks inst = { 0 };
    cl_cmd_line_toks *p = &inst;
    cl_cmd_tok *cmd_tok = "c";

    CL_TESTS_ASSERT(NULL == cl_cmd_line_toks_get_cmd_tok(NULL));
    CL_TESTS_ASSERT(NULL == cl_cmd_line_toks_get_cmd_tok(p));

    p->cmd_tok = cmd_tok;
    CL_TESTS_ASSERT(cmd_tok == cl_cmd_line_toks_get_cmd_tok(p));

    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_toks_get_arg_tok_returns_value(void) {
    cl_cmd_line_toks inst = { 0 };
    cl_cmd_line_toks *p = &inst;
    cl_arg_tok *arg_tok = "a";

    CL_TESTS_ASSERT(NULL == cl_cmd_line_toks_get_arg_tok(NULL));
    CL_TESTS_ASSERT(NULL == cl_cmd_line_toks_get_arg_tok(p));

    p->arg_tok = arg_tok;
    CL_TESTS_ASSERT(arg_tok == cl_cmd_line_toks_get_arg_tok(p));

    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_toks_get_switch_tok_returns_value(void) {
    cl_cmd_line_toks inst = { 0 };
    cl_cmd_line_toks *p = &inst;
    cl_arg_tok *switch_tok = "-s";

    CL_TESTS_ASSERT(NULL == cl_cmd_line_toks_get_switch_tok(NULL));
    CL_TESTS_ASSERT(NULL == cl_cmd_line_toks_get_switch_tok(p));

    p->switch_tok = switch_tok;
    CL_TESTS_ASSERT(switch_tok == cl_cmd_line_toks_get_switch_tok(p));

    return CL_TESTS_NO_ERR;
}

CL_TESTS_ERR cl_cmd_line_toks_tests(void) {
    CL_TESTS_RUN(cl_cmd_line_toks_get_cmd_tok_returns_value);
    CL_TESTS_RUN(cl_cmd_line_toks_get_arg_tok_returns_value);
    CL_TESTS_RUN(cl_cmd_line_toks_get_switch_tok_returns_value);
    return CL_TESTS_NO_ERR;
}
