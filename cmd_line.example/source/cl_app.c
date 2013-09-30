#include <stdlib.h>
#include "cl_app_p.h"
#include "cl_cmd_line.h"
#include "cl_cmd_line_app.h"

static char *receive(char *buf, size_t buf_size, void *state) {
    cl_app *app = (cl_app*)state;
    return cl_app_receive_data(app, buf, buf_size);
}

static void transmit(const char *data, void *state) {
    cl_app *app = (cl_app*)state;
    cl_app_send_data(app, data);
}

char *cl_app_receive_data(cl_app *p, char *buf, size_t buf_size) {
    if (NULL == p) return NULL;
    if (NULL == p->receive_data) return NULL;
    return p->receive_data(buf, buf_size);
}

void cl_app_send_data(cl_app *p, const char *data) {
    if (NULL == p) return;
    if (NULL == p->send_data) return;
    p->send_data(data);
}

cl_cmd_line_opt *cl_app_create_cmd_opt(cl_app *p) {
    if (NULL == p) return NULL;
    if (NULL == p->create_cmd_opt) return NULL;
    return p->create_cmd_opt(p);
}

void cl_app_set_send_data(cl_app *p, cl_app_send_data_func *value) {
    if (NULL == p) return;
    p->send_data = value;
}

void cl_app_set_receive_data(cl_app *p, cl_app_receive_data_func *value) {
    if (NULL == p) return;
    p->receive_data = value;
}

void cl_app_run(cl_app *p) {

    cl_cmd_line_opt *cmd_opt = cl_app_create_cmd_opt(p);
    cl_cmd_line_app *cmd_app = cl_cmd_line_app_get_instance();
    cl_cmd_line *cmd = cl_cmd_line_app_get_cmd(cmd_app);

    cl_cmd_line_set_transmit(cmd, transmit);
    cl_cmd_line_set_transmit_state(cmd, p);

    cl_cmd_line_app_set_receive(cmd_app, receive);
    cl_cmd_line_app_set_receive_state(cmd_app, p);

    cl_cmd_line_app_run(cmd_app, cmd_opt);

    cl_cmd_line_opt_destroy_chain(cmd_opt);
}

cl_app *cl_app_init(cl_app *p, cl_app_create_cmd_opt_func *create_cmd_opt) {
    if (NULL == p) return NULL;

    p->send_data = NULL;
    p->receive_data = NULL;
    p->create_cmd_opt = create_cmd_opt;

    return p;
}
