#ifndef UC_CMD_PARSER_P_H
#define UC_CMD_PARSER_P_H

#include "ucCmdParser.h"

struct ucCmdParser {
    ucCmdTok *(*parse)(ucCmdParser *p, char *cmd);
};

#endif
