#include <string.h>
#include "ucTest.h"

void *receive_1_state;
static char *receive_1(char *buf, size_t buf_size, void *state) { 
    receive_1_state = state;
    return 0; 
}

static char *receive_2(char *buf, size_t buf_size, void *state) { return 0; }

static ucCmdLineApp *init_subject(void) {
    return ucCmdLineApp_init(ucCmdLineApp_get_instance(), ucCmdParser_get_instance(), ucCmdLine_get_instance());
}

static ucTestErr ucCmdLineApp_get_instance_is_not_null(ucTestGroup *p) {
    ucTest_ASSERT(NULL != ucCmdLineApp_get_instance());
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLineApp_set_receive_sets_receive(ucTestGroup *p) {
    ucCmdLineApp ptr;
    ucCmdLineApp_set_receive(&ptr, receive_1);
    ucTest_ASSERT(receive_1 == ucCmdLineApp_get_receive(&ptr));
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLineApp_get_receive_returns_receive(ucTestGroup *p) {
    ucCmdLineApp a;
    a.receive = receive_2;
    ucTest_ASSERT(receive_2 == ucCmdLineApp_get_receive(&a));
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLineApp_set_help_command_sets_value(ucTestGroup *p) {
    ucCmdLineApp ptr;
    ucCmdLineApp_set_help_command(&ptr, "h");
    ucTest_ASSERT(0 == strcmp("h", ucCmdLineApp_get_help_command(&ptr)));

    ucCmdLineApp_set_help_command(&ptr, "helpme");
    ucTest_ASSERT(0 == strcmp("helpme", ucCmdLineApp_get_help_command(&ptr)));
    
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLineApp_get_help_command_returns_value(ucTestGroup *p) {
    ucCmdLineApp a;
    a.help_command = "my_help_value";
    ucTest_ASSERT(ucCmdLineApp_get_help_command(&a));
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLineApp_set_quit_command_sets_value(ucTestGroup *p) {
    ucCmdLineApp ptr;
    ucCmdLineApp_set_quit_command(&ptr, "q");
    ucTest_ASSERT(0 == strcmp("q", ucCmdLineApp_get_quit_command(&ptr)));

    ucCmdLineApp_set_quit_command(&ptr, "exit");
    ucTest_ASSERT(0 == strcmp("exit", ucCmdLineApp_get_quit_command(&ptr)));
    
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLineApp_get_quit_command_returns_value(ucTestGroup *p) {
    ucCmdLineApp a;
    a.quit_command = "getout";
    ucTest_ASSERT(ucCmdLineApp_get_quit_command(&a));
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLineApp_get_cmd_returns_value(ucTestGroup *p) {
    ucCmdLineApp a;
    ucCmdLine c;
    a.cmd = &c;
    ucTest_ASSERT(&c == ucCmdLineApp_get_cmd(&a));
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLineApp_set_cmd_sets_value(ucTestGroup *p) {
    ucCmdLineApp a;
    ucCmdLine c;
    ucCmdLineApp_set_cmd(&a, &c);
    ucTest_ASSERT(&c == a.cmd);
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLineApp_get_cmd_parser_is_not_null(ucTestGroup *p) {
    ucCmdLineApp *a = ucCmdLineApp_get_instance();
    ucTest_ASSERT(NULL != ucCmdLineApp_get_cmd_parser(a));
    return ucTestErr_NONE;    
}

static ucTestErr ucCmdLineApp_get_escape_response_returns_value(ucTestGroup *p) {
    ucCmdLineApp a;
    a.escape_response = "escape";
    ucTest_ASSERT(0 == strcmp("escape", ucCmdLineApp_get_escape_response(&a)));
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLineApp_set_escape_response_sets_value(ucTestGroup *p) {
    ucCmdLineApp a;
    ucCmdLineApp_set_escape_response(&a, "esc");
    ucTest_ASSERT(0 == strcmp("esc", a.escape_response));
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLineApp_set_receive_state_sets_value(ucTestGroup *p) {
    char *state = "val";
    ucCmdLineApp *ptr = ucCmdLineApp_get_instance();
    void *prev_state = ptr->receive_state;

    ucCmdLineApp_set_receive_state(ptr, state);
    ucTest_ASSERT(ptr->receive_state == state);

    ptr->receive_state = prev_state;
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLineApp_get_receive_state_gets_value(ucTestGroup *p) {
    int state;
    ucCmdLineApp *ptr = ucCmdLineApp_get_instance();
    void *prev_state = ptr->receive_state;

    ucCmdLineApp_set_receive_state(ptr, &state);
    ucTest_ASSERT(&state == ucCmdLineApp_get_receive_state(ptr));

    ptr->receive_state = prev_state;
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLineApp_receive_uses_state(ucTestGroup *p) {
    double state;
    ucCmdLineApp *ptr = ucCmdLineApp_get_instance();
    ucCmdLineApp_ReceiveFunc *prev_func = ptr->receive;
    void *prev_state = ptr->receive_state;

    ucCmdLineApp_set_receive(ptr, receive_1);
    ucCmdLineApp_set_receive_state(ptr, &state);
    ucCmdLineApp_receive(ptr);
    ucTest_ASSERT(&state == receive_1_state);

    ptr->receive = prev_func;
    ptr->receive_state = prev_state;
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLineApp_response_terminator_is_initially_null(ucTestGroup *p) {
    ucCmdLineApp *subject = init_subject();
    ucTest_ASSERT(NULL == subject->response_terminator);
    return ucTestErr_NONE;
}

static ucTestErr ucCmdLineApp_get_response_terminator_returns_set_value(ucTestGroup *p) {
    ucCmdLineApp *subject = init_subject();
    const char *expected, *actual, *values[] = { "EOT", "\x1b", "We're DONE!" };
    int i;
    for (i = 0; i < 3; i++) {
        expected = values[i];
        ucCmdLineApp_set_response_terminator(subject, expected);
        actual = ucCmdLineApp_get_response_terminator(subject);
        ucTest_ASSERT(0 == strcmp(expected, actual));
        ucTest_ASSERT(expected == actual);
    }
    return ucTestErr_NONE;
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
    ucCmdLineApp *subject = init_subject();
    ucCmdLine *cmd = ucCmdLineApp_get_cmd(subject);
    ucCmdLineApp_run_ends_when_quit_is_received_count = 0;
    ucCmdLineApp_set_receive(subject, ucCmdLineApp_run_ends_when_quit_is_received_receive);
    ucCmdLineApp_run(subject, NULL);
    ucTest_ASSERT(2 == ucCmdLineApp_run_ends_when_quit_is_received_count);
    return ucTestErr_NONE;
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
    ucCmdLineApp *subject = init_subject();
    ucCmdLine *cmd = ucCmdLineApp_get_cmd(subject);
    ucCmdLineApp_run_sends_response_terminator_after_command_completion_count = 0;
    ucCmdLineApp_run_sends_response_terminator_after_command_completion_transmit_error = 1;
    ucCmdLine_set_transmit(cmd, ucCmdLineApp_run_sends_response_terminator_after_command_completion_transmit);
    ucCmdLineApp_set_receive(subject, ucCmdLineApp_run_sends_response_terminator_after_command_completion_receive);

    ucCmdLineApp_set_response_terminator(subject, "End of transmission");
    ucCmdLineApp_run(subject, NULL);
    ucTest_ASSERT(0 == ucCmdLineApp_run_sends_response_terminator_after_command_completion_transmit_error);
    return ucTestErr_NONE;
}

ucTestGroup *ucCmdLineApp_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_TestFunc *tests[] = {
        ucCmdLineApp_get_instance_is_not_null,
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
        ucCmdLineApp_response_terminator_is_initially_null,
        ucCmdLineApp_get_response_terminator_returns_set_value,
        ucCmdLineApp_run_ends_when_quit_is_received,
        ucCmdLineApp_run_sends_response_terminator_after_command_completion,
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
