#ifndef UC_APP_P_H
#define UC_APP_P_H

#include <stdlib.h>
#include "uc_app.h"
#include "uc_cmd_line_opt.h"

typedef uc_cmd_line_opt *(uc_app_create_cmd_opt_func)(uc_app *p);

UC_EXPORTED uc_app *uc_app_init(uc_app *p, uc_app_create_cmd_opt_func *create_cmd_opt);

UC_EXPORTED char *uc_app_receive_data(uc_app *p, char *buf, size_t buf_size);

UC_EXPORTED void uc_app_send_data(uc_app *p, const char *data);

struct uc_app {
    uc_app_create_cmd_opt_func *create_cmd_opt;
    uc_app_send_data_func *send_data;
    uc_app_receive_data_func *receive_data;
};

#endif
