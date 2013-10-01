#ifndef CL_CMD_LINE_P_H
#define CL_CMD_LINE_P_H

#include "cl_cmd_line.h"

#ifndef CL_CMD_LINE_RESPONSE_LENGTH
#define CL_CMD_LINE_RESPONSE_LENGTH     500
#endif

struct cl_cmd_line {
    cl_cmd_tok *cmd_tok;
    cl_cmd_line_transmit_func *transmit;
    cl_cmd_line_is_cancelled_func *is_cancelled;
    void *transmit_state;
    void *is_cancelled_state;
    char response[CL_CMD_LINE_RESPONSE_LENGTH];
};

#endif
