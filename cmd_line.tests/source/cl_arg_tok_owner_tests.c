#include "cl_arg_tok_owner.h"
#include "cl_arg_tok_owner_tests.h"
#include "cl_tests_p.h"
#include "cl_tok.h"

static cl_tests_err cl_arg_tok_owner_get_arg_returns_first_arg(void) {
    char cmd[] = "c\0a1\0a2\0\n";
    CL_TESTS_ASSERT(CL_TRUE == cl_tok_equals((cl_tok*)cl_arg_tok_owner_get_arg(cmd), "a1"));
    return CL_TESTS_ERR_NONE;
}

static cl_tests_err cl_arg_tok_owner_get_arg_returns_null(void) {
    char cmd[] = "cmd\0\n";
    CL_TESTS_ASSERT(NULL == cl_arg_tok_owner_get_arg(cmd));
    return CL_TESTS_ERR_NONE;
}

cl_tests_err cl_arg_tok_owner_tests(void) {
    CL_TESTS_RUN(cl_arg_tok_owner_get_arg_returns_first_arg);
    CL_TESTS_RUN(cl_arg_tok_owner_get_arg_returns_null);
    return CL_TESTS_ERR_NONE;
}
