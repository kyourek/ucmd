#ifndef UCCMDLINEOPT_P_H
#define UCCMDLINEOPT_P_H

#include "ucArgOptOwner_p.h"
#include "ucCmdLineOpt.h"
#include "ucSwitchOpt_p.h"

struct ucCmdLineOpt {
    ucArgOptOwner base;
    ucCmdLineOpt_Func *func;
    void *state;
    ucSwitchOpt *switch_opt;
    ucCmdLineOpt *next;
};

uc_EXPORTED ucCmdLineOpt *ucCmdLineOpt_init(ucCmdLineOpt *p, ucCmdLineOpt_Func *func, void* state, const char *name, const char *desc, ucArgOpt* arg_opt, ucSwitchOpt *switch_opt, ucCmdLineOpt *next);

uc_EXPORTED const char *ucCmdLineOpt_format_validation_err(ucCmdLineOpt *p, ucCmdLine *cmd);

#endif
