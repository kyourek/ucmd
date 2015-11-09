#include "ucmdtests.h"

static ucCmd *cmd_line;
static ucCmdParser *cmd_parser;
static ucCmdApp *subject;

void *receive_1_state;
static char *receive_1(char *buf, size_t buf_size, void *state) { 
    receive_1_state = state;
    return 0; 
}

static char *receive_2(char *buf, size_t buf_size, void *state) { return 0; }

uc_TEST(prior)
    subject = ucCmdApp_create();
    cmd_line = ucCmdApp_get_cmd(subject);
    cmd_parser = ucCmdApp_get_cmd_parser(subject);
uc_PASS

uc_TEST(after)
    ucCmdApp_destroy(subject);
uc_PASS

uc_TEST(setup)
    ucTestGroup_setup_test(p, prior, after);
uc_PASS

uc_TEST(ucCmdApp_set_receive_sets_receive)
    ucCmdApp_set_receive(subject, receive_1);
    uc_TRUE(receive_1 == ucCmdApp_get_receive(subject));
uc_PASS

uc_TEST(ucCmdApp_get_receive_returns_receive)
    subject->receive = receive_2;
    uc_TRUE(receive_2 == ucCmdApp_get_receive(subject));
uc_PASS

uc_TEST(ucCmdApp_set_help_command_sets_value)
    ucCmdApp_set_help_command(subject, "h");
    uc_TRUE(0 == strcmp("h", ucCmdApp_get_help_command(subject)));

    ucCmdApp_set_help_command(subject, "helpme");
    uc_TRUE(0 == strcmp("helpme", ucCmdApp_get_help_command(subject)));
uc_PASS

uc_TEST(ucCmdApp_get_help_command_returns_value)
    subject->help_command = "my_help_value";
    uc_TRUE(ucCmdApp_get_help_command(subject));
uc_PASS

uc_TEST(ucCmdApp_set_quit_command_sets_value)
    ucCmdApp_set_quit_command(subject, "q");
    uc_TRUE(0 == strcmp("q", ucCmdApp_get_quit_command(subject)));

    ucCmdApp_set_quit_command(subject, "exit");
    uc_TRUE(0 == strcmp("exit", ucCmdApp_get_quit_command(subject)));
uc_PASS

uc_TEST(ucCmdApp_get_quit_command_returns_value)
    subject->quit_command = "getout";
    uc_TRUE(ucCmdApp_get_quit_command(subject));
uc_PASS

uc_TEST(ucCmdApp_get_cmd_returns_value)
    uc_TRUE(cmd_line == ucCmdApp_get_cmd(subject));
uc_PASS

uc_TEST(ucCmdApp_get_cmd_parser_is_not_null)
    uc_TRUE(NULL != ucCmdApp_get_cmd_parser(subject));
uc_PASS

uc_TEST(ucCmdApp_get_escape_response_returns_value)
    subject->escape_response = "escape";
    uc_TRUE(0 == strcmp("escape", ucCmdApp_get_escape_response(subject)));
uc_PASS

uc_TEST(ucCmdApp_set_escape_response_sets_value)
    ucCmdApp_set_escape_response(subject, "esc");
    uc_TRUE(0 == strcmp("esc", subject->escape_response));
uc_PASS

uc_TEST(ucCmdApp_set_receive_state_sets_value)
    char *state = "val";
    void *prev_state = subject->receive_state;

    ucCmdApp_set_receive_state(subject, state);
    uc_TRUE(subject->receive_state == state);

    subject->receive_state = prev_state;
uc_PASS

uc_TEST(ucCmdApp_get_receive_state_gets_value)
    int state;
    void *prev_state = subject->receive_state;

    ucCmdApp_set_receive_state(subject, &state);
    uc_TRUE(&state == ucCmdApp_get_receive_state(subject));

    subject->receive_state = prev_state;
uc_PASS

uc_TEST(ucCmdApp_receive_uses_state)
    double state;
    ucCmdApp_ReceiveFunc *prev_func = subject->receive;
    void *prev_state = subject->receive_state;

    ucCmdApp_set_receive(subject, receive_1);
    ucCmdApp_set_receive_state(subject, &state);
    ucCmdApp_receive(subject);
    uc_TRUE(&state == receive_1_state);

    subject->receive = prev_func;
    subject->receive_state = prev_state;
uc_PASS

uc_TEST(ucCmdApp_get_cmd_str_size_max_gets_size)
    size_t size = ucCmdApp_get_cmd_str_size_max(subject);
    uc_TRUE((ucCmdApp_CMD_STR_SIZE) == size);
uc_PASS

static int ucCmdApp_run_ends_when_quit_is_received_count = 0;
static char *ucCmdApp_run_ends_when_quit_is_received_receive(char *buf, size_t buf_size, void *state) {
    if (0 == ucCmdApp_run_ends_when_quit_is_received_count) {
        strncpy(buf, "help", buf_size);
    }
    else {
        strncpy(buf, "quit", buf_size);
    }
    ucCmdApp_run_ends_when_quit_is_received_count++;

    return buf;
}

uc_TEST(ucCmdApp_run_ends_when_quit_is_received)
    ucCmd *cmd = ucCmdApp_get_cmd(subject);
    ucCmdApp_run_ends_when_quit_is_received_count = 0;
    ucCmdApp_set_receive(subject, ucCmdApp_run_ends_when_quit_is_received_receive);
    ucCmdApp_run(subject, NULL);
    uc_TRUE(2 == ucCmdApp_run_ends_when_quit_is_received_count);
uc_PASS

static int ucCmdApp_run_sends_response_terminator_after_command_completion_count;
static char *ucCmdApp_run_sends_response_terminator_after_command_completion_receive(char *buf, size_t buf_size, void *state) {
    if (0 == ucCmdApp_run_sends_response_terminator_after_command_completion_count) {
        strncpy(buf, "help", buf_size);
    }
    else {
        strncpy(buf, "quit", buf_size);
    }
    ucCmdApp_run_sends_response_terminator_after_command_completion_count++;
    return buf;
}

static int ucCmdApp_run_sends_response_terminator_after_command_completion_transmit_error;
static void ucCmdApp_run_sends_response_terminator_after_command_completion_transmit(const char *response, void *state) {
    if (1 == ucCmdApp_run_sends_response_terminator_after_command_completion_count) {
        if (0 != strcmp("End of transmission", response)) {
            ucCmdApp_run_sends_response_terminator_after_command_completion_transmit_error = 1;
        }
        else {
            ucCmdApp_run_sends_response_terminator_after_command_completion_transmit_error = 0;
        }
    }
}

uc_TEST(ucCmdApp_run_sends_response_terminator_after_command_completion)
    ucCmd *cmd = ucCmdApp_get_cmd(subject);
    ucCmdApp_run_sends_response_terminator_after_command_completion_count = 0;
    ucCmdApp_run_sends_response_terminator_after_command_completion_transmit_error = 1;
    ucCmd_set_transmit(cmd, ucCmdApp_run_sends_response_terminator_after_command_completion_transmit);
    ucCmdApp_set_receive(subject, ucCmdApp_run_sends_response_terminator_after_command_completion_receive);

    ucCmd_set_response_terminator(ucCmdApp_get_cmd(subject), "End of transmission");
    ucCmdApp_run(subject, NULL);
    uc_TRUE(0 == ucCmdApp_run_sends_response_terminator_after_command_completion_transmit_error);
uc_PASS

static int ucCmdApp_run_sends_command_acknowledgment_testing;
static char *ucCmdApp_run_sends_command_acknowledgment_receive(char *buf, size_t buf_size, void *state) {
    if (0 == ucCmdApp_run_sends_command_acknowledgment_testing) {
        strncpy(buf, "help", buf_size);
        ucCmdApp_run_sends_command_acknowledgment_testing = 1;
    }
    else {
        strncpy(buf, "quit", buf_size);
    }
    return buf;
}

static const char *ucCmdApp_run_sends_command_acknowledgment_value;
static int ucCmdApp_run_sends_command_acknowledgment_error;
static ucCmdApp_run_sends_command_acknowledgment_transmit(const char *response, void *state) {
    if (1 == ucCmdApp_run_sends_command_acknowledgment_testing) {
        if ( 0 != strcmp(ucCmdApp_run_sends_command_acknowledgment_value, response)) {
            ucCmdApp_run_sends_command_acknowledgment_error = 1;
        }
        else {
            ucCmdApp_run_sends_command_acknowledgment_error = 0;
        }
        ucCmdApp_run_sends_command_acknowledgment_testing = -1;
    }
}

static int ucCmdApp_run_sends_command_acknowledgment(ucTestGroup *p, const char *command_acknowledgment) {
    ucCmd *cmd = ucCmdApp_get_cmd(subject);
    ucCmdApp_run_sends_command_acknowledgment_error = 1;
    ucCmdApp_run_sends_command_acknowledgment_testing = 0;
    ucCmdApp_run_sends_command_acknowledgment_value = command_acknowledgment;
    ucCmd_set_command_acknowledgment(cmd, command_acknowledgment);
    ucCmd_set_transmit(cmd, ucCmdApp_run_sends_command_acknowledgment_transmit);
    ucCmdApp_set_receive(subject, ucCmdApp_run_sends_command_acknowledgment_receive);
    ucCmdApp_run(subject, NULL);
    uc_TRUE(0 == ucCmdApp_run_sends_command_acknowledgment_error);
    return 0;
}

uc_TEST(ucCmdApp_run_sends_command_acknowledgment_here_we_go)
    return ucCmdApp_run_sends_command_acknowledgment(p, "Here we GO!");
}

uc_TEST(ucCmdApp_run_sends_command_acknowledgment_dashes)
    return ucCmdApp_run_sends_command_acknowledgment(p, "----");
}

uc_TEST_GROUP(ucCmdApp, setup,
    ucCmdApp_set_receive_sets_receive,
    ucCmdApp_get_receive_returns_receive,
    ucCmdApp_set_help_command_sets_value,
    ucCmdApp_get_help_command_returns_value,
    ucCmdApp_set_quit_command_sets_value,
    ucCmdApp_get_quit_command_returns_value,
    ucCmdApp_get_cmd_returns_value,
    ucCmdApp_get_cmd_parser_is_not_null,
    ucCmdApp_get_escape_response_returns_value,
    ucCmdApp_set_escape_response_sets_value,
    ucCmdApp_set_receive_state_sets_value,
    ucCmdApp_get_receive_state_gets_value,
    ucCmdApp_receive_uses_state,
    ucCmdApp_get_cmd_str_size_max_gets_size,
    ucCmdApp_run_ends_when_quit_is_received,
    ucCmdApp_run_sends_response_terminator_after_command_completion,
    ucCmdApp_run_sends_command_acknowledgment_here_we_go,
    ucCmdApp_run_sends_command_acknowledgment_dashes)
