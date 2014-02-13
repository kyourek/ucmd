#ifndef UC_CMD_PARSER_P_H
#define UC_CMD_PARSER_P_H

#include "uc_cmd_parser.h"

struct uc_cmd_parser {
    uc_cmd_tok *(*parse)(uc_cmd_parser *p, char *cmd);
};

#endif
