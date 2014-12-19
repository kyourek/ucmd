#ifndef UCCMDLINE_P_H
#define UCCMDLINE_P_H

#include "ucCmdLine.h"

struct ucCmdLine {
    ucCmdTok *cmd_tok;
    ucCmdLine_transmit_func *transmit;
    ucCmdLine_is_cancelled_func *is_cancelled;
    void *transmit_state;
    void *is_cancelled_state;
    char response[ucCmdLine_RESPONSE_LENGTH];
	char response_buffer[ucCmdLine_RESPONSE_LENGTH];
    ucBool is_quiet;
};

#endif
