#ifndef UCCMDLINEAPP_P_H
#define UCCMDLINEAPP_P_H

#include "ucCmdParser_p.h"
#include "ucCmdLineApp.h"

struct ucCmdLineApp {
    ucCmdLine *cmd;
    ucCmdParser *cmd_parser;
    ucCmdLineApp_receive_func *receive;
    void *receive_state;
    ucErr (*run)(ucCmdLineApp *p, ucCmdLineOpt *cmd_opt);
    const char *help_command;
    const char *quit_command;
    const char *escape_response;
    char cmd_buf[ucCmdLineApp_CMD_BUF_LENGTH + 1];
};

uc_EXPORTED char *ucCmdLineApp_receive(ucCmdLineApp *p);

#endif
