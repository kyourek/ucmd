#include <string.h>
#include "uc_cmd_line_p.h"
#include "uc_cmd_line_app_p.h"
#include "uc_cmd_line_app_tests.h"
#include "uc_test.h"

void *receive_1_state;
static char *receive_1(char *buf, size_t buf_size, void *state) { 
    receive_1_state = state;
    return 0; 
}

static char *receive_2(char *buf, size_t buf_size, void *state) { return 0; }

static uc_test_err uc_cmd_line_app_get_instance_is_not_null(uc_test_group *p) {
    UC_TEST_ASSERT(NULL != uc_cmd_line_app_get_instance());
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_app_set_receive_sets_receive(uc_test_group *p) {
    uc_cmd_line_app ptr;
    uc_cmd_line_app_set_receive(&ptr, receive_1);
    UC_TEST_ASSERT(receive_1 == uc_cmd_line_app_get_receive(&ptr));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_app_get_receive_returns_receive(uc_test_group *p) {
    uc_cmd_line_app a;
    a.receive = receive_2;
    UC_TEST_ASSERT(receive_2 == uc_cmd_line_app_get_receive(&a));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_app_set_help_command_sets_value(uc_test_group *p) {
    uc_cmd_line_app ptr;
    uc_cmd_line_app_set_help_command(&ptr, "h");
    UC_TEST_ASSERT(0 == strcmp("h", uc_cmd_line_app_get_help_command(&ptr)));

    uc_cmd_line_app_set_help_command(&ptr, "helpme");
    UC_TEST_ASSERT(0 == strcmp("helpme", uc_cmd_line_app_get_help_command(&ptr)));
    
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_app_get_help_command_returns_value(uc_test_group *p) {
    uc_cmd_line_app a;
    a.help_command = "my_help_value";
    UC_TEST_ASSERT(uc_cmd_line_app_get_help_command(&a));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_app_set_quit_command_sets_value(uc_test_group *p) {
    uc_cmd_line_app ptr;
    uc_cmd_line_app_set_quit_command(&ptr, "q");
    UC_TEST_ASSERT(0 == strcmp("q", uc_cmd_line_app_get_quit_command(&ptr)));

    uc_cmd_line_app_set_quit_command(&ptr, "exit");
    UC_TEST_ASSERT(0 == strcmp("exit", uc_cmd_line_app_get_quit_command(&ptr)));
    
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_app_get_quit_command_returns_value(uc_test_group *p) {
    uc_cmd_line_app a;
    a.quit_command = "getout";
    UC_TEST_ASSERT(uc_cmd_line_app_get_quit_command(&a));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_app_get_cmd_returns_value(uc_test_group *p) {
    uc_cmd_line_app a;
    uc_cmd_line c;
    a.cmd = &c;
    UC_TEST_ASSERT(&c == uc_cmd_line_app_get_cmd(&a));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_app_set_cmd_sets_value(uc_test_group *p) {
    uc_cmd_line_app a;
    uc_cmd_line c;
    uc_cmd_line_app_set_cmd(&a, &c);
    UC_TEST_ASSERT(&c == a.cmd);
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_app_get_cmd_parser_is_not_null(uc_test_group *p) {
    uc_cmd_line_app *a = uc_cmd_line_app_get_instance();
    UC_TEST_ASSERT(NULL != uc_cmd_line_app_get_cmd_parser(a));
    return UC_TEST_ERR_NONE;    
}

static uc_test_err uc_cmd_line_app_get_escape_response_returns_value(uc_test_group *p) {
    uc_cmd_line_app a;
    a.escape_response = "escape";
    UC_TEST_ASSERT(0 == strcmp("escape", uc_cmd_line_app_get_escape_response(&a)));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_app_set_escape_response_sets_value(uc_test_group *p) {
    uc_cmd_line_app a;
    uc_cmd_line_app_set_escape_response(&a, "esc");
    UC_TEST_ASSERT(0 == strcmp("esc", a.escape_response));
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_app_set_receive_state_sets_value(uc_test_group *p) {
    char *state = "val";
    uc_cmd_line_app *ptr = uc_cmd_line_app_get_instance();
    void *prev_state = ptr->receive_state;

    uc_cmd_line_app_set_receive_state(ptr, state);
    UC_TEST_ASSERT(ptr->receive_state == state);

    ptr->receive_state = prev_state;
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_app_get_receive_state_gets_value(uc_test_group *p) {
    int state;
    uc_cmd_line_app *ptr = uc_cmd_line_app_get_instance();
    void *prev_state = ptr->receive_state;

    uc_cmd_line_app_set_receive_state(ptr, &state);
    UC_TEST_ASSERT(&state == uc_cmd_line_app_get_receive_state(ptr));

    ptr->receive_state = prev_state;
    return UC_TEST_ERR_NONE;
}

static uc_test_err uc_cmd_line_app_receive_uses_state(uc_test_group *p) {
    double state;
    uc_cmd_line_app *ptr = uc_cmd_line_app_get_instance();
    uc_cmd_line_app_receive_func *prev_func = ptr->receive;
    void *prev_state = ptr->receive_state;

    uc_cmd_line_app_set_receive(ptr, receive_1);
    uc_cmd_line_app_set_receive_state(ptr, &state);
    uc_cmd_line_app_receive(ptr);
    UC_TEST_ASSERT(&state == receive_1_state);

    ptr->receive = prev_func;
    ptr->receive_state = prev_state;
    return UC_TEST_ERR_NONE;
}

uc_test_group *uc_cmd_line_app_tests_get_group(void) {
    static uc_test_group group;
    static uc_test_group_test_func *tests[] = {
        uc_cmd_line_app_get_instance_is_not_null,
        uc_cmd_line_app_set_receive_sets_receive,
        uc_cmd_line_app_get_receive_returns_receive,
        uc_cmd_line_app_set_help_command_sets_value,
        uc_cmd_line_app_get_help_command_returns_value,
        uc_cmd_line_app_set_quit_command_sets_value,
        uc_cmd_line_app_get_quit_command_returns_value,
        uc_cmd_line_app_get_cmd_returns_value,
        uc_cmd_line_app_set_cmd_sets_value,
        uc_cmd_line_app_get_cmd_parser_is_not_null,
        uc_cmd_line_app_get_escape_response_returns_value,
        uc_cmd_line_app_set_escape_response_sets_value,
        uc_cmd_line_app_set_receive_state_sets_value,
        uc_cmd_line_app_get_receive_state_gets_value,
        uc_cmd_line_app_receive_uses_state,
        NULL
    };

    return uc_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
