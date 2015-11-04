#include <string.h>
#include "ucmdtests.h"

static const char *transmit_func_one_response = NULL;
static void transmit_func_one(const char *response, void *state) {
    transmit_func_one_response = response;
}

static const char *uart_func_one(ucCmdLine *p, void *state) {
    ucPASS();
}

static const char *uart_func_two(ucCmdLine *p, void *state) {
    return "The result of uart_func_two.";
}

static void *handle_invalid_command_1_state;
static const char *handle_invalid_command_1_invalid_command;
static ucBool handle_invalid_command_1_return = ucBool_false;
static ucBool handle_invalid_command_1(const char *invalid_command, void *state) {
    handle_invalid_command_1_state = state;
    handle_invalid_command_1_invalid_command = invalid_command;
    return handle_invalid_command_1_return;
}

static ucTestErr ucCmdLineOpt_create_creates_structure(ucTestGroup *p) {
    int state;
    ucArgOpt *arg_opt;
    ucSwitchOpt* switch_opt;
    ucCmdLineOpt *ptr;

    ptr = ucCmdLineOpt_create(
        uart_func_one, &state, "uart_func_one", "The first function.",
            ucArgOpt_create("arg1", "The first arg.",
            ucArgOpt_create("arg2", "The second arg.", NULL)),
            ucSwitchOpt_create("sw1", "The first switch.",
                ucArgOpt_create("sw1arg1", "The first arg of sw1.",
                ucArgOpt_create("sw1arg2", "The second arg of sw1.", NULL)),
            ucSwitchOpt_create("sw2", "The second switch.",
                ucArgOpt_create("sw2arg1", "The first arg of sw2.",
                ucArgOpt_create("sw2arg2", "The second arg of sw2.", NULL)),
            NULL)),
        NULL
    );

    ucTRUE(NULL != ptr);
    ucTRUE(uart_func_one == ucCmdLineOpt_get_work(ptr));
    ucTRUE(&state == ucCmdLineOpt_get_state(ptr));
    ucTRUE(ucBool_true == ucOpt_is_required((ucOpt*)ptr));

    arg_opt = ucArgOptOwner_get_arg_opt((ucArgOptOwner*)ptr);
    ucTRUE(ucOpt_get_name((ucOpt*)arg_opt));
    ucTRUE(ucOpt_get_desc((ucOpt*)arg_opt));

    arg_opt = ucArgOpt_get_next(arg_opt);
    ucTRUE(ucOpt_get_name((ucOpt*)arg_opt));
    ucTRUE(ucOpt_get_desc((ucOpt*)arg_opt));

    ucTRUE(NULL == ucArgOpt_get_next(arg_opt));

    switch_opt = ucCmdLineOpt_get_switch_opt(ptr);
    ucTRUE(ucOpt_get_name((ucOpt*)switch_opt));
    ucTRUE(ucOpt_get_desc((ucOpt*)switch_opt));

    arg_opt = ucArgOptOwner_get_arg_opt((ucArgOptOwner*)switch_opt);
    ucTRUE(ucOpt_get_name((ucOpt*)arg_opt));
    ucTRUE(ucOpt_get_desc((ucOpt*)arg_opt));

    // TODO: Finish making assertions here.

    ucCmdLineOpt_destroy_chain(ptr);

    ucPASS();
}

static ucTestErr ucCmdLineOpt_process_calls_func(ucTestGroup *p) {
    ucCmdLine *cmd;
    ucCmdLineOpt *cmd_opt;

    cmd = ucCmdLine_instance();
    cmd_opt = ucCmdLineOpt_create(uart_func_two, NULL, "uart_func", "The UART function.", NULL, NULL, NULL);
    ucCmdLine_set_cmd_tok(cmd, "uart_func\0\n");
     
    ucTRUE(ucCmdLineOpt_process(cmd_opt, cmd));

    ucCmdLineOpt_destroy_chain(cmd_opt);

    ucPASS();
}

static ucTestErr ucCmdLineOpt_create_creates_different_instances(ucTestGroup *p) {
    ucCmdLineOpt *c1, *c2, *c3;

    c1 = ucCmdLineOpt_create(NULL, NULL, "c1", "c1", NULL, NULL, NULL);
    c2 = ucCmdLineOpt_create(NULL, NULL, "c2", "c2", NULL, NULL, NULL);
    c3 = ucCmdLineOpt_create(NULL, NULL, "c3", "c3", NULL, NULL, NULL);

    ucTRUE(c1 != c2);
    ucTRUE(c2 != c3);

    ucCmdLineOpt_destroy(c1);
    ucCmdLineOpt_destroy(c2);
    ucCmdLineOpt_destroy(c3);

    ucPASS();
}

static ucTestErr ucCmdLineOpt_destroy_releases_instance(ucTestGroup *p) {
    ucCmdLineOpt *c1, *c2, *c3, *c4;

    c1 = ucCmdLineOpt_create(NULL, NULL, "c1", "c1", NULL, NULL, NULL);
    c2 = ucCmdLineOpt_create(NULL, NULL, "c2", "c2", NULL, NULL, NULL);
    c3 = ucCmdLineOpt_create(NULL, NULL, "c3", "c3", NULL, NULL, NULL);

    ucCmdLineOpt_destroy(c3);

    c4 = ucCmdLineOpt_create(NULL, NULL, "c4", "c4", NULL, NULL, NULL);

    ucTRUE(c4 == c3);
    
    ucCmdLineOpt_destroy(c1);
    ucCmdLineOpt_destroy(c2);
    ucCmdLineOpt_destroy(c4);

    ucPASS();
}

static ucTestErr ucCmdLineOpt_destroy_chain_releases_all_instances(ucTestGroup *p) {
    ucArgOpt *a1, *a2, *a3, *a4, *a4_2;
    ucSwitchOpt *s1, *s2, *s3, *s3_2;
    ucCmdLineOpt *c1, *c2, *c3, *c3_2;

    a4 = ucArgOpt_create("a4", "a4", NULL);
    a3 = ucArgOpt_create("a3", "a3", a4);
    s3 = ucSwitchOpt_create("-s3", "-s3", a3, NULL);
    c3 = ucCmdLineOpt_create(NULL, NULL, "c3", "c3", NULL, s3, NULL);

    a2 = ucArgOpt_create("a2", "a2", NULL);
    s2 = ucSwitchOpt_create("-s2", "-s2", a2, NULL);
    s1 = ucSwitchOpt_create("-s1", "-s1", NULL, s2);
    c2 = ucCmdLineOpt_create(NULL, NULL, "c2", "c2", NULL, s1, c3);

    a1 = ucArgOpt_create("a1", "a1", NULL);
    c1 = ucCmdLineOpt_create(NULL, NULL, "c1", "c1", a1, NULL, c2);

    ucCmdLineOpt_destroy_chain(c1);

    a4_2 = ucArgOpt_create("a4_2", "a4_2", NULL);
    ucTRUE(a4_2 == a4);

    s3_2 = ucSwitchOpt_create("-s3_2", "-s3_2", NULL, NULL);
    ucTRUE(s3_2 == s3);

    c3_2 = ucCmdLineOpt_create(NULL, NULL, "c3_2", "c3_2", NULL, NULL, NULL);
    ucTRUE(c3_2 == c3);

    ucArgOpt_destroy(a4_2);
    ucSwitchOpt_destroy(s3_2);
    ucCmdLineOpt_destroy(c3_2);

    ucPASS();
}

static ucTestErr ucCmdLineOpt_send_usage_responds_with_usage_string(ucTestGroup *p) {
    const char *expected;
    ucCmdLine *cmd = ucCmdLine_instance();
    ucCmdLine_TransmitFunc *prev_transmit_func = ucCmdLine_get_transmit(cmd);

    ucCmdLineOpt *cmd_opt = 
        ucCmdLineOpt_create(NULL, NULL, "dothis", NULL,
            ucArgOpt_create("firstarg", NULL,
            ucArgOpt_create_required("secondarg", NULL,
            ucArgOpt_create_numeric(NULL, 0, 5,
            NULL))),
            ucSwitchOpt_create("-s1", NULL, 
                ucArgOpt_create_required_numeric(NULL, 0, 10,
                NULL),
            ucSwitchOpt_create("-s2", NULL,
                NULL,
            ucSwitchOpt_create_required("-sthree", NULL,
                ucArgOpt_create("s3arg", NULL,
                NULL),
            NULL))),
        ucCmdLineOpt_create(NULL, NULL, "boringcmd", NULL, NULL, NULL, NULL));

    ucCmdLine_set_transmit(cmd, transmit_func_one);
    
    ucCmdLineOpt_send_usage(cmd_opt, cmd);
    expected = "dothis [firstarg] secondarg [<numeric>] [-s1] <numeric> [-s2] -sthree [s3arg]";
    ucTRUE(0 == strcmp(expected, transmit_func_one_response));

    ucCmdLineOpt_send_usage(ucCmdLineOpt_get_next(cmd_opt), cmd);
    expected = "boringcmd";
    ucTRUE(0 == strcmp(expected, transmit_func_one_response));

    ucCmdLineOpt_destroy_chain(cmd_opt);
    ucCmdLine_set_transmit(cmd, prev_transmit_func);
    ucPASS();
}

static ucTestErr ucCmdLineOpt_send_usage__uses_boolean_argument_name(ucTestGroup *p) {
    const char *expected;
    ucCmdLine *cmd = ucCmdLine_instance();
    ucCmdLine_TransmitFunc *prev_transmit_func = ucCmdLine_get_transmit(cmd);

    ucCmdLineOpt *cmd_opt =
        ucCmdLineOpt_create(NULL, NULL, "some-action", NULL,
        ucArgOpt_create_boolean("bool val", NULL),
        ucSwitchOpt_create_required("-sw", "the-sw",
        ucArgOpt_create_required_boolean("req. bool val",
        NULL), NULL), NULL);

    ucCmdLine_set_transmit(cmd, transmit_func_one);

    ucCmdLineOpt_send_usage(cmd_opt, cmd);
    expected = "some-action [<boolean>] -sw <boolean>";
    ucTRUE(0 == strcmp(expected, transmit_func_one_response));

    ucCmdLineOpt_destroy_chain(cmd_opt);
    ucCmdLine_set_transmit(cmd, prev_transmit_func);
    ucPASS();
}

static ucTestErr ucCmdLineOpt_format_validation_err_catches_required_arg(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_instance();
    ucCmdLineOpt *opt = ucCmdLineOpt_create(NULL, NULL, "opt", NULL, ucArgOpt_create_required("a", NULL, NULL), NULL, NULL);

    ucCmdLine_set_cmd_tok(cmd, "opt\0\n");
    err = ucCmdLineOpt_format_validation_err(opt, cmd);
    ucTRUE(NULL != err);

    ucCmdLine_set_cmd_tok(cmd, "opt\0arg\0\n");
    err = ucCmdLineOpt_format_validation_err(opt, cmd);
    ucTRUE(NULL == err);

    ucCmdLineOpt_destroy_chain(opt);
    ucPASS();
}

static ucTestErr ucCmdLineOpt_format_validation_err_catches_required_switch(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_instance();
    ucCmdLineOpt *opt = ucCmdLineOpt_create(NULL, NULL, "opt", NULL, NULL, ucSwitchOpt_create_required("-s", NULL, NULL, NULL), NULL);

    ucCmdLine_set_cmd_tok(cmd, "opt\0-z\0\n");
    err = ucCmdLineOpt_format_validation_err(opt, cmd);
    ucTRUE(NULL != err);

    ucCmdLine_set_cmd_tok(cmd, "opt\0-s\0\n");
    err = ucCmdLineOpt_format_validation_err(opt, cmd);
    ucTRUE(NULL == err);

    ucCmdLineOpt_destroy_chain(opt);
    ucPASS();
}

static ucTestErr ucCmdLineOpt_process_handles_invalid_commands(ucTestGroup *p) {
    int state;
    const char *err;
    ucCmdLine *cmd = ucCmdLine_instance();
    ucCmdLineOpt *opt = ucCmdLineOpt_create(NULL, NULL, "opt", NULL, NULL, NULL, NULL);

    ucCmdLine_set_handle_invalid_command(cmd, handle_invalid_command_1);
    ucCmdLine_set_handle_invalid_command_state(cmd, &state);
    ucCmdLine_set_cmd_tok(cmd, "noopt\0\n");

    handle_invalid_command_1_return = ucBool_true;
    err = ucCmdLineOpt_process(opt, cmd);
    ucTRUE(NULL == err);
    ucTRUE(handle_invalid_command_1_state == &state);
    ucTRUE(0 == strcmp(handle_invalid_command_1_invalid_command, "noopt"));

    ucCmdLineOpt_destroy_chain(opt);
    ucPASS();
}

static ucTestErr ucCmdLineOpt_process_does_not_handle_invalid_command(ucTestGroup *p) {
    const char *err;
    ucCmdLine *cmd = ucCmdLine_instance();
    ucCmdLineOpt *opt = ucCmdLineOpt_create(NULL, NULL, "opt", NULL, NULL, NULL, NULL);

    ucCmdLine_set_handle_invalid_command(cmd, NULL);
    ucCmdLine_set_cmd_tok(cmd, "noopt\0\n");

    err = ucCmdLineOpt_process(opt, cmd);
    ucTRUE(0 == strcmp(err, "Invalid: No command option found for \"noopt\""));

    ucCmdLineOpt_destroy_chain(opt);
    ucPASS();
}

static ucTestErr ucCmdLineOpt_process_responds_correctly_if_no_work_exists(ucTestGroup *p) {
    const char *response;
    ucCmdLine *cmd = ucCmdLine_instance();
    ucCmdLineOpt *opt = ucCmdLineOpt_create(NULL, NULL, "opt", NULL, NULL, NULL, NULL);
    ucCmdLine_set_cmd_tok(cmd, "opt\0\n");
    response = ucCmdLineOpt_process(opt, cmd);
    ucTRUE(0 == strcmp(response, "No work found for command \"opt\""));
    ucPASS();
}

ucTestGroup *ucCmdLineOpt_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_TestFunc *tests[] = {
        ucCmdLineOpt_create_creates_structure,
        ucCmdLineOpt_process_calls_func,
        ucCmdLineOpt_create_creates_different_instances,
        ucCmdLineOpt_destroy_releases_instance,
        ucCmdLineOpt_destroy_chain_releases_all_instances,
        ucCmdLineOpt_send_usage_responds_with_usage_string,
        ucCmdLineOpt_format_validation_err_catches_required_arg,
        ucCmdLineOpt_format_validation_err_catches_required_switch,
        ucCmdLineOpt_process_handles_invalid_commands,
        ucCmdLineOpt_process_does_not_handle_invalid_command,
        ucCmdLineOpt_process_responds_correctly_if_no_work_exists,
        ucCmdLineOpt_send_usage__uses_boolean_argument_name,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
