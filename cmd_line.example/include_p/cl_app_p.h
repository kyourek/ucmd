#ifndef CL_APP_P_H
#define CL_APP_P_H

#include <stdlib.h>
#include "cl_app.h"
#include "cl_common.h"
#include "cl_cmd_line_opt.h"

typedef cl_cmd_line_opt *(cl_app_create_cmd_opt_func)(cl_app *p);

CL_EXPORTED cl_app *cl_app_init(cl_app *p, cl_app_create_cmd_opt_func *create_cmd_opt);

CL_EXPORTED char *cl_app_receive_data(cl_app *p, char *buf, size_t buf_size);

CL_EXPORTED void cl_app_send_data(cl_app *p, const char *data);

struct cl_app {
    cl_app_create_cmd_opt_func *create_cmd_opt;
    cl_app_send_data_func *send_data;
    cl_app_receive_data_func *receive_data;
};

#endif
