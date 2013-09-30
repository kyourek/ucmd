#ifndef CL_CMD_PARSER_P_H
#define CL_CMD_PARSER_P_H

#include "cl_cmd_parser.h"

struct cl_cmd_parser {
    cl_cmd_tok *(*parse)(cl_cmd_parser *p, char *cmd);
};

#endif
