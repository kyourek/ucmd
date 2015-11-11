#include "ucmdtests.h"

static ucCmd *cmd;

static const char *transmit_func_one_response = NULL;
static void transmit_func_one(const char *response, void *state) {
    transmit_func_one_response = response;
}

static const char *uart_func_one(ucCmd *p, void *state) {
    return 0;
}

static const char *uart_func_two(ucCmd *p, void *state) {
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

uc_TEST(prior)
cmd = ucCmd_create();
uc_PASS

uc_TEST(after)
ucCmd_destroy(cmd);
uc_PASS

uc_TEST(setup)
    ucTestGroup_setup_test(p, prior, after);
uc_PASS

uc_TEST(ucCmdOpt_create_creates_structure)
    int state;
    ucArgOpt *arg_opt;
    ucSwitchOpt* switch_opt;
    ucCmdOpt *ptr;

    ptr = ucCmdOpt_create(
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

    uc_TRUE(NULL != ptr);
    uc_TRUE(uart_func_one == ucCmdOpt_get_work(ptr));
    uc_TRUE(&state == ucCmdOpt_get_state(ptr));
    uc_TRUE(ucBool_true == ucOpt_is_required((ucOpt*)ptr));

    arg_opt = ucArgOptOwner_get_arg_opt((ucArgOptOwner*)ptr);
    uc_TRUE(ucOpt_get_name((ucOpt*)arg_opt));
    uc_TRUE(ucOpt_get_desc((ucOpt*)arg_opt));

    arg_opt = ucArgOpt_get_next(arg_opt);
    uc_TRUE(ucOpt_get_name((ucOpt*)arg_opt));
    uc_TRUE(ucOpt_get_desc((ucOpt*)arg_opt));

    uc_TRUE(NULL == ucArgOpt_get_next(arg_opt));

    switch_opt = ucCmdOpt_get_switch_opt(ptr);
    uc_TRUE(ucOpt_get_name((ucOpt*)switch_opt));
    uc_TRUE(ucOpt_get_desc((ucOpt*)switch_opt));

    arg_opt = ucArgOptOwner_get_arg_opt((ucArgOptOwner*)switch_opt);
    uc_TRUE(ucOpt_get_name((ucOpt*)arg_opt));
    uc_TRUE(ucOpt_get_desc((ucOpt*)arg_opt));

    /* TODO: Finish making assertions here. */

    ucCmdOpt_destroy_chain(ptr);
uc_PASS

uc_TEST(ucCmdOpt_process_calls_func)
    ucCmdOpt *cmd_opt;
    cmd_opt = ucCmdOpt_create(uart_func_two, NULL, "uart_func", "The UART function.", NULL, NULL, NULL);
    ucCmd_set_command(cmd, "uart_func\0\n");
    uc_TRUE(ucCmdOpt_process(cmd_opt, cmd));
    ucCmdOpt_destroy_chain(cmd_opt);
uc_PASS

uc_TEST(ucCmdOpt_create_creates_different_instances)
    ucCmdOpt *c1, *c2, *c3;

    c1 = ucCmdOpt_create(NULL, NULL, "c1", "c1", NULL, NULL, NULL);
    c2 = ucCmdOpt_create(NULL, NULL, "c2", "c2", NULL, NULL, NULL);
    c3 = ucCmdOpt_create(NULL, NULL, "c3", "c3", NULL, NULL, NULL);

    uc_TRUE(c1 != c2);
    uc_TRUE(c2 != c3);

    ucCmdOpt_destroy(c1);
    ucCmdOpt_destroy(c2);
    ucCmdOpt_destroy(c3);
uc_PASS

uc_TEST(ucCmdOpt_destroy_releases_instance)
    ucCmdOpt *c1, *c2, *c3, *c4;

    c1 = ucCmdOpt_create(NULL, NULL, "c1", "c1", NULL, NULL, NULL);
    c2 = ucCmdOpt_create(NULL, NULL, "c2", "c2", NULL, NULL, NULL);
    c3 = ucCmdOpt_create(NULL, NULL, "c3", "c3", NULL, NULL, NULL);

    ucCmdOpt_destroy(c3);

    c4 = ucCmdOpt_create(NULL, NULL, "c4", "c4", NULL, NULL, NULL);

    uc_TRUE(c4 == c3);
    
    ucCmdOpt_destroy(c1);
    ucCmdOpt_destroy(c2);
    ucCmdOpt_destroy(c4);
uc_PASS

uc_TEST(ucCmdOpt_destroy_chain_releases_all_instances)
    ucArgOpt *a1, *a2, *a3, *a4, *a4_2;
    ucSwitchOpt *s1, *s2, *s3, *s3_2;
    ucCmdOpt *c1, *c2, *c3, *c3_2;

    a4 = ucArgOpt_create("a4", "a4", NULL);
    a3 = ucArgOpt_create("a3", "a3", a4);
    s3 = ucSwitchOpt_create("-s3", "-s3", a3, NULL);
    c3 = ucCmdOpt_create(NULL, NULL, "c3", "c3", NULL, s3, NULL);

    a2 = ucArgOpt_create("a2", "a2", NULL);
    s2 = ucSwitchOpt_create("-s2", "-s2", a2, NULL);
    s1 = ucSwitchOpt_create("-s1", "-s1", NULL, s2);
    c2 = ucCmdOpt_create(NULL, NULL, "c2", "c2", NULL, s1, c3);

    a1 = ucArgOpt_create("a1", "a1", NULL);
    c1 = ucCmdOpt_create(NULL, NULL, "c1", "c1", a1, NULL, c2);

    ucCmdOpt_destroy_chain(c1);

    a4_2 = ucArgOpt_create("a4_2", "a4_2", NULL);
    uc_TRUE(a4_2 == a4);

    s3_2 = ucSwitchOpt_create("-s3_2", "-s3_2", NULL, NULL);
    uc_TRUE(s3_2 == s3);

    c3_2 = ucCmdOpt_create(NULL, NULL, "c3_2", "c3_2", NULL, NULL, NULL);
    uc_TRUE(c3_2 == c3);

    ucArgOpt_destroy(a4_2);
    ucSwitchOpt_destroy(s3_2);
    ucCmdOpt_destroy(c3_2);
uc_PASS

uc_TEST(ucCmdOpt_send_usage_responds_with_usage_string)
    const char *expected;

    ucCmdOpt *cmd_opt = 
        ucCmdOpt_create(NULL, NULL, "dothis", NULL,
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
        ucCmdOpt_create(NULL, NULL, "boringcmd", NULL, NULL, NULL, NULL));

    ucCmd_set_transmit(cmd, transmit_func_one);
    
    ucCmdOpt_send_usage(cmd_opt, cmd);
    expected = "dothis [firstarg] secondarg [<numeric>] [-s1] <numeric> [-s2] -sthree [s3arg]";
    uc_TRUE(0 == strcmp(expected, transmit_func_one_response));

    ucCmdOpt_send_usage(ucCmdOpt_get_next(cmd_opt), cmd);
    expected = "boringcmd";
    uc_TRUE(0 == strcmp(expected, transmit_func_one_response));

    ucCmdOpt_destroy_chain(cmd_opt);
uc_PASS

uc_TEST(ucCmdOpt_send_usage_uses_boolean_argument_name)
    const char *expected;

    ucCmdOpt *cmd_opt =
        ucCmdOpt_create(NULL, NULL, "some-action", NULL,
        ucArgOpt_create_boolean("bool val", NULL),
        ucSwitchOpt_create_required("-sw", "the-sw",
        ucArgOpt_create_required_boolean("req. bool val",
        NULL), NULL), NULL);

    ucCmd_set_transmit(cmd, transmit_func_one);

    ucCmdOpt_send_usage(cmd_opt, cmd);
    expected = "some-action [<boolean>] -sw <boolean>";
    uc_TRUE(0 == strcmp(expected, transmit_func_one_response));

    ucCmdOpt_destroy_chain(cmd_opt);
uc_PASS

uc_TEST(ucCmdOpt_format_validation_err_catches_required_arg)
    const char *err;
    ucCmdOpt *opt = ucCmdOpt_create(NULL, NULL, "opt", NULL, ucArgOpt_create_required("a", NULL, NULL), NULL, NULL);

    ucCmd_set_command(cmd, "opt\0\n");
    err = ucCmdOpt_format_validation_err(opt, cmd);
    uc_TRUE(NULL != err);

    ucCmd_set_command(cmd, "opt\0arg\0\n");
    err = ucCmdOpt_format_validation_err(opt, cmd);
    uc_TRUE(NULL == err);

    ucCmdOpt_destroy_chain(opt);
uc_PASS

uc_TEST(ucCmdOpt_format_validation_err_catches_required_switch)
    const char *err;
    ucCmdOpt *opt = ucCmdOpt_create(NULL, NULL, "opt", NULL, NULL, ucSwitchOpt_create_required("-s", NULL, NULL, NULL), NULL);

    ucCmd_set_command(cmd, "opt\0-z\0\n");
    err = ucCmdOpt_format_validation_err(opt, cmd);
    uc_TRUE(NULL != err);

    ucCmd_set_command(cmd, "opt\0-s\0\n");
    err = ucCmdOpt_format_validation_err(opt, cmd);
    uc_TRUE(NULL == err);

    ucCmdOpt_destroy_chain(opt);
uc_PASS

uc_TEST(ucCmdOpt_process_handles_invalid_commands)
    int state;
    const char *err;
    ucCmdOpt *opt = ucCmdOpt_create(NULL, NULL, "opt", NULL, NULL, NULL, NULL);

    ucCmd_set_handle_invalid_command(cmd, handle_invalid_command_1);
    ucCmd_set_handle_invalid_command_state(cmd, &state);
    ucCmd_set_command(cmd, "noopt\0\n");

    handle_invalid_command_1_return = ucBool_true;
    err = ucCmdOpt_process(opt, cmd);
    uc_TRUE(NULL == err);
    uc_TRUE(handle_invalid_command_1_state == &state);
    uc_TRUE(0 == strcmp(handle_invalid_command_1_invalid_command, "noopt"));

    ucCmdOpt_destroy_chain(opt);
uc_PASS

uc_TEST(ucCmdOpt_process_does_not_handle_invalid_command)
    const char *err;
    ucCmdOpt *opt = ucCmdOpt_create(NULL, NULL, "opt", NULL, NULL, NULL, NULL);

    ucCmd_set_handle_invalid_command(cmd, NULL);
    ucCmd_set_command(cmd, "noopt\0\n");

    err = ucCmdOpt_process(opt, cmd);
    uc_TRUE(0 == strcmp(err, ucOpt_INVALID "Command 'noopt' does not exist."));

    ucCmdOpt_destroy_chain(opt);
uc_PASS

uc_TEST(ucCmdOpt_process_responds_correctly_if_no_work_exists)
    const char *response;
    ucCmdOpt *opt = ucCmdOpt_create(NULL, NULL, "opt", NULL, NULL, NULL, NULL);
    ucCmd_set_command(cmd, "opt\0\n");
    response = ucCmdOpt_process(opt, cmd);
    uc_TRUE(0 == strcmp(response, "No work found for command \"opt\""));
uc_PASS

uc_TEST(ucCmdOpt_format_validation_err_creates_correct_message, ucCmdOpt *cmd_opt, const char *command, const char *correct_message)
    const char *validation_err;
    ucCmd_parse_const(cmd, command);
    validation_err = ucCmdOpt_format_validation_err(cmd_opt, cmd);
    ucCmdOpt_destroy(cmd_opt);
    uc_TRUE(uc_STR_EQ(validation_err, correct_message));
uc_PASS
uc_CASE(ucCmdOpt_format_validation_err_creates_correct_message, if_args_are_not_required, ucCmdOpt_create(NULL, NULL, "cmd", NULL, NULL, NULL, NULL), "cmd arg", ucOpt_INVALID "Command 'cmd' requires no arguments.")
uc_CASE(ucCmdOpt_format_validation_err_creates_correct_message, if_arg_is_required, ucCmdOpt_create(NULL, NULL, "cmd", NULL, ucArgOpt_create("unreq-arg", NULL, ucArgOpt_create_required("req-arg", NULL, NULL)), NULL, NULL), "cmd a1", ucOpt_INVALID "Argument 'req-arg' is required.")
uc_CASE(ucCmdOpt_format_validation_err_creates_correct_message, if_too_many_args_given, ucCmdOpt_create(NULL, NULL, "cmd", NULL, ucArgOpt_create("arg1", NULL, NULL), NULL, NULL), "cmd a1 a2", ucOpt_INVALID "Command 'cmd' has no option for argument 'a2'.")
uc_CASE(ucCmdOpt_format_validation_err_creates_correct_message, if_switches_are_not_allowed, ucCmdOpt_create(NULL, NULL, "c", NULL, ucArgOpt_create("a", NULL, NULL), NULL, NULL), "c arg -switch", ucOpt_INVALID "Command 'c' requires no switches.")
uc_CASE(ucCmdOpt_format_validation_err_creates_correct_message, if_too_many_switches_passed, ucCmdOpt_create(NULL, NULL, "c", NULL, NULL, ucSwitchOpt_create("-s1", NULL, NULL, NULL), NULL), "c -s1 -s2", ucOpt_INVALID "Command 'c' has no option for switch '-s2'.")
uc_CASE(ucCmdOpt_format_validation_err_creates_correct_message, if_switch_is_required, ucCmdOpt_create(NULL, NULL, "c", NULL, NULL, ucSwitchOpt_create_required("-s1", NULL, NULL, NULL), NULL), "c", ucOpt_INVALID "Command 'c' requires switch '-s1'.")

uc_TEST_GROUP(ucCmdOpt, setup,
    ucCmdOpt_create_creates_structure,
    ucCmdOpt_process_calls_func,
    ucCmdOpt_create_creates_different_instances,
    ucCmdOpt_destroy_releases_instance,
    ucCmdOpt_destroy_chain_releases_all_instances,
    ucCmdOpt_send_usage_responds_with_usage_string,
    ucCmdOpt_format_validation_err_catches_required_arg,
    ucCmdOpt_format_validation_err_catches_required_switch,
    ucCmdOpt_format_validation_err_creates_correct_message_if_arg_is_required,
    ucCmdOpt_format_validation_err_creates_correct_message_if_args_are_not_required,
    ucCmdOpt_format_validation_err_creates_correct_message_if_switches_are_not_allowed,
    ucCmdOpt_format_validation_err_creates_correct_message_if_switch_is_required,
    ucCmdOpt_format_validation_err_creates_correct_message_if_too_many_args_given,
    ucCmdOpt_format_validation_err_creates_correct_message_if_too_many_switches_passed,
    ucCmdOpt_process_handles_invalid_commands,
    ucCmdOpt_process_does_not_handle_invalid_command,
    ucCmdOpt_process_responds_correctly_if_no_work_exists,
    ucCmdOpt_send_usage_uses_boolean_argument_name)
