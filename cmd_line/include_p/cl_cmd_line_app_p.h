#ifndef CL_CMD_LINE_APP_P_H
#define CL_CMD_LINE_APP_P_H

#include "cl_common.h"
#include "cl_cmd_parser.h"
#include "cl_cmd_parser_p.h"
#include "cl_cmd_line_app.h"
#include "cl_cmd_line_opt.h"

struct cl_cmd_line_app {
    cl_cmd_line *cmd;
    cl_cmd_parser *cmd_parser;
    cl_cmd_line_app_receive_func *receive;
    void *receive_state;
    CL_ERR (*run)(cl_cmd_line_app *p, cl_cmd_line_opt *cmd_opt);
    const char *help_command;
    const char *quit_command;
    const char *escape_response;
    char cmd_buf[CL_CMD_LEN_MAX + 1];
};

CL_EXPORTED char *cl_cmd_line_app_receive(cl_cmd_line_app *p);

#endif
