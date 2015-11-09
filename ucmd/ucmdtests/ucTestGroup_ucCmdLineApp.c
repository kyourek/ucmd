#include "ucmdtests.h"

static ucCmdLine *cmd_line;
static ucCmdParser *cmd_parser;
static ucCmdLineApp *subject;

void *receive_1_state;
static char *receive_1(char *buf, size_t buf_size, void *state) { 
    receive_1_state = state;
    return 0; 
}

static char *receive_2(char *buf, size_t buf_size, void *state) { return 0; }

uc_TEST(prior)
    subject = ucCmdLineApp_create();
    cmd_line = ucCmdLineApp_get_cmd(subject);
    cmd_parser = ucCmdLineApp_get_cmd_parser(subject);
uc_PASS

uc_TEST(after)
    ucCmdLineApp_destroy(subject);
uc_PASS

uc_TEST(setup)
    ucTestGroup_setup_test(p, prior, after);
uc_PASS

uc_TEST(ucCmdLineApp_set_receive_sets_receive)
    ucCmdLineApp_set_receive(subject, receive_1);
    uc_TRUE(receive_1 == ucCmdLineApp_get_receive(subject));
uc_PASS

uc_TEST(ucCmdLineApp_get_receive_returns_receive)
    subject->receive = receive_2;
    uc_TRUE(receive_2 == ucCmdLineApp_get_receive(subject));
uc_PASS

uc_TEST(ucCmdLineApp_set_help_command_sets_value)
    ucCmdLineApp_set_help_command(subject, "h");
    uc_TRUE(0 == strcmp("h", ucCmdLineApp_get_help_command(subject)));

    ucCmdLineApp_set_help_command(subject, "helpme");
    uc_TRUE(0 == strcmp("helpme", ucCmdLineApp_get_help_command(subject)));
uc_PASS

uc_TEST(ucCmdLineApp_get_help_command_returns_value)
    subject->help_command = "my_help_value";
    uc_TRUE(ucCmdLineApp_get_help_command(subject));
uc_PASS

uc_TEST(ucCmdLineApp_set_quit_command_sets_value)
    ucCmdLineApp_set_quit_command(subject, "q");
    uc_TRUE(0 == strcmp("q", ucCmdLineApp_get_quit_command(subject)));

    ucCmdLineApp_set_quit_command(subject, "exit");
    uc_TRUE(0 == strcmp("exit", ucCmdLineApp_get_quit_command(subject)));
uc_PASS

uc_TEST(ucCmdLineApp_get_quit_command_returns_value)
    subject->quit_command = "getout";
    uc_TRUE(ucCmdLineApp_get_quit_command(subject));
uc_PASS

uc_TEST(ucCmdLineApp_get_cmd_returns_value)
    uc_TRUE(cmd_line == ucCmdLineApp_get_cmd(subject));
uc_PASS

uc_TEST(ucCmdLineApp_get_cmd_parser_is_not_null)
    uc_TRUE(NULL != ucCmdLineApp_get_cmd_parser(subject));
uc_PASS

uc_TEST(ucCmdLineApp_get_escape_response_returns_value)
    subject->escape_response = "escape";
    uc_TRUE(0 == strcmp("escape", ucCmdLineApp_get_escape_response(subject)));
uc_PASS

uc_TEST(ucCmdLineApp_set_escape_response_sets_value)
    ucCmdLineApp_set_escape_response(subject, "esc");
    uc_TRUE(0 == strcmp("esc", subject->escape_response));
uc_PASS

uc_TEST(ucCmdLineApp_set_receive_state_sets_value)
    char *state = "val";
    void *prev_state = subject->receive_state;

    ucCmdLineApp_set_receive_state(subject, state);
    uc_TRUE(subject->receive_state == state);

    subject->receive_state = prev_state;
uc_PASS

uc_TEST(ucCmdLineApp_get_receive_state_gets_value)
    int state;
    void *prev_state = subject->receive_state;

    ucCmdLineApp_set_receive_state(subject, &state);
    uc_TRUE(&state == ucCmdLineApp_get_receive_state(subject));

    subject->receive_state = prev_state;
uc_PASS

uc_TEST(ucCmdLineApp_receive_uses_state)
    double state;
    ucCmdLineApp_ReceiveFunc *prev_func = subject->receive;
    void *prev_state = subject->receive_state;

    ucCmdLineApp_set_receive(subject, receive_1);
    ucCmdLineApp_set_receive_state(subject, &state);
    ucCmdLineApp_receive(subject);
    uc_TRUE(&state == receive_1_state);

    subject->receive = prev_func;
    subject->receive_state = prev_state;
uc_PASS

uc_TEST(ucCmdLineApp_get_cmd_str_size_max_gets_size)
    size_t size = ucCmdLineApp_get_cmd_str_size_max(subject);
    uc_TRUE((ucCmdLineApp_CMD_STR_SIZE) == size);
uc_PASS

static int ucCmdLineApp_run_ends_when_quit_is_received_count = 0;
static char *ucCmdLineApp_run_ends_when_quit_is_received_receive(char *buf, size_t buf_size, void *state) {
    if (0 == ucCmdLineApp_run_ends_when_quit_is_received_count) {
        strncpy(buf, "help", buf_size);
    }
    else {
        strncpy(buf, "quit", buf_size);
    }
    ucCmdLineApp_run_ends_when_quit_is_received_count++;

    return buf;
}

uc_TEST(ucCmdLineApp_run_ends_when_quit_is_received)
    ucCmdLine *cmd = ucCmdLineApp_get_cmd(subject);
    ucCmdLineApp_run_ends_when_quit_is_received_count = 0;
    ucCmdLineApp_set_receive(subject, ucCmdLineApp_run_ends_when_quit_is_received_receive);
    ucCmdLineApp_run(subject, NULL);
    uc_TRUE(2 == ucCmdLineApp_run_ends_when_quit_is_received_count);
uc_PASS

static int ucCmdLineApp_run_sends_response_terminator_after_command_completion_count;
static char *ucCmdLineApp_run_sends_response_terminator_after_command_completion_receive(char *buf, size_t buf_size, void *state) {
    if (0 == ucCmdLineApp_run_sends_response_terminator_after_command_completion_count) {
        strncpy(buf, "help", buf_size);
    }
    else {
        strncpy(buf, "quit", buf_size);
    }
    ucCmdLineApp_run_sends_response_terminator_after_command_completion_count++;
    return buf;
}

static int ucCmdLineApp_run_sends_response_terminator_after_command_completion_transmit_error;
static void ucCmdLineApp_run_sends_response_terminator_after_command_completion_transmit(const char *response, void *state) {
    if (1 == ucCmdLineApp_run_sends_response_terminator_after_command_completion_count) {
        if (0 != strcmp("End of transmission", response)) {
            ucCmdLineApp_run_sends_response_terminator_after_command_completion_transmit_error = 1;
        }
        else {
            ucCmdLineApp_run_sends_response_terminator_after_command_completion_transmit_error = 0;
        }
    }
}

uc_TEST(ucCmdLineApp_run_sends_response_terminator_after_command_completion)
    ucCmdLine *cmd = ucCmdLineApp_get_cmd(subject);
    ucCmdLineApp_run_sends_response_terminator_after_command_completion_count = 0;
    ucCmdLineApp_run_sends_response_terminator_after_command_completion_transmit_error = 1;
    ucCmdLine_set_transmit(cmd, ucCmdLineApp_run_sends_response_terminator_after_command_completion_transmit);
    ucCmdLineApp_set_receive(subject, ucCmdLineApp_run_sends_response_terminator_after_command_completion_receive);

    ucCmdLine_set_response_terminator(ucCmdLineApp_get_cmd(subject), "End of transmission");
    ucCmdLineApp_run(subject, NULL);
    uc_TRUE(0 == ucCmdLineApp_run_sends_response_terminator_after_command_completion_transmit_error);
uc_PASS

static int ucCmdLineApp_run_sends_command_acknowledgment_testing;
static char *ucCmdLineApp_run_sends_command_acknowledgment_receive(char *buf, size_t buf_size, void *state) {
    if (0 == ucCmdLineApp_run_sends_command_acknowledgment_testing) {
        strncpy(buf, "help", buf_size);
        ucCmdLineApp_run_sends_command_acknowledgment_testing = 1;
    }
    else {
        strncpy(buf, "quit", buf_size);
    }
    return buf;
}

static const char *ucCmdLineApp_run_sends_command_acknowledgment_value;
static int ucCmdLineApp_run_sends_command_acknowledgment_error;
static ucCmdLineApp_run_sends_command_acknowledgment_transmit(const char *response, void *state) {
    if (1 == ucCmdLineApp_run_sends_command_acknowledgment_testing) {
        if ( 0 != strcmp(ucCmdLineApp_run_sends_command_acknowledgment_value, response)) {
            ucCmdLineApp_run_sends_command_acknowledgment_error = 1;
        }
        else {
            ucCmdLineApp_run_sends_command_acknowledgment_error = 0;
        }
        ucCmdLineApp_run_sends_command_acknowledgment_testing = -1;
    }
}

static int ucCmdLineApp_run_sends_command_acknowledgment(ucTestGroup *p, const char *command_acknowledgment) {
    ucCmdLine *cmd = ucCmdLineApp_get_cmd(subject);
    ucCmdLineApp_run_sends_command_acknowledgment_error = 1;
    ucCmdLineApp_run_sends_command_acknowledgment_testing = 0;
    ucCmdLineApp_run_sends_command_acknowledgment_value = command_acknowledgment;
    ucCmdLine_set_command_acknowledgment(cmd, command_acknowledgment);
    ucCmdLine_set_transmit(cmd, ucCmdLineApp_run_sends_command_acknowledgment_transmit);
    ucCmdLineApp_set_receive(subject, ucCmdLineApp_run_sends_command_acknowledgment_receive);
    ucCmdLineApp_run(subject, NULL);
    uc_TRUE(0 == ucCmdLineApp_run_sends_command_acknowledgment_error);
    return 0;
}

uc_TEST(ucCmdLineApp_run_sends_command_acknowledgment_here_we_go)
    return ucCmdLineApp_run_sends_command_acknowledgment(p, "Here we GO!");
}

uc_TEST(ucCmdLineApp_run_sends_command_acknowledgment_dashes)
    return ucCmdLineApp_run_sends_command_acknowledgment(p, "----");
}

uc_TEST_GROUP(ucCmdLineApp, setup,
    ucCmdLineApp_set_receive_sets_receive,
    ucCmdLineApp_get_receive_returns_receive,
    ucCmdLineApp_set_help_command_sets_value,
    ucCmdLineApp_get_help_command_returns_value,
    ucCmdLineApp_set_quit_command_sets_value,
    ucCmdLineApp_get_quit_command_returns_value,
    ucCmdLineApp_get_cmd_returns_value,
    ucCmdLineApp_get_cmd_parser_is_not_null,
    ucCmdLineApp_get_escape_response_returns_value,
    ucCmdLineApp_set_escape_response_sets_value,
    ucCmdLineApp_set_receive_state_sets_value,
    ucCmdLineApp_get_receive_state_gets_value,
    ucCmdLineApp_receive_uses_state,
    ucCmdLineApp_get_cmd_str_size_max_gets_size,
    ucCmdLineApp_run_ends_when_quit_is_received,
    ucCmdLineApp_run_sends_response_terminator_after_command_completion,
    ucCmdLineApp_run_sends_command_acknowledgment_here_we_go,
    ucCmdLineApp_run_sends_command_acknowledgment_dashes)
