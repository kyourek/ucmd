#include <string.h>
#include "cl_cmd_line_opt.h"
#include "cl_cmd_line_opt_p.h"
#include "cl_cmd_line_opt_tests.h"
#include "cl_tests.h"

static const char *uart_func_one(cl_cmd_line *p, void *state) {
    return CL_TESTS_NO_ERR;
}

static const char *uart_func_two(cl_cmd_line *p, void *state) {
    return "The result of uart_func_two.";
}

static cl_tests_err cl_cmd_line_opt_create_creates_structure(void) {
    int state;
    cl_arg_opt *arg_opt;
    cl_switch_opt* switch_opt;
    cl_cmd_line_opt *p;

    p = cl_cmd_line_opt_create(
        uart_func_one, &state, "uart_func_one", "The first function.",
            cl_arg_opt_create("arg1", "The first arg.",
            cl_arg_opt_create("arg2", "The second arg.", NULL)),
            cl_switch_opt_create("sw1", "The first switch.",
                cl_arg_opt_create("sw1arg1", "The first arg of sw1.",
                cl_arg_opt_create("sw1arg2", "The second arg of sw1.", NULL)),
            cl_switch_opt_create("sw2", "The second switch.",
                cl_arg_opt_create("sw2arg1", "The first arg of sw2.",
                cl_arg_opt_create("sw2arg2", "The second arg of sw2.", NULL)),
            NULL)),
        NULL
    );

    CL_TESTS_ASSERT(NULL != p);
    CL_TESTS_ASSERT(uart_func_one == cl_cmd_line_opt_get_func(p));
    CL_TESTS_ASSERT(&state == cl_cmd_line_opt_get_state(p));
    CL_TESTS_ASSERT(CL_TRUE == cl_opt_is_required((cl_opt*)p));

    arg_opt = cl_arg_opt_owner_get_arg_opt((cl_arg_opt_owner*)p);
    CL_TESTS_ASSERT(cl_opt_get_name((cl_opt*)arg_opt));
    CL_TESTS_ASSERT(cl_opt_get_desc((cl_opt*)arg_opt));

    arg_opt = cl_arg_opt_get_next(arg_opt);
    CL_TESTS_ASSERT(cl_opt_get_name((cl_opt*)arg_opt));
    CL_TESTS_ASSERT(cl_opt_get_desc((cl_opt*)arg_opt));

    CL_TESTS_ASSERT(NULL == cl_arg_opt_get_next(arg_opt));

    switch_opt = cl_cmd_line_opt_get_switch_opt(p);
    CL_TESTS_ASSERT(cl_opt_get_name((cl_opt*)switch_opt));
    CL_TESTS_ASSERT(cl_opt_get_desc((cl_opt*)switch_opt));

    arg_opt = cl_arg_opt_owner_get_arg_opt((cl_arg_opt_owner*)switch_opt);
    CL_TESTS_ASSERT(cl_opt_get_name((cl_opt*)arg_opt));
    CL_TESTS_ASSERT(cl_opt_get_desc((cl_opt*)arg_opt));

    // TODO: Finish making assertions here.

    cl_cmd_line_opt_destroy_chain(p);

    return CL_TESTS_NO_ERR;
}

static cl_tests_err cl_cmd_line_opt_process_calls_func(void) {
    cl_cmd_line *cmd;
    cl_cmd_line_opt *cmd_opt;

    cmd = cl_cmd_line_get_instance();
    cmd_opt = cl_cmd_line_opt_create(uart_func_two, NULL, "uart_func", "The UART function.", NULL, NULL, NULL);
    cl_cmd_line_set_cmd_tok(cmd, "uart_func\0\n");
     
    CL_TESTS_ASSERT(cl_cmd_line_opt_process(cmd_opt, cmd));

    cl_cmd_line_opt_destroy_chain(cmd_opt);

    return CL_TESTS_NO_ERR;
}

static cl_tests_err cl_cmd_line_opt_create_creates_different_instances(void) {
    cl_cmd_line_opt *c1, *c2, *c3;
    char *message = "cl_cmd_line_opt_create did not create different instances.";

    c1 = cl_cmd_line_opt_create(NULL, NULL, "c1", "c1", NULL, NULL, NULL);
    c2 = cl_cmd_line_opt_create(NULL, NULL, "c2", "c2", NULL, NULL, NULL);
    c3 = cl_cmd_line_opt_create(NULL, NULL, "c3", "c3", NULL, NULL, NULL);

    CL_TESTS_ASSERT(c1 != c2);
    CL_TESTS_ASSERT(c2 != c3);

    cl_cmd_line_opt_destroy(c1);
    cl_cmd_line_opt_destroy(c2);
    cl_cmd_line_opt_destroy(c3);

    return CL_TESTS_NO_ERR;
}

static cl_tests_err cl_cmd_line_opt_destroy_releases_instance(void) {
    cl_cmd_line_opt *c1, *c2, *c3, *c4;
    char *message = "cl_cmd_line_opt_destroy did not release instance.";

    c1 = cl_cmd_line_opt_create(NULL, NULL, "c1", "c1", NULL, NULL, NULL);
    c2 = cl_cmd_line_opt_create(NULL, NULL, "c2", "c2", NULL, NULL, NULL);
    c3 = cl_cmd_line_opt_create(NULL, NULL, "c3", "c3", NULL, NULL, NULL);

    cl_cmd_line_opt_destroy(c3);

    c4 = cl_cmd_line_opt_create(NULL, NULL, "c4", "c4", NULL, NULL, NULL);

    CL_TESTS_ASSERT(c4 == c3);
    
    cl_cmd_line_opt_destroy(c1);
    cl_cmd_line_opt_destroy(c2);
    cl_cmd_line_opt_destroy(c4);

    return CL_TESTS_NO_ERR;
}

static cl_tests_err cl_cmd_line_opt_destroy_chain_releases_all_instances(void) {
    cl_arg_opt *a1, *a2, *a3, *a4, *a4_2;
    cl_switch_opt *s1, *s2, *s3, *s3_2;
    cl_cmd_line_opt *c1, *c2, *c3, *c3_2;

    a4 = cl_arg_opt_create("a4", "a4", NULL);
    a3 = cl_arg_opt_create("a3", "a3", a4);
    s3 = cl_switch_opt_create("-s3", "-s3", a3, NULL);
    c3 = cl_cmd_line_opt_create(NULL, NULL, "c3", "c3", NULL, s3, NULL);

    a2 = cl_arg_opt_create("a2", "a2", NULL);
    s2 = cl_switch_opt_create("-s2", "-s2", a2, NULL);
    s1 = cl_switch_opt_create("-s1", "-s1", NULL, s2);
    c2 = cl_cmd_line_opt_create(NULL, NULL, "c2", "c2", NULL, s1, c3);

    a1 = cl_arg_opt_create("a1", "a1", NULL);
    c1 = cl_cmd_line_opt_create(NULL, NULL, "c1", "c1", a1, NULL, c2);

    cl_cmd_line_opt_destroy_chain(c1);

    a4_2 = cl_arg_opt_create("a4_2", "a4_2", NULL);
    CL_TESTS_ASSERT(a4_2 == a4);

    s3_2 = cl_switch_opt_create("-s3_2", "-s3_2", NULL, NULL);
    CL_TESTS_ASSERT(s3_2 == s3);

    c3_2 = cl_cmd_line_opt_create(NULL, NULL, "c3_2", "c3_2", NULL, NULL, NULL);
    CL_TESTS_ASSERT(c3_2 == c3);

    cl_arg_opt_destroy(a4_2);
    cl_switch_opt_destroy(s3_2);
    cl_cmd_line_opt_destroy(c3_2);

    return CL_TESTS_NO_ERR;
}

cl_tests_err cl_cmd_line_opt_tests(void) {
    CL_TESTS_RUN(cl_cmd_line_opt_create_creates_structure);
    CL_TESTS_RUN(cl_cmd_line_opt_process_calls_func);
    CL_TESTS_RUN(cl_cmd_line_opt_create_creates_different_instances);
    CL_TESTS_RUN(cl_cmd_line_opt_destroy_releases_instance);
    CL_TESTS_RUN(cl_cmd_line_opt_destroy_chain_releases_all_instances);
    return CL_TESTS_NO_ERR;
}
