#ifndef UCSWITCHOPT_P_H
#define UCSWITCHOPT_P_H

#include "ucArgOptOwner_p.h"
#include "ucCmdLine.h"
#include "ucSwitchOpt.h"

struct ucSwitchOpt {
    ucArgOptOwner base;
    ucSwitchOpt *next;
};

UC_EXPORTED ucSwitchOpt *ucSwitchOpt_init(ucSwitchOpt *p, const char *name, const char *desc, ucBool is_required, ucArgOpt *arg_opt, ucSwitchOpt *next);

UC_EXPORTED const char *ucSwitchOpt_format_validation_err(ucSwitchOpt *p, ucCmdLine *cmd, ucSwitchTok *switch_tok);

#endif
