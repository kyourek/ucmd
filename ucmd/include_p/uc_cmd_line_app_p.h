#ifndef UC_CMD_LINE_APP_P_H
#define UC_CMD_LINE_APP_P_H

#include "uc_cmd_parser_p.h"
#include "uc_cmd_line_app.h"

#ifndef UC_CMD_LINE_APP_CMD_BUF_LENGTH
#define UC_CMD_LINE_APP_CMD_BUF_LENGTH      200
#endif

struct uc_cmd_line_app {
    uc_cmd_line *cmd;
    uc_cmd_parser *cmd_parser;
    uc_cmd_line_app_receive_func *receive;
    void *receive_state;
    uc_err (*run)(uc_cmd_line_app *p, uc_cmd_line_opt *cmd_opt);
    const char *help_command;
    const char *quit_command;
    const char *escape_response;
    char cmd_buf[UC_CMD_LINE_APP_CMD_BUF_LENGTH + 1];
};

UC_EXPORTED char *uc_cmd_line_app_receive(uc_cmd_line_app *p);

#endif
