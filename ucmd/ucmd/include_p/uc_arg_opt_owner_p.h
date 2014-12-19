#ifndef UC_ARG_OPT_OWNER_P_H
#define UC_ARG_OPT_OWNER_P_H

#include "ucArgOpt_p.h"
#include "ucArgOpt_owner.h"

struct ucArgOpt_owner {
    uc_opt base;
    ucArgOpt *arg_opt;
};

UC_EXPORTED ucArgOpt_owner *ucArgOpt_owner_init(ucArgOpt_owner *p, const char *name, const char *desc, uc_bool is_required, ucArgOpt *arg_opt);

UC_EXPORTED const char *ucArgOpt_owner_format_validation_err(ucArgOpt_owner *p, uc_cmd_line *cmd, uc_arg_tok *arg_tok, const char *switch_name);

#endif
