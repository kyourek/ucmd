#ifndef UC_SWITCH_OPT_P_H
#define UC_SWITCH_OPT_P_H

#include "ucArgOptOwner_p.h"
#include "uc_cmd_line.h"
#include "uc_switch_opt.h"

struct uc_switch_opt {
    ucArgOptOwner base;
    uc_switch_opt *next;
};

UC_EXPORTED uc_switch_opt *uc_switch_opt_init(uc_switch_opt *p, const char *name, const char *desc, uc_bool is_required, ucArgOpt *arg_opt, uc_switch_opt *next);

UC_EXPORTED const char *uc_switch_opt_format_validation_err(uc_switch_opt *p, uc_cmd_line *cmd, uc_switch_tok *switch_tok);

#endif
