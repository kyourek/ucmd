#include "cl_arg_tok_owner.h"
#include "cl_arg_tok_owner_tests.h"
#include "cl_tests.h"
#include "cl_tok.h"

static char *cl_arg_tok_owner_get_arg_returns_first_arg(void) {
    char cmd[] = "c\0a1\0a2\0\n";
    cl_assert("cl_arg_tok_owner_get_arg did not return first arg.", CL_TRUE == cl_tok_equals((cl_tok*)cl_arg_tok_owner_get_arg(cmd), "a1"));
    return 0;
}

static char *cl_arg_tok_owner_get_arg_returns_null(void) {
    char cmd[] = "cmd\0\n";
    cl_assert("cl_arg_tok_owner_get_arg did not return null.", NULL == cl_arg_tok_owner_get_arg(cmd));
    return 0;
}

char *cl_arg_tok_owner_tests(void) {
    cl_run_test(cl_arg_tok_owner_get_arg_returns_first_arg);
    cl_run_test(cl_arg_tok_owner_get_arg_returns_null);
    return 0;
}
