#include <string.h>
#include "cl_cmd_line_p.h"
#include "cl_cmd_line_app_p.h"
#include "cl_cmd_line_app_tests.h"
#include "cl_test.h"

void *receive_1_state;
static char *receive_1(char *buf, size_t buf_size, void *state) { 
    receive_1_state = state;
    return 0; 
}

static char *receive_2(char *buf, size_t buf_size, void *state) { return 0; }

static cl_test_err cl_cmd_line_app_get_instance_is_not_null(cl_test_group *p) {
    CL_TEST_ASSERT(NULL != cl_cmd_line_app_get_instance());
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_app_set_receive_sets_receive(cl_test_group *p) {
    cl_cmd_line_app ptr;
    cl_cmd_line_app_set_receive(&ptr, receive_1);
    CL_TEST_ASSERT(receive_1 == cl_cmd_line_app_get_receive(&ptr));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_app_get_receive_returns_receive(cl_test_group *p) {
    cl_cmd_line_app a;
    a.receive = receive_2;
    CL_TEST_ASSERT(receive_2 == cl_cmd_line_app_get_receive(&a));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_app_set_help_command_sets_value(cl_test_group *p) {
    cl_cmd_line_app ptr;
    cl_cmd_line_app_set_help_command(&ptr, "h");
    CL_TEST_ASSERT(0 == strcmp("h", cl_cmd_line_app_get_help_command(&ptr)));

    cl_cmd_line_app_set_help_command(&ptr, "helpme");
    CL_TEST_ASSERT(0 == strcmp("helpme", cl_cmd_line_app_get_help_command(&ptr)));
    
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_app_get_help_command_returns_value(cl_test_group *p) {
    cl_cmd_line_app a;
    a.help_command = "my_help_value";
    CL_TEST_ASSERT(cl_cmd_line_app_get_help_command(&a));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_app_set_quit_command_sets_value(cl_test_group *p) {
    cl_cmd_line_app ptr;
    cl_cmd_line_app_set_quit_command(&ptr, "q");
    CL_TEST_ASSERT(0 == strcmp("q", cl_cmd_line_app_get_quit_command(&ptr)));

    cl_cmd_line_app_set_quit_command(&ptr, "exit");
    CL_TEST_ASSERT(0 == strcmp("exit", cl_cmd_line_app_get_quit_command(&ptr)));
    
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_app_get_quit_command_returns_value(cl_test_group *p) {
    cl_cmd_line_app a;
    a.quit_command = "getout";
    CL_TEST_ASSERT(cl_cmd_line_app_get_quit_command(&a));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_app_get_cmd_returns_value(cl_test_group *p) {
    cl_cmd_line_app a;
    cl_cmd_line c;
    a.cmd = &c;
    CL_TEST_ASSERT(&c == cl_cmd_line_app_get_cmd(&a));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_app_set_cmd_sets_value(cl_test_group *p) {
    cl_cmd_line_app a;
    cl_cmd_line c;
    cl_cmd_line_app_set_cmd(&a, &c);
    CL_TEST_ASSERT(&c == a.cmd);
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_app_get_cmd_parser_is_not_null(cl_test_group *p) {
    cl_cmd_line_app *a = cl_cmd_line_app_get_instance();
    CL_TEST_ASSERT(NULL != cl_cmd_line_app_get_cmd_parser(a));
    return CL_TEST_ERR_NONE;    
}

static cl_test_err cl_cmd_line_app_get_escape_response_returns_value(cl_test_group *p) {
    cl_cmd_line_app a;
    a.escape_response = "escape";
    CL_TEST_ASSERT(0 == strcmp("escape", cl_cmd_line_app_get_escape_response(&a)));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_app_set_escape_response_sets_value(cl_test_group *p) {
    cl_cmd_line_app a;
    cl_cmd_line_app_set_escape_response(&a, "esc");
    CL_TEST_ASSERT(0 == strcmp("esc", a.escape_response));
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_app_set_receive_state_sets_value(cl_test_group *p) {
    char *state = "val";
    cl_cmd_line_app *ptr = cl_cmd_line_app_get_instance();
    void *prev_state = ptr->receive_state;

    cl_cmd_line_app_set_receive_state(ptr, state);
    CL_TEST_ASSERT(ptr->receive_state == state);

    ptr->receive_state = prev_state;
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_app_get_receive_state_gets_value(cl_test_group *p) {
    int state;
    cl_cmd_line_app *ptr = cl_cmd_line_app_get_instance();
    void *prev_state = ptr->receive_state;

    cl_cmd_line_app_set_receive_state(ptr, &state);
    CL_TEST_ASSERT(&state == cl_cmd_line_app_get_receive_state(ptr));

    ptr->receive_state = prev_state;
    return CL_TEST_ERR_NONE;
}

static cl_test_err cl_cmd_line_app_receive_uses_state(cl_test_group *p) {
    double state;
    cl_cmd_line_app *ptr = cl_cmd_line_app_get_instance();
    cl_cmd_line_app_receive_func *prev_func = ptr->receive;
    void *prev_state = ptr->receive_state;

    cl_cmd_line_app_set_receive(ptr, receive_1);
    cl_cmd_line_app_set_receive_state(ptr, &state);
    cl_cmd_line_app_receive(ptr);
    CL_TEST_ASSERT(&state == receive_1_state);

    ptr->receive = prev_func;
    ptr->receive_state = prev_state;
    return CL_TEST_ERR_NONE;
}

cl_test_group *cl_cmd_line_app_tests_get_group(void) {
    static cl_test_group group;
    static cl_test_group_test_func *tests[] = {
        cl_cmd_line_app_get_instance_is_not_null,
        cl_cmd_line_app_set_receive_sets_receive,
        cl_cmd_line_app_get_receive_returns_receive,
        cl_cmd_line_app_set_help_command_sets_value,
        cl_cmd_line_app_get_help_command_returns_value,
        cl_cmd_line_app_set_quit_command_sets_value,
        cl_cmd_line_app_get_quit_command_returns_value,
        cl_cmd_line_app_get_cmd_returns_value,
        cl_cmd_line_app_set_cmd_sets_value,
        cl_cmd_line_app_get_cmd_parser_is_not_null,
        cl_cmd_line_app_get_escape_response_returns_value,
        cl_cmd_line_app_set_escape_response_sets_value,
        cl_cmd_line_app_set_receive_state_sets_value,
        cl_cmd_line_app_get_receive_state_gets_value,
        cl_cmd_line_app_receive_uses_state,
        NULL
    };

    return cl_test_group_init(&group, NULL, NULL, NULL, NULL, tests);
}
