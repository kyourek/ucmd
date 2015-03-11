#ifndef UCARGOPTOWNER_P_H
#define UCARGOPTOWNER_P_H

#include "ucArgOpt_p.h"
#include "ucArgOptOwner.h"

struct ucArgOptOwner {
    ucOpt base;
    ucArgOpt *arg_opt;
};

uc_EXPORTED ucArgOptOwner *ucArgOptOwner_init(ucArgOptOwner *p, const char *name, const char *desc, ucBool is_required, ucArgOpt *arg_opt);

uc_EXPORTED const char *ucArgOptOwner_format_validation_err(ucArgOptOwner *p, ucCmdLine *cmd, ucArgTok *arg_tok, const char *switch_name);

#endif
