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

static ucTestErr before_each_test(ucTestGroup *p) {
    subject = ucCmdLineApp_create();
    cmd_line = ucCmdLineApp_get_cmd(subject);
    cmd_parser = ucCmdLineApp_get_cmd_parser(subject);
    ucPASS();
}

static ucTestErr after_each_test(ucTestGroup *p) {
    ucCmdLineApp_destroy(subject);
    ucPASS();
}

static ucTestErr ucCmdLineApp_set_receive_sets_receive(ucTestGroup *p) {
    ucCmdLineApp_set_receive(subject, receive_1);
    ucTRUE(receive_1 == ucCmdLineApp_get_receive(subject));
    ucPASS();
}

static ucTestErr ucCmdLineApp_get_receive_returns_receive(ucTestGroup *p) {
    subject->receive = receive_2;
    ucTRUE(receive_2 == ucCmdLineApp_get_receive(subject));
    ucPASS();
}

static ucTestErr ucCmdLineApp_set_help_command_sets_value(ucTestGroup *p) {
    ucCmdLineApp_set_help_command(subject, "h");
    ucTRUE(0 == strcmp("h", ucCmdLineApp_get_help_command(subject)));

    ucCmdLineApp_set_help_command(subject, "helpme");
    ucTRUE(0 == strcmp("helpme", ucCmdLineApp_get_help_command(subject)));
    
    ucPASS();
}

static ucTestErr ucCmdLineApp_get_help_command_returns_value(ucTestGroup *p) {
    subject->help_command = "my_help_value";
    ucTRUE(ucCmdLineApp_get_help_command(subject));
    ucPASS();
}

static ucTestErr ucCmdLineApp_set_quit_command_sets_value(ucTestGroup *p) {
    ucCmdLineApp_set_quit_command(subject, "q");
    ucTRUE(0 == strcmp("q", ucCmdLineApp_get_quit_command(subject)));

    ucCmdLineApp_set_quit_command(subject, "exit");
    ucTRUE(0 == strcmp("exit", ucCmdLineApp_get_quit_command(subject)));
    
    ucPASS();
}

static ucTestErr ucCmdLineApp_get_quit_command_returns_value(ucTestGroup *p) {
    subject->quit_command = "getout";
    ucTRUE(ucCmdLineApp_get_quit_command(subject));
    ucPASS();
}

static ucTestErr ucCmdLineApp_get_cmd_returns_value(ucTestGroup *p) {
    ucTRUE(cmd_line == ucCmdLineApp_get_cmd(subject));
    ucPASS();
}

static ucTestErr ucCmdLineApp_set_cmd_sets_value(ucTestGroup *p) {
    ucCmdLineApp_set_cmd(subject, cmd_line);
    ucTRUE(cmd_line == subject->cmd);
    ucPASS();
}

static ucTestErr ucCmdLineApp_get_cmd_parser_is_not_null(ucTestGroup *p) {
    ucTRUE(NULL != ucCmdLineApp_get_cmd_parser(subject));
    ucPASS();    
}

static ucTestErr ucCmdLineApp_get_escape_response_returns_value(ucTestGroup *p) {
    subject->escape_response = "escape";
    ucTRUE(0 == strcmp("escape", ucCmdLineApp_get_escape_response(subject)));
    ucPASS();
}

static ucTestErr ucCmdLineApp_set_escape_response_sets_value(ucTestGroup *p) {
    ucCmdLineApp_set_escape_response(subject, "esc");
    ucTRUE(0 == strcmp("esc", subject->escape_response));
    ucPASS();
}

static ucTestErr ucCmdLineApp_set_receive_state_sets_value(ucTestGroup *p) {
    char *state = "val";
    void *prev_state = subject->receive_state;

    ucCmdLineApp_set_receive_state(subject, state);
    ucTRUE(subject->receive_state == state);

    subject->receive_state = prev_state;
    ucPASS();
}

static ucTestErr ucCmdLineApp_get_receive_state_gets_value(ucTestGroup *p) {
    int state;
    void *prev_state = subject->receive_state;

    ucCmdLineApp_set_receive_state(subject, &state);
    ucTRUE(&state == ucCmdLineApp_get_receive_state(subject));

    subject->receive_state = prev_state;
    ucPASS();
}

static ucTestErr ucCmdLineApp_receive_uses_state(ucTestGroup *p) {
    double state;
    ucCmdLineApp_ReceiveFunc *prev_func = subject->receive;
    void *prev_state = subject->receive_state;

    ucCmdLineApp_set_receive(subject, receive_1);
    ucCmdLineApp_set_receive_state(subject, &state);
    ucCmdLineApp_receive(subject);
    ucTRUE(&state == receive_1_state);

    subject->receive = prev_func;
    subject->receive_state = prev_state;
    ucPASS();
}

static ucTestErr ucCmdLineApp_get_cmd_str_size_max_gets_size(ucTestGroup *p) {
    size_t size = ucCmdLineApp_get_cmd_str_size_max(subject);
    ucTRUE((ucCmdLineApp_CMD_STR_SIZE) == size);
    ucPASS();
}

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

static ucTestErr ucCmdLineApp_run_ends_when_quit_is_received(ucTestGroup *p) {
    ucCmdLine *cmd = ucCmdLineApp_get_cmd(subject);
    ucCmdLineApp_run_ends_when_quit_is_received_count = 0;
    ucCmdLineApp_set_receive(subject, ucCmdLineApp_run_ends_when_quit_is_received_receive);
    ucCmdLineApp_run(subject, NULL);
    ucTRUE(2 == ucCmdLineApp_run_ends_when_quit_is_received_count);
    ucPASS();
}

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

static ucTestErr ucCmdLineApp_run_sends_response_terminator_after_command_completion(ucTestGroup *p) {
    ucCmdLine *cmd = ucCmdLineApp_get_cmd(subject);
    ucCmdLineApp_run_sends_response_terminator_after_command_completion_count = 0;
    ucCmdLineApp_run_sends_response_terminator_after_command_completion_transmit_error = 1;
    ucCmdLine_set_transmit(cmd, ucCmdLineApp_run_sends_response_terminator_after_command_completion_transmit);
    ucCmdLineApp_set_receive(subject, ucCmdLineApp_run_sends_response_terminator_after_command_completion_receive);

    ucCmdLine_set_response_terminator(ucCmdLineApp_get_cmd(subject), "End of transmission");
    ucCmdLineApp_run(subject, NULL);
    ucTRUE(0 == ucCmdLineApp_run_sends_response_terminator_after_command_completion_transmit_error);
    ucPASS();
}

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

static ucTestErr ucCmdLineApp_run_sends_command_acknowledgment(ucTestGroup *p, const char *command_acknowledgment) {
    ucCmdLine *cmd = ucCmdLineApp_get_cmd(subject);
    ucCmdLineApp_run_sends_command_acknowledgment_error = 1;
    ucCmdLineApp_run_sends_command_acknowledgment_testing = 0;
    ucCmdLineApp_run_sends_command_acknowledgment_value = command_acknowledgment;
    ucCmdLine_set_command_acknowledgment(cmd, command_acknowledgment);
    ucCmdLine_set_transmit(cmd, ucCmdLineApp_run_sends_command_acknowledgment_transmit);
    ucCmdLineApp_set_receive(subject, ucCmdLineApp_run_sends_command_acknowledgment_receive);
    ucCmdLineApp_run(subject, NULL);
    ucTRUE(0 == ucCmdLineApp_run_sends_command_acknowledgment_error);
    ucPASS();
}

static ucTestErr ucCmdLineApp_run_sends_command_acknowledgment_here_we_go(ucTestGroup *p) {
    return ucCmdLineApp_run_sends_command_acknowledgment(p, "Here we GO!");
}

static ucTestErr ucCmdLineApp_run_sends_command_acknowledgment_dashes(ucTestGroup *p) {
    return ucCmdLineApp_run_sends_command_acknowledgment(p, "----");
}

ucTestGroup *ucCmdLineApp_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_TestFunc *tests[] = {
        ucCmdLineApp_set_receive_sets_receive,
        ucCmdLineApp_get_receive_returns_receive,
        ucCmdLineApp_set_help_command_sets_value,
        ucCmdLineApp_get_help_command_returns_value,
        ucCmdLineApp_set_quit_command_sets_value,
        ucCmdLineApp_get_quit_command_returns_value,
        ucCmdLineApp_get_cmd_returns_value,
        ucCmdLineApp_set_cmd_sets_value,
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
        ucCmdLineApp_run_sends_command_acknowledgment_dashes,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, before_each_test, after_each_test, tests);
}
