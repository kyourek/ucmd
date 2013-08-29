#ifndef CL_CMD_LINE_VALIDATOR_P_H
#define CL_CMD_LINE_VALIDATOR_P_H

#include "cl_cmd_line.h"
#include "cl_cmd_line_opt.h"
#include "cl_cmd_line_validator.h"

struct cl_cmd_line_validator {
    const char *(*validate)(cl_cmd_line_validator *p, cl_cmd_line *cmd, cl_cmd_line_opt *cmd_opt);
};

#endif
