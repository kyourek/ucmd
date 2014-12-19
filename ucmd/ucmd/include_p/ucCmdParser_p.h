#ifndef UCCMDPARSER_P_H
#define UCCMDPARSER_P_H

#include "ucCmdParser.h"

struct ucCmdParser {
    ucCmdTok *(*parse)(ucCmdParser *p, char *cmd);
};

#endif
