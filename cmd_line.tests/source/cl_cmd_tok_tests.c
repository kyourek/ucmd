#include "cl_cmd_tok.h"
#include "cl_cmd_tok_tests.h"
#include "cl_tests.h"
#include "cl_tok.h"

static char *cl_cmd_tok_get_arg_gets_arg(void) {
    char cmd[] = "command\0arg\0\n";
    cl_assert("cl_cmd_tok_get_arg did not return argument.", CL_TRUE == cl_tok_equals((cl_tok*)cl_cmd_tok_get_arg(cmd), "arg"));
    return 0;
}

static char *cl_cmd_tok_get_arg_returns_null_if_followed_by_switch(void) {
    char cmd[] = "cmd\0-s\0a\0\n";
    cl_assert("cl_cmd_tok_get_arg did not return null when followed by a switch.", NULL == cl_cmd_tok_get_arg(cmd));
    return 0;
}

static char *cl_cmd_tok_get_switch_gets_first_switch(void) {
    char cmd[] = "c\0a\0a2\0-s1\0-s2\0\n";
    cl_assert("cl_cmd_tok_get_switch did not return first switch.", cl_tok_equals((cl_tok*)cl_cmd_tok_get_switch(cmd), "-s1"));
    return 0;
}

static char *cl_cmd_tok_get_switch_returns_null_if_no_switch(void) {
    char cmd[] = "name\0a1\0ssss\0\n";
    cl_assert("cl_cmd_tok_get_switch did not return null.", NULL == cl_cmd_tok_get_switch(cmd));
    return 0;
}

char *cl_cmd_tok_tests(void) {
    cl_run_test(cl_cmd_tok_get_arg_gets_arg);
    cl_run_test(cl_cmd_tok_get_arg_returns_null_if_followed_by_switch);
    cl_run_test(cl_cmd_tok_get_switch_gets_first_switch);
    cl_run_test(cl_cmd_tok_get_switch_returns_null_if_no_switch);
    return 0;
}
