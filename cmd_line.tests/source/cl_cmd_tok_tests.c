#include "cl_cmd_tok.h"
#include "cl_cmd_tok_tests.h"
#include "cl_tests.h"
#include "cl_tests_p.h"
#include "cl_tok.h"

static CL_TESTS_ERR cl_cmd_tok_get_arg_gets_arg(void) {
    char cmd[] = "command\0arg\0\n";
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_cmd_tok_get_arg(cmd), "arg"));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_tok_get_arg_returns_null_if_followed_by_switch(void) {
    char cmd[] = "cmd\0-s\0a\0\n";
    CL_TESTS_ASSERT(NULL == cl_cmd_tok_get_arg(cmd));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_tok_get_switch_gets_first_switch(void) {
    char cmd[] = "c\0a\0a2\0-s1\0-s2\0\n";
    CL_TESTS_ASSERT(cl_tok_equals((cl_tok*)cl_cmd_tok_get_switch(cmd), "-s1"));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_tok_get_switch_returns_null_if_no_switch(void) {
    char cmd[] = "name\0a1\0ssss\0\n";
    CL_TESTS_ASSERT(NULL == cl_cmd_tok_get_switch(cmd));
    return CL_TESTS_NO_ERR;
}

CL_TESTS_ERR cl_cmd_tok_tests(void) {
    CL_TESTS_RUN(cl_cmd_tok_get_arg_gets_arg);
    CL_TESTS_RUN(cl_cmd_tok_get_arg_returns_null_if_followed_by_switch);
    CL_TESTS_RUN(cl_cmd_tok_get_switch_gets_first_switch);
    CL_TESTS_RUN(cl_cmd_tok_get_switch_returns_null_if_no_switch);
    return CL_TESTS_NO_ERR;
}
