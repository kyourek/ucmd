#include <stdlib.h>
#include "uc_app_p.h"
#include "uc_cmd_line.h"
#include "uc_cmd_line_app.h"

static char *receive(char *buf, size_t buf_size, void *state) {
    uc_app *app = (uc_app*)state;
    return uc_app_receive_data(app, buf, buf_size);
}

static void transmit(const char *data, void *state) {
    uc_app *app = (uc_app*)state;
    uc_app_send_data(app, data);
}

char *uc_app_receive_data(uc_app *p, char *buf, size_t buf_size) {
    if (NULL == p) return NULL;
    if (NULL == p->receive_data) return NULL;
    return p->receive_data(buf, buf_size);
}

void uc_app_send_data(uc_app *p, const char *data) {
    if (NULL == p) return;
    if (NULL == p->send_data) return;
    p->send_data(data);
}

uc_cmd_line_opt *uc_app_create_cmd_opt(uc_app *p) {
    if (NULL == p) return NULL;
    if (NULL == p->create_cmd_opt) return NULL;
    return p->create_cmd_opt(p);
}

void uc_app_set_send_data(uc_app *p, uc_app_send_data_func *value) {
    if (NULL == p) return;
    p->send_data = value;
}

void uc_app_set_receive_data(uc_app *p, uc_app_receive_data_func *value) {
    if (NULL == p) return;
    p->receive_data = value;
}

void uc_app_run(uc_app *p) {

    uc_cmd_line_opt *cmd_opt = uc_app_create_cmd_opt(p);
    uc_cmd_line_app *cmd_app = uc_cmd_line_app_get_instance();
    uc_cmd_line *cmd = uc_cmd_line_app_get_cmd(cmd_app);

    uc_cmd_line_set_transmit(cmd, transmit);
    uc_cmd_line_set_transmit_state(cmd, p);

    uc_cmd_line_app_set_receive(cmd_app, receive);
    uc_cmd_line_app_set_receive_state(cmd_app, p);

    uc_cmd_line_app_run(cmd_app, cmd_opt);

    uc_cmd_line_opt_destroy_chain(cmd_opt);
}

uc_app *uc_app_init(uc_app *p, uc_app_create_cmd_opt_func *create_cmd_opt) {
    if (NULL == p) return NULL;

    p->send_data = NULL;
    p->receive_data = NULL;
    p->create_cmd_opt = create_cmd_opt;

    return p;
}
