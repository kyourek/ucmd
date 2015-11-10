#include "ucmdtests.h"

static ucApp *subject;

void *receive_1_state;
static char *receive_1(char *buf, size_t buf_size, void *state) { 
    receive_1_state = state;
    return 0; 
}

static char *receive_2(char *buf, size_t buf_size, void *state) { return 0; }

uc_TEST(prior)
    subject = ucApp_create();
uc_PASS

uc_TEST(after)
    ucApp_destroy(subject);
uc_PASS

uc_TEST(setup)
    ucTestGroup_setup_test(p, prior, after);
uc_PASS

uc_TEST(ucApp_set_help_command_sets_value)
    ucApp_set_help_command(subject, "h");
    uc_TRUE(0 == strcmp("h", ucApp_get_help_command(subject)));

    ucApp_set_help_command(subject, "helpme");
    uc_TRUE(0 == strcmp("helpme", ucApp_get_help_command(subject)));
uc_PASS

uc_TEST(ucApp_get_help_command_returns_value)
    subject->help_command = "my_help_value";
    uc_TRUE(ucApp_get_help_command(subject));
uc_PASS

uc_TEST(ucApp_set_quit_command_sets_value)
    ucApp_set_quit_command(subject, "q");
    uc_TRUE(0 == strcmp("q", ucApp_get_quit_command(subject)));

    ucApp_set_quit_command(subject, "exit");
    uc_TRUE(0 == strcmp("exit", ucApp_get_quit_command(subject)));
uc_PASS

uc_TEST(ucApp_get_quit_command_returns_value)
    subject->quit_command = "getout";
    uc_TRUE(ucApp_get_quit_command(subject));
uc_PASS

uc_TEST(ucApp_get_cmd_returns_value)
    uc_TRUE(NULL != ucApp_get_cmd(subject));
uc_PASS

uc_TEST(ucApp_get_escape_response_returns_value)
    subject->escape_response = "escape";
    uc_TRUE(0 == strcmp("escape", ucApp_get_escape_response(subject)));
uc_PASS

uc_TEST(ucApp_set_escape_response_sets_value)
    ucApp_set_escape_response(subject, "esc");
    uc_TRUE(0 == strcmp("esc", subject->escape_response));
uc_PASS

static int ucApp_run_ends_when_quit_is_received_count = 0;
static char *ucApp_run_ends_when_quit_is_received_receive(char *buf, size_t buf_size, void *state) {
    if (0 == ucApp_run_ends_when_quit_is_received_count) {
        strncpy(buf, "help", buf_size);
    }
    else {
        strncpy(buf, "quit", buf_size);
    }
    ucApp_run_ends_when_quit_is_received_count++;

    return buf;
}

uc_TEST(ucApp_run_ends_when_quit_is_received)
    ucCmd *cmd = ucApp_get_cmd(subject);
    ucApp_run_ends_when_quit_is_received_count = 0;
    ucCmd_set_receive(cmd, ucApp_run_ends_when_quit_is_received_receive);
    ucApp_run(subject, NULL);
    uc_TRUE(2 == ucApp_run_ends_when_quit_is_received_count);
uc_PASS

static int ucApp_run_sends_response_terminator_after_command_completion_count;
static char *ucApp_run_sends_response_terminator_after_command_completion_receive(char *buf, size_t buf_size, void *state) {
    if (0 == ucApp_run_sends_response_terminator_after_command_completion_count) {
        strncpy(buf, "help", buf_size);
    }
    else {
        strncpy(buf, "quit", buf_size);
    }
    ucApp_run_sends_response_terminator_after_command_completion_count++;
    return buf;
}

static int ucApp_run_sends_response_terminator_after_command_completion_transmit_error;
static void ucApp_run_sends_response_terminator_after_command_completion_transmit(const char *response, void *state) {
    if (1 == ucApp_run_sends_response_terminator_after_command_completion_count) {
        if (0 != strcmp("End of transmission", response)) {
            ucApp_run_sends_response_terminator_after_command_completion_transmit_error = 1;
        }
        else {
            ucApp_run_sends_response_terminator_after_command_completion_transmit_error = 0;
        }
    }
}

uc_TEST(ucApp_run_sends_response_terminator_after_command_completion)
    ucCmd *cmd = ucApp_get_cmd(subject);
    ucApp_run_sends_response_terminator_after_command_completion_count = 0;
    ucApp_run_sends_response_terminator_after_command_completion_transmit_error = 1;
    ucCmd_set_transmit(cmd, ucApp_run_sends_response_terminator_after_command_completion_transmit);
    ucCmd_set_receive(cmd, ucApp_run_sends_response_terminator_after_command_completion_receive);

    ucCmd_set_response_terminator(ucApp_get_cmd(subject), "End of transmission");
    ucApp_run(subject, NULL);
    uc_TRUE(0 == ucApp_run_sends_response_terminator_after_command_completion_transmit_error);
uc_PASS

static int ucApp_run_sends_command_acknowledgment_testing;
static char *ucApp_run_sends_command_acknowledgment_receive(char *buf, size_t buf_size, void *state) {
    if (0 == ucApp_run_sends_command_acknowledgment_testing) {
        strncpy(buf, "help", buf_size);
        ucApp_run_sends_command_acknowledgment_testing = 1;
    }
    else {
        strncpy(buf, "quit", buf_size);
    }
    return buf;
}

static const char *ucApp_run_sends_command_acknowledgment_value;
static int ucApp_run_sends_command_acknowledgment_error;
static ucApp_run_sends_command_acknowledgment_transmit(const char *response, void *state) {
    if (1 == ucApp_run_sends_command_acknowledgment_testing) {
        if ( 0 != strcmp(ucApp_run_sends_command_acknowledgment_value, response)) {
            ucApp_run_sends_command_acknowledgment_error = 1;
        }
        else {
            ucApp_run_sends_command_acknowledgment_error = 0;
        }
        ucApp_run_sends_command_acknowledgment_testing = -1;
    }
}

static int ucApp_run_sends_command_acknowledgment(ucTestGroup *p, const char *command_acknowledgment) {
    ucCmd *cmd = ucApp_get_cmd(subject);
    ucApp_run_sends_command_acknowledgment_error = 1;
    ucApp_run_sends_command_acknowledgment_testing = 0;
    ucApp_run_sends_command_acknowledgment_value = command_acknowledgment;
    ucCmd_set_command_acknowledgment(cmd, command_acknowledgment);
    ucCmd_set_transmit(cmd, ucApp_run_sends_command_acknowledgment_transmit);
    ucCmd_set_receive(cmd, ucApp_run_sends_command_acknowledgment_receive);
    ucApp_run(subject, NULL);
    uc_TRUE(0 == ucApp_run_sends_command_acknowledgment_error);
    return 0;
}

uc_TEST(ucApp_run_sends_command_acknowledgment_here_we_go)
    return ucApp_run_sends_command_acknowledgment(p, "Here we GO!");
}

uc_TEST(ucApp_run_sends_command_acknowledgment_dashes)
    return ucApp_run_sends_command_acknowledgment(p, "----");
}

uc_TEST_GROUP(ucApp, setup,
    ucApp_set_help_command_sets_value,
    ucApp_get_help_command_returns_value,
    ucApp_set_quit_command_sets_value,
    ucApp_get_quit_command_returns_value,
    ucApp_get_cmd_returns_value,
    ucApp_get_escape_response_returns_value,
    ucApp_set_escape_response_sets_value,
    ucApp_run_ends_when_quit_is_received,
    ucApp_run_sends_response_terminator_after_command_completion,
    ucApp_run_sends_command_acknowledgment_here_we_go,
    ucApp_run_sends_command_acknowledgment_dashes)
