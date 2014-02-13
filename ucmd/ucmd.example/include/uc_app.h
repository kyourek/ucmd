#ifndef UC_APP_H
#define UC_APP_H

#include <stdlib.h>
#include "uc_common.h"

typedef struct uc_app uc_app;

typedef void (uc_app_send_data_func)(const char *data);

typedef char *(uc_app_receive_data_func)(char *buf, size_t buf_size);

UC_EXPORTED void uc_app_set_send_data(uc_app *p, uc_app_send_data_func *value);

UC_EXPORTED void uc_app_set_receive_data(uc_app *p, uc_app_receive_data_func *value);

UC_EXPORTED void uc_app_run(uc_app *p);

#endif
