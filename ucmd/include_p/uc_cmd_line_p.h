#ifndef UC_CMD_LINE_P_H
#define UC_CMD_LINE_P_H

#include "uc_cmd_line.h"

#ifndef UC_CMD_LINE_RESPONSE_LENGTH
#define UC_CMD_LINE_RESPONSE_LENGTH     500
#endif

struct uc_cmd_line {
    uc_cmd_tok *cmd_tok;
    uc_cmd_line_transmit_func *transmit;
    uc_cmd_line_is_cancelled_func *is_cancelled;
    void *transmit_state;
    void *is_cancelled_state;
    char response[UC_CMD_LINE_RESPONSE_LENGTH];
    uc_bool is_quiet;
};

#endif
