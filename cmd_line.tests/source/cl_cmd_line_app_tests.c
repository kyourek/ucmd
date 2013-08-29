#include <string.h>
#include "cl_cmd_line.h"
#include "cl_cmd_line_p.h"
#include "cl_cmd_line_app.h"
#include "cl_cmd_line_app_p.h"
#include "cl_cmd_line_app_tests.h"
#include "cl_tests.h"

static char *receive_1(char *buf, int buf_size) { return 0; }

static char *receive_2(char *buf, int buf_size) { return 0; }

char *cl_cmd_line_app_get_instance_is_not_null(void) {
    cl_assert("cl_cmd_line_app_get_instance did not return instance.", NULL != cl_cmd_line_app_get_instance());
    return 0;
}

char *cl_cmd_line_app_set_receive_sets_receive(void) {
    cl_cmd_line_app p;
    cl_cmd_line_app_set_receive(&p, receive_1);
    cl_assert("cl_cmd_line_app_set_receive did not set function pointer.", receive_1 == cl_cmd_line_app_get_receive(&p));
    return 0;
}

char *cl_cmd_line_app_get_receive_returns_receive(void) {
    cl_cmd_line_app a;
    a.receive = receive_2;
    cl_assert("cl_cmd_line_app_get_receive did not return value.", receive_2 == cl_cmd_line_app_get_receive(&a));
    return 0;
}

char *cl_cmd_line_app_set_help_command_sets_value(void) {
    char *message = "cl_cmd_line_app_set_help_command did not set value.";
    cl_cmd_line_app p;
    cl_cmd_line_app_set_help_command(&p, "h");
    cl_assert(message, 0 == strcmp("h", cl_cmd_line_app_get_help_command(&p)));

    cl_cmd_line_app_set_help_command(&p, "helpme");
    cl_assert(message, 0 == strcmp("helpme", cl_cmd_line_app_get_help_command(&p)));
    
    return 0;
}

char *cl_cmd_line_app_get_help_command_returns_value(void) {
    cl_cmd_line_app a;
    a.help_command = "my_help_value";
    cl_assert("cl_cmd_line_app_get_help_command did not return value.", 0 == strcmp("my_help_value", cl_cmd_line_app_get_help_command(&a)));
    return 0;
}

char *cl_cmd_line_app_set_quit_command_sets_value(void) {
    char *message = "cl_cmd_line_app_set_quit_command did not set value.";
    cl_cmd_line_app p;
    cl_cmd_line_app_set_quit_command(&p, "q");
    cl_assert(message, 0 == strcmp("q", cl_cmd_line_app_get_quit_command(&p)));

    cl_cmd_line_app_set_quit_command(&p, "exit");
    cl_assert(message, 0 == strcmp("exit", cl_cmd_line_app_get_quit_command(&p)));
    
    return 0;
}

char *cl_cmd_line_app_get_quit_command_returns_value(void) {
    cl_cmd_line_app a;
    a.quit_command = "getout";
    cl_assert("cl_cmd_line_app_get_quit_command did not return value.", 0 == strcmp("getout", cl_cmd_line_app_get_quit_command(&a)));
    return 0;
}

char *cl_cmd_line_app_get_cmd_returns_value(void) {
    cl_cmd_line_app a;
    cl_cmd_line c;
    a.cmd = &c;
    cl_assert("cl_cmd_line_app_get_cmd did not return value.", &c == cl_cmd_line_app_get_cmd(&a));
    return 0;
}

char *cl_cmd_line_app_set_cmd_sets_value(void) {
    cl_cmd_line_app a;
    cl_cmd_line c;
    cl_cmd_line_app_set_cmd(&a, &c);
    cl_assert("cl_cmd_line_app_set_cmd did not set value.", &c == a.cmd);
    return 0;
}

char *cl_cmd_line_app_get_cmd_parser_is_not_null(void) {
    cl_cmd_line_app *a = cl_cmd_line_app_get_instance();
    cl_assert("cl_cmd_line_app_get_cmd_parser did not return instance.", NULL != cl_cmd_line_app_get_cmd_parser(a));
    return 0;    
}

char *cl_cmd_line_app_get_escape_response_returns_value(void) {
    cl_cmd_line_app a;
    a.escape_response = "escape";
    cl_assert("cl_cmd_line_app_get_escape_response did not return value.", 0 == strcmp("escape", cl_cmd_line_app_get_escape_response(&a)));
    return 0;
}

char *cl_cmd_line_app_set_escape_response_sets_value(void) {
    cl_cmd_line_app a;
    cl_cmd_line_app_set_escape_response(&a, "esc");
    cl_assert("cl_cmd_line_app_set_escape_response did not set value.", 0 == strcmp("esc", a.escape_response));
    return 0;
}

char *cl_cmd_line_app_tests(void) {
    cl_run_test(cl_cmd_line_app_get_instance_is_not_null);
    cl_run_test(cl_cmd_line_app_set_receive_sets_receive);
    cl_run_test(cl_cmd_line_app_get_receive_returns_receive);
    cl_run_test(cl_cmd_line_app_set_help_command_sets_value);
    cl_run_test(cl_cmd_line_app_get_help_command_returns_value);
    cl_run_test(cl_cmd_line_app_set_quit_command_sets_value);
    cl_run_test(cl_cmd_line_app_get_quit_command_returns_value);
    cl_run_test(cl_cmd_line_app_get_cmd_returns_value);
    cl_run_test(cl_cmd_line_app_set_cmd_sets_value);
    cl_run_test(cl_cmd_line_app_get_cmd_parser_is_not_null);
    cl_run_test(cl_cmd_line_app_get_escape_response_returns_value);
    cl_run_test(cl_cmd_line_app_set_escape_response_sets_value);
    return 0;
}
