#ifndef UCCMDLINE_P_H
#define UCCMDLINE_P_H

#include "ucCmdLine.h"

struct ucCmdLine {
    ucCmdTok *cmd_tok;
    ucCmdLine_TransmitFunc *transmit;
    ucCmdLine_IsCancelledFunc *is_cancelled;
    void *transmit_state;
    void *is_cancelled_state;
    char response[ucCmdLine_RESPONSE_LENGTH];
	char response_buffer[ucCmdLine_RESPONSE_LENGTH];
    ucBool is_quiet;
};

#endif
