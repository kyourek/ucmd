#ifndef CL_APP_H
#define CL_APP_H

#include <stdlib.h>
#include "cl_common.h"

typedef struct cl_app cl_app;

typedef void (cl_app_send_data_func)(const char *data);

typedef char *(cl_app_receive_data_func)(char *buf, size_t buf_size);

CL_EXPORTED void cl_app_set_send_data(cl_app *p, cl_app_send_data_func *value);

CL_EXPORTED void cl_app_set_receive_data(cl_app *p, cl_app_receive_data_func *value);

CL_EXPORTED void cl_app_run(cl_app *p);

#endif
