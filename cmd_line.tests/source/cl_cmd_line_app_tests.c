#include <string.h>
#include "cl_cmd_line.h"
#include "cl_cmd_line_p.h"
#include "cl_cmd_line_app.h"
#include "cl_cmd_line_app_p.h"
#include "cl_cmd_line_app_tests.h"
#include "cl_tests.h"
#include "cl_tests_p.h"

static char *receive_1(char *buf, int buf_size) { return CL_TESTS_NO_ERR; }

static char *receive_2(char *buf, int buf_size) { return CL_TESTS_NO_ERR; }

static CL_BOOL ready_1_returned_value;
static CL_BOOL ready_1(void) { return ready_1_returned_value; }

static CL_TESTS_ERR cl_cmd_line_app_get_instance_is_not_null(void) {
    CL_TESTS_ASSERT(NULL != cl_cmd_line_app_get_instance());
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_app_set_receive_sets_receive(void) {
    cl_cmd_line_app p;
    cl_cmd_line_app_set_receive(&p, receive_1);
    CL_TESTS_ASSERT(receive_1 == cl_cmd_line_app_get_receive(&p));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_app_get_receive_returns_receive(void) {
    cl_cmd_line_app a;
    a.receive = receive_2;
    CL_TESTS_ASSERT(receive_2 == cl_cmd_line_app_get_receive(&a));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_app_set_help_command_sets_value(void) {
    cl_cmd_line_app p;
    cl_cmd_line_app_set_help_command(&p, "h");
    CL_TESTS_ASSERT(0 == strcmp("h", cl_cmd_line_app_get_help_command(&p)));

    cl_cmd_line_app_set_help_command(&p, "helpme");
    CL_TESTS_ASSERT(0 == strcmp("helpme", cl_cmd_line_app_get_help_command(&p)));
    
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_app_get_help_command_returns_value(void) {
    cl_cmd_line_app a;
    a.help_command = "my_help_value";
    CL_TESTS_ASSERT(cl_cmd_line_app_get_help_command(&a));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_app_set_quit_command_sets_value(void) {
    cl_cmd_line_app p;
    cl_cmd_line_app_set_quit_command(&p, "q");
    CL_TESTS_ASSERT(0 == strcmp("q", cl_cmd_line_app_get_quit_command(&p)));

    cl_cmd_line_app_set_quit_command(&p, "exit");
    CL_TESTS_ASSERT(0 == strcmp("exit", cl_cmd_line_app_get_quit_command(&p)));
    
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_app_get_quit_command_returns_value(void) {
    cl_cmd_line_app a;
    a.quit_command = "getout";
    CL_TESTS_ASSERT(cl_cmd_line_app_get_quit_command(&a));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_app_get_cmd_returns_value(void) {
    cl_cmd_line_app a;
    cl_cmd_line c;
    a.cmd = &c;
    CL_TESTS_ASSERT(&c == cl_cmd_line_app_get_cmd(&a));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_app_set_cmd_sets_value(void) {
    cl_cmd_line_app a;
    cl_cmd_line c;
    cl_cmd_line_app_set_cmd(&a, &c);
    CL_TESTS_ASSERT(&c == a.cmd);
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_app_get_cmd_parser_is_not_null(void) {
    cl_cmd_line_app *a = cl_cmd_line_app_get_instance();
    CL_TESTS_ASSERT(NULL != cl_cmd_line_app_get_cmd_parser(a));
    return CL_TESTS_NO_ERR;    
}

static CL_TESTS_ERR cl_cmd_line_app_get_escape_response_returns_value(void) {
    cl_cmd_line_app a;
    a.escape_response = "escape";
    CL_TESTS_ASSERT(cl_cmd_line_app_get_escape_response(&a));
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_app_set_escape_response_sets_value(void) {
    cl_cmd_line_app a;
    cl_cmd_line_app_set_escape_response(&a, "esc");
    CL_TESTS_ASSERT(a.escape_response);
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_app_set_ready_sets_value(void) {
    cl_cmd_line_app *p = cl_cmd_line_app_get_instance();
    
    cl_cmd_line_app_ready_func *prev_value = p->ready;
    CL_TESTS_ASSERT(prev_value != ready_1);

    cl_cmd_line_app_set_ready(p, ready_1);
    CL_TESTS_ASSERT(p->ready == ready_1);

    p->ready = prev_value;
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_app_get_ready_sets_value(void) {
    cl_cmd_line_app *p = cl_cmd_line_app_get_instance();

    cl_cmd_line_app_ready_func *prev_value = p->ready;
    CL_TESTS_ASSERT(prev_value != ready_1);

    p->ready = ready_1;
    CL_TESTS_ASSERT(cl_cmd_line_app_get_ready(p) == ready_1);

    p->ready = prev_value;
    return CL_TESTS_NO_ERR;
}

static CL_TESTS_ERR cl_cmd_line_app_ready_uses_ready(void) {
    cl_cmd_line_app *p = cl_cmd_line_app_get_instance();

    cl_cmd_line_app_ready_func *prev_value = p->ready;

    p->ready = ready_1;
    
    ready_1_returned_value = CL_FALSE;
    CL_TESTS_ASSERT(cl_cmd_line_app_ready(p) == CL_FALSE);

    ready_1_returned_value = CL_TRUE;
    CL_TESTS_ASSERT(cl_cmd_line_app_ready(p) == CL_TRUE);

    p->ready = prev_value;
    return CL_TESTS_NO_ERR;
}

CL_TESTS_ERR cl_cmd_line_app_tests(void) {
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
    CL_TESTS_RUN(cl_cmd_line_app_set_ready_sets_value);
    CL_TESTS_RUN(cl_cmd_line_app_get_ready_sets_value);
    CL_TESTS_RUN(cl_cmd_line_app_ready_uses_ready);
    return CL_TESTS_NO_ERR;
}
