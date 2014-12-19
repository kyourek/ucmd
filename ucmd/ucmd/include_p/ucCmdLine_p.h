#ifndef UC_CMD_LINE_P_H
#define UC_CMD_LINE_P_H

#include "ucCmdLine.h"

struct ucCmdLine {
    ucCmdTok *cmd_tok;
    ucCmdLine_transmit_func *transmit;
    ucCmdLine_is_cancelled_func *is_cancelled;
    void *transmit_state;
    void *is_cancelled_state;
    char response[UC_CMD_LINE_RESPONSE_LENGTH];
	char response_buffer[UC_CMD_LINE_RESPONSE_LENGTH];
    ucBool is_quiet;
};

#endif
