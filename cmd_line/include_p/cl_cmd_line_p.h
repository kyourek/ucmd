#ifndef CL_CMD_LINE_P_H
#define CL_CMD_LINE_P_H

#include "cl_cmd_tok.h"
#include "cl_common.h"
#include "cl_cmd_line.h"

struct cl_cmd_line {
    cl_cmd_tok *cmd_tok;
    void (*transmit)(const char *response);
    CL_BOOL (*is_cancelled)(void);
    char response[CL_RESPONSE_LEN_MAX];
};

#endif
