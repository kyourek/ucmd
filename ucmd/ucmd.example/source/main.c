#include <stdio.h>
#include <string.h>
#include "uc_math_app.h"

static void send_data(const char *data) {
    printf("%s\n", data);
}

static char *receive_data(char *buf, size_t buf_size) {
    return fgets(buf, buf_size, stdin);
}

int main (int argc, const char *argv[]) {
    uc_app *app = uc_math_app_get_instance();
    uc_app_set_send_data(app, send_data);
    uc_app_set_receive_data(app, receive_data);
    uc_app_run(app);
    return 0;
}
