#ifndef UC_ARG_OPT_OWNER_P_H
#define UC_ARG_OPT_OWNER_P_H

#include "uc_arg_opt_p.h"
#include "uc_arg_opt_owner.h"

struct uc_arg_opt_owner {
    uc_opt base;
    uc_arg_opt *arg_opt;
};

UC_EXPORTED uc_arg_opt_owner *uc_arg_opt_owner_init(uc_arg_opt_owner *p, const char *name, const char *desc, uc_bool is_required, uc_arg_opt *arg_opt);

UC_EXPORTED const char *uc_arg_opt_owner_format_validation_err(uc_arg_opt_owner *p, uc_cmd_line *cmd, uc_arg_tok *arg_tok, const char *switch_name);

#endif
