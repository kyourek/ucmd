#ifndef UCARGOPTOWNER_P_H
#define UCARGOPTOWNER_P_H

#include "ucArgOpt_p.h"
#include "ucArgOptOwner.h"

struct ucArgOptOwner {
    uc_opt base;
    ucArgOpt *arg_opt;
};

UC_EXPORTED ucArgOptOwner *ucArgOptOwner_init(ucArgOptOwner *p, const char *name, const char *desc, uc_bool is_required, ucArgOpt *arg_opt);

UC_EXPORTED const char *ucArgOptOwner_format_validation_err(ucArgOptOwner *p, uc_cmd_line *cmd, ucArgTok *arg_tok, const char *switch_name);

#endif
