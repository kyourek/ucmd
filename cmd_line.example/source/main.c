#include <stdio.h>
#include <string.h>
#include "cl_tok.h"
#include "cl_arg_tok.h"
#include "cl_cmd_tok.h"
#include "cl_switch_tok.h"
#include "cl_cmd_line.h"
#include "cl_cmd_line_app.h"
#include "cl_math_app.h"

static void run_math_app(cl_cmd_line_app *app) {
    cl_cmd_line_app_run(app, cl_math_app_create_cmd_opt());
}

int main (int argc, const char *argv[]) {
    cl_cmd_line_app *app = cl_cmd_line_app_get_instance();

    run_math_app(app);

    return 0;
}
