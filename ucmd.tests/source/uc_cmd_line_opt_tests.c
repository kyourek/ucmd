#include <string.h>
#include "uc_cmd_line_opt_p.h"
#include "uc_cmd_line_opt_tests.h"
#include "uc_test.h"

static const char *transmit_func_one_response = NULL;
static void transmit_func_one(const char *response, void *state) {
    transmit_func_one_response = response;
}

static const char *uart_func_one(uc_cmd_line *p, void *state) {
    return UC_TEST_ERR_NONE;
}

static const char *uart_func_two(uc_cmd_line *p, void *state) {
    return "The result of uart_func_two.";
}

static uc_test_err uc_cmd_line_opt_create_creates_structure(uc_test_group *p) {
    int state;
    uc_arg_opt *arg_opt;
    uc_switch_opt* switch_opt;
    uc_cmd_line_opt *ptr;

    ptr = uc_cmd_line_opt_create(
        uart_func_one, &state, "uart_func_one", "The first function.",
            uc_arg_opt_create("arg1", "The first arg.",
            uc_arg_opt_create("arg2", "The second arg.", NULL)),
            uc_switch_opt_create("sw1", "The first switch.",
                uc_arg_opt_create("sw1arg1", "The first arg of sw1.",
                uc_arg_opt_create("sw1arg2", "The second arg of sw1.", NULL)),
            uc_switch_opt_create("sw2", "The second switch.",
                uc_arg_opt_create("sw2arg1", "The first arg of sw2.",
                uc_arg_opt_create("sw2arg2", "The second arg of sw2.", NULL)),
            NULL)),
        NULL
    );

    UC_TEST_ASSERT(NULL != ptr);
    UC_TEST_ASSERT(uart_func_one == uc_cmd_line_opt_get_func(ptr));
    UC_TEST_ASSERT(&state == uc_cmd_line_opt_get_state(ptr));
    UC_TEST_ASSERT(UC_TRUE == uc_opt_is_required((uc_opt*)ptr));

    arg_opt = uc_arg_opt_owner_get_arg_opt((uc_arg_opt_owner*)ptr);
    UC_TEST_ASSERT(uc_opt_get_name((uc_opt*)arg_opt));
    UC_TEST_ASSERT(uc_opt_get_desc((uc_opt*)arg_opt));

    arg_opt = uc_arg_opt_get_next(arg_opt);
    UC_TEST_ASSERT(uc_opt_get_name((uc_opt*)arg_opt));
    UC_TEST_ASSERT(uc_opt_get_desc((uc_opt*)arg_opt));

    UC_TEST_ASSERT(NULL == uc_arg_opt_get_next(arg_opt));

    switch_opt = uc_cmd_line_opt_get_switch_opt(ptr);
    UC_TEST_ASSERT(uc_opt_get_name((uc_opt*)switch_opt));
    UC_TEST_ASSERT(uc_opt_get_desc((uc_opt*)switch_opt));

    arg_opt = uc_arg_opt_owner_get_arg_opt((uc_arg_opt_owner*)switch_opt);
    UC_TEST_ASSERT(uc_opt_get_name((uc_opt*)arg_opt));
    UC_TEST_ASSERT(uc_opt_get_desc((uc_opt*)arg_opt));

    // TODO: Finish making assertions here.

    uc_cmd_line_opt_destroy_chain(ptr);

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_opt_process_calls_func(uc_test_group *p) {
    uc_cmd_line *cmd;
    uc_cmd_line_opt *cmd_opt;

    cmd = uc_cmd_line_get_instance();
    cmd_opt = uc_cmd_line_opt_create(uart_func_two, NULL, "uart_func", "The UART function.", NULL, NULL, NULL);
    uc_cmd_line_set_cmd_tok(cmd, "uart_func\0\n");
     
    UC_TEST_ASSERT(uc_cmd_line_opt_process(cmd_opt, cmd));

    uc_cmd_line_opt_destroy_chain(cmd_opt);

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_opt_create_creates_different_instances(uc_test_group *p) {
    uc_cmd_line_opt *c1, *c2, *c3;

    c1 = uc_cmd_line_opt_create(NULL, NULL, "c1", "c1", NULL, NULL, NULL);
    c2 = uc_cmd_line_opt_create(NULL, NULL, "c2", "c2", NULL, NULL, NULL);
    c3 = uc_cmd_line_opt_create(NULL, NULL, "c3", "c3", NULL, NULL, NULL);

    UC_TEST_ASSERT(c1 != c2);
    UC_TEST_ASSERT(c2 != c3);

    uc_cmd_line_opt_destroy(c1);
    uc_cmd_line_opt_destroy(c2);
    uc_cmd_line_opt_destroy(c3);

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_opt_destroy_releases_instance(uc_test_group *p) {
    uc_cmd_line_opt *c1, *c2, *c3, *c4;

    c1 = uc_cmd_line_opt_create(NULL, NULL, "c1", "c1", NULL, NULL, NULL);
    c2 = uc_cmd_line_opt_create(NULL, NULL, "c2", "c2", NULL, NULL, NULL);
    c3 = uc_cmd_line_opt_create(NULL, NULL, "c3", "c3", NULL, NULL, NULL);

    uc_cmd_line_opt_destroy(c3);

    c4 = uc_cmd_line_opt_create(NULL, NULL, "c4", "c4", NULL, NULL, NULL);

    UC_TEST_ASSERT(c4 == c3);
    
    uc_cmd_line_opt_destroy(c1);
    uc_cmd_line_opt_destroy(c2);
    uc_cmd_line_opt_destroy(c4);

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_opt_destroy_chain_releases_all_instances(uc_test_group *p) {
    uc_arg_opt *a1, *a2, *a3, *a4, *a4_2;
    uc_switch_opt *s1, *s2, *s3, *s3_2;
    uc_cmd_line_opt *c1, *c2, *c3, *c3_2;

    a4 = uc_arg_opt_create("a4", "a4", NULL);
    a3 = uc_arg_opt_create("a3", "a3", a4);
    s3 = uc_switch_opt_create("-s3", "-s3", a3, NULL);
    c3 = uc_cmd_line_opt_create(NULL, NULL, "c3", "c3", NULL, s3, NULL);

    a2 = uc_arg_opt_create("a2", "a2", NULL);
    s2 = uc_switch_opt_create("-s2", "-s2", a2, NULL);
    s1 = uc_switch_opt_create("-s1", "-s1", NULL, s2);
    c2 = uc_cmd_line_opt_create(NULL, NULL, "c2", "c2", NULL, s1, c3);

    a1 = uc_arg_opt_create("a1", "a1", NULL);
    c1 = uc_cmd_line_opt_create(NULL, NULL, "c1", "c1", a1, NULL, c2);

    uc_cmd_line_opt_destroy_chain(c1);

    a4_2 = uc_arg_opt_create("a4_2", "a4_2", NULL);
    UC_TEST_ASSERT(a4_2 == a4);

    s3_2 = uc_switch_opt_create("-s3_2", "-s3_2", NULL, NULL);
    UC_TEST_ASSERT(s3_2 == s3);

    c3_2 = uc_cmd_line_opt_create(NULL, NULL, "c3_2", "c3_2", NULL, NULL, NULL);
    UC_TEST_ASSERT(c3_2 == c3);

    uc_arg_opt_destroy(a4_2);
    uc_switch_opt_destroy(s3_2);
    uc_cmd_line_opt_destroy(c3_2);

    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_opt_send_usage_responds_with_usage_string(uc_test_group *p) {
    const char *expected;
    uc_cmd_line *cmd = uc_cmd_line_get_instance();
    uc_cmd_line_transmit_func *prev_transmit_func = uc_cmd_line_get_transmit(cmd);

    uc_cmd_line_opt *cmd_opt = 
        uc_cmd_line_opt_create(NULL, NULL, "dothis", NULL,
            uc_arg_opt_create("firstarg", NULL,
            uc_arg_opt_create_required("secondarg", NULL,
            uc_arg_opt_create_numeric(NULL, 0, 5,
            NULL))),
            uc_switch_opt_create("-s1", NULL, 
                uc_arg_opt_create_required_numeric(NULL, 0, 10,
                NULL),
            uc_switch_opt_create("-s2", NULL,
                NULL,
            uc_switch_opt_create_required("-sthree", NULL,
                uc_arg_opt_create("s3arg", NULL,
                NULL),
            NULL))),
        uc_cmd_line_opt_create(NULL, NULL, "boringcmd", NULL, NULL, NULL, NULL));

    uc_cmd_line_set_transmit(cmd, transmit_func_one);
    
    uc_cmd_line_opt_send_usage(cmd_opt, cmd);
    expected = "dothis [firstarg] secondarg [<number>] [-s1] <number> [-s2] -sthree [s3arg]";
    UC_TEST_ASSERT(0 == strcmp(expected, transmit_func_one_response));

    uc_cmd_line_opt_send_usage(uc_cmd_line_opt_get_next(cmd_opt), cmd);
    expected = "boringcmd";
    UC_TEST_ASSERT(0 == strcmp(expected, transmit_func_one_response));

    uc_cmd_line_opt_destroy_chain(cmd_opt);
    uc_cmd_line_set_transmit(cmd, prev_transmit_func);
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_opt_format_validation_err_catches_required_arg(uc_test_group *p) {
    const char *err;
    uc_cmd_line *cmd = uc_cmd_line_get_instance();
    uc_cmd_line_opt *opt = uc_cmd_line_opt_create(NULL, NULL, "opt", NULL, uc_arg_opt_create_required("a", NULL, NULL), NULL, NULL);

    uc_cmd_line_set_cmd_tok(cmd, "opt\0\n");
    err = uc_cmd_line_opt_format_validation_err(opt, cmd);
    UC_TEST_ASSERT(NULL != err);

    uc_cmd_line_set_cmd_tok(cmd, "opt\0arg\0\n");
    err = uc_cmd_line_opt_format_validation_err(opt, cmd);
    UC_TEST_ASSERT(NULL == err);

    uc_cmd_line_opt_destroy_chain(opt);
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_opt_format_validation_err_catches_required_switch(uc_test_group *p) {
    const char *err;
    uc_cmd_line *cmd = uc_cmd_line_get_instance();
    uc_cmd_line_opt *opt = uc_cmd_line_opt_create(NULL, NULL, "opt", NULL, NULL, uc_switch_opt_create_required("-s", NULL, NULL, NULL), NULL);

    uc_cmd_line_set_cmd_tok(cmd, "opt\0-z\0\n");
    err = uc_cmd_line_opt_format_validation_err(opt, cmd);
    UC_TEST_ASSERT(NULL != err);

    uc_cmd_line_set_cmd_tok(cmd, "opt\0-s\0\n");
    err = uc_cmd_line_opt_format_validation_err(opt, cmd);
    UC_TEST_ASSERT(NULL == err);

    uc_cmd_line_opt_destroy_chain(opt);
    return UC_TEST_ERR_NONE;
}

uc_test_group *uc_cmd_line_opt_tests_get_group(void) {
    static uc_test_group group;
    static uc_test_group_test_func *tests[] = {
        uc_cmd_line_opt_create_creates_structure,
        uc_cmd_line_opt_process_calls_func,
        uc_cmd_line_opt_create_creates_different_instances,
        uc_cmd_line_opt_destroy_releases_instance,
        uc_cmd_line_opt_destroy_chain_releases_all_instances,
        uc_cmd_line_opt_send_usage_responds_with_usage_string,
        uc_cmd_line_opt_format_validation_err_catches_required_arg,
        uc_cmd_line_opt_format_validation_err_catches_required_switch,
        NULL
    };

    return uc_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
