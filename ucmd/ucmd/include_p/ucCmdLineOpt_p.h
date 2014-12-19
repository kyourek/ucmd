#ifndef UC_CMD_LINE_OPT_P_H
#define UC_CMD_LINE_OPT_P_H

#include "ucArgOptOwner_p.h"
#include "ucCmdLineOpt.h"
#include "ucSwitchOpt_p.h"

struct ucCmdLineOpt {
    ucArgOptOwner base;
    ucCmdLineOpt_func *func;
    void *state;
    ucSwitchOpt *switch_opt;
    ucCmdLineOpt *next;
};

UC_EXPORTED ucCmdLineOpt *ucCmdLineOpt_init(ucCmdLineOpt *p, ucCmdLineOpt_func *func, void* state, const char *name, const char *desc, ucArgOpt* arg_opt, ucSwitchOpt *switch_opt, ucCmdLineOpt *next);

UC_EXPORTED const char *ucCmdLineOpt_format_validation_err(ucCmdLineOpt *p, ucCmdLine *cmd);

#endif
