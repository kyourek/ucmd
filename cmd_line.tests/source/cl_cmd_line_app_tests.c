#include <string.h>
#include "cl_cmd_line_p.h"
#include "cl_cmd_line_app_p.h"
#include "cl_cmd_line_app_tests.h"
#include "cl_tests_p.h"

void *receive_1_state;
static char *receive_1(char *buf, size_t buf_size, void *state) { 
    receive_1_state = state;
    return 0; 
}

static char *receive_2(char *buf, size_t buf_size, void *state) { return 0; }

static cl_tests_err cl_cmd_line_app_get_instance_is_not_null(void) {
    CL_TESTS_ASSERT(NULL != cl_cmd_line_app_get_instance());
    return CL_TESTS_ERR_NONE;
}

static cl_tests_err cl_cmd_line_app_set_receive_sets_receive(void) {
    cl_cmd_line_app p;
    cl_cmd_line_app_set_receive(&p, receive_1);
    CL_TESTS_ASSERT(receive_1 == cl_cmd_line_app_get_receive(&p));
    return CL_TESTS_ERR_NONE;
}

static cl_tests_err cl_cmd_line_app_get_receive_returns_receive(void) {
    cl_cmd_line_app a;
    a.receive = receive_2;
    CL_TESTS_ASSERT(receive_2 == cl_cmd_line_app_get_receive(&a));
    return CL_TESTS_ERR_NONE;
}

static cl_tests_err cl_cmd_line_app_set_help_command_sets_value(void) {
    cl_cmd_line_app p;
    cl_cmd_line_app_set_help_command(&p, "h");
    CL_TESTS_ASSERT(0 == strcmp("h", cl_cmd_line_app_get_help_command(&p)));

    cl_cmd_line_app_set_help_command(&p, "helpme");
    CL_TESTS_ASSERT(0 == strcmp("helpme", cl_cmd_line_app_get_help_command(&p)));
    
    return CL_TESTS_ERR_NONE;
}

static cl_tests_err cl_cmd_line_app_get_help_command_returns_value(void) {
    cl_cmd_line_app a;
    a.help_command = "my_help_value";
    CL_TESTS_ASSERT(cl_cmd_line_app_get_help_command(&a));
    return CL_TESTS_ERR_NONE;
}

static cl_tests_err cl_cmd_line_app_set_quit_command_sets_value(void) {
    cl_cmd_line_app p;
    cl_cmd_line_app_set_quit_command(&p, "q");
    CL_TESTS_ASSERT(0 == strcmp("q", cl_cmd_line_app_get_quit_command(&p)));

    cl_cmd_line_app_set_quit_command(&p, "exit");
    CL_TESTS_ASSERT(0 == strcmp("exit", cl_cmd_line_app_get_quit_command(&p)));
    
    return CL_TESTS_ERR_NONE;
}

static cl_tests_err cl_cmd_line_app_get_quit_command_returns_value(void) {
    cl_cmd_line_app a;
    a.quit_command = "getout";
    CL_TESTS_ASSERT(cl_cmd_line_app_get_quit_command(&a));
    return CL_TESTS_ERR_NONE;
}

static cl_tests_err cl_cmd_line_app_get_cmd_returns_value(void) {
    cl_cmd_line_app a;
    cl_cmd_line c;
    a.cmd = &c;
    CL_TESTS_ASSERT(&c == cl_cmd_line_app_get_cmd(&a));
    return CL_TESTS_ERR_NONE;
}

static cl_tests_err cl_cmd_line_app_set_cmd_sets_value(void) {
    cl_cmd_line_app a;
    cl_cmd_line c;
    cl_cmd_line_app_set_cmd(&a, &c);
    CL_TESTS_ASSERT(&c == a.cmd);
    return CL_TESTS_ERR_NONE;
}

static cl_tests_err cl_cmd_line_app_get_cmd_parser_is_not_null(void) {
    cl_cmd_line_app *a = cl_cmd_line_app_get_instance();
    CL_TESTS_ASSERT(NULL != cl_cmd_line_app_get_cmd_parser(a));
    return CL_TESTS_ERR_NONE;    
}

static cl_tests_err cl_cmd_line_app_get_escape_response_returns_value(void) {
    cl_cmd_line_app a;
    a.escape_response = "escape";
    CL_TESTS_ASSERT(0 == strcmp("escape", cl_cmd_line_app_get_escape_response(&a)));
    return CL_TESTS_ERR_NONE;
}

static cl_tests_err cl_cmd_line_app_set_escape_response_sets_value(void) {
    cl_cmd_line_app a;
    cl_cmd_line_app_set_escape_response(&a, "esc");
    CL_TESTS_ASSERT(0 == strcmp("esc", a.escape_response));
    return CL_TESTS_ERR_NONE;
}

static cl_tests_err cl_cmd_line_app_set_receive_state_sets_value(void) {
    char *state = "val";
    cl_cmd_line_app *p = cl_cmd_line_app_get_instance();
    void *prev_state = p->receive_state;

    cl_cmd_line_app_set_receive_state(p, state);
    CL_TESTS_ASSERT(p->receive_state == state);

    p->receive_state = prev_state;
    return CL_TESTS_ERR_NONE;
}

static cl_tests_err cl_cmd_line_app_get_receive_state_gets_value(void) {
    int state;
    cl_cmd_line_app *p = cl_cmd_line_app_get_instance();
    void *prev_state = p->receive_state;

    cl_cmd_line_app_set_receive_state(p, &state);
    CL_TESTS_ASSERT(&state == cl_cmd_line_app_get_receive_state(p));

    p->receive_state = prev_state;
    return CL_TESTS_ERR_NONE;
}

static cl_tests_err cl_cmd_line_app_receive_uses_state(void) {
    double state;
    cl_cmd_line_app *p = cl_cmd_line_app_get_instance();
    cl_cmd_line_app_receive_func *prev_func = p->receive;
    void *prev_state = p->receive_state;

    cl_cmd_line_app_set_receive(p, receive_1);
    cl_cmd_line_app_set_receive_state(p, &state);
    cl_cmd_line_app_receive(p);
    CL_TESTS_ASSERT(&state == receive_1_state);

    p->receive = prev_func;
    p->receive_state = prev_state;
    return CL_TESTS_ERR_NONE;
}

cl_tests_err cl_cmd_line_app_tests(void) {
    CL_TESTS_RUN(cl_cmd_line_app_get_instance_is_not_null);
    CL_TESTS_RUN(cl_cmd_line_app_set_receive_sets_receive);
    CL_TESTS_RUN(cl_cmd_line_app_get_receive_returns_receive);
    CL_TESTS_RUN(cl_cmd_line_app_set_help_command_sets_value);
    CL_TESTS_RUN(cl_cmd_line_app_get_help_command_returns_value);
    CL_TESTS_RUN(cl_cmd_line_app_set_quit_command_sets_value);
    CL_TESTS_RUN(cl_cmd_line_app_get_quit_command_returns_value);
    CL_TESTS_RUN(cl_cmd_line_app_get_cmd_returns_value);
    CL_TESTS_RUN(cl_cmd_line_app_set_cmd_sets_value);
    CL_TESTS_RUN(cl_cmd_line_app_get_cmd_parser_is_not_null);
    CL_TESTS_RUN(cl_cmd_line_app_get_escape_response_returns_value);
    CL_TESTS_RUN(cl_cmd_line_app_set_escape_response_sets_value);
    CL_TESTS_RUN(cl_cmd_line_app_set_receive_state_sets_value);
    CL_TESTS_RUN(cl_cmd_line_app_get_receive_state_gets_value);
    CL_TESTS_RUN(cl_cmd_line_app_receive_uses_state);
    return CL_TESTS_ERR_NONE;
}
