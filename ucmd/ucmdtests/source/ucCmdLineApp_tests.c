#include <string.h>
#include "ucCmdLine_p.h"
#include "ucCmdLineApp_p.h"
#include "ucTest.h"

void *receive_1_state;
static char *receive_1(char *buf, size_t buf_size, void *state) { 
    receive_1_state = state;
    return 0; 
}

static char *receive_2(char *buf, size_t buf_size, void *state) { return 0; }

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
    ucCmdLineApp_receive_func *prev_func = ptr->receive;
    void *prev_state = ptr->receive_state;

    ucCmdLineApp_set_receive(ptr, receive_1);
    ucCmdLineApp_set_receive_state(ptr, &state);
    ucCmdLineApp_receive(ptr);
    ucTest_ASSERT(&state == receive_1_state);

    ptr->receive = prev_func;
    ptr->receive_state = prev_state;
    return ucTestErr_NONE;
}

ucTestGroup *ucCmdLineApp_tests_get_group(void) {
    static ucTestGroup group;
    static ucTestGroup_test_func *tests[] = {
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
        NULL
    };

    return ucTestGroup_init(&group, NULL, NULL, NULL, NULL, tests);
}
