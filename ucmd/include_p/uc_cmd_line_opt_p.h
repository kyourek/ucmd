#ifndef UC_CMD_LINE_OPT_P_H
#define UC_CMD_LINE_OPT_P_H

#include "uc_arg_opt_owner_p.h"
#include "uc_cmd_line_opt.h"
#include "uc_switch_opt_p.h"

struct uc_cmd_line_opt {
    uc_arg_opt_owner base;
    uc_cmd_line_opt_func *func;
    void *state;
    uc_switch_opt *switch_opt;
    uc_cmd_line_opt *next;
};

UC_EXPORTED uc_cmd_line_opt *uc_cmd_line_opt_init(uc_cmd_line_opt *p, uc_cmd_line_opt_func *func, void* state, const char *name, const char *desc, uc_arg_opt* arg_opt, uc_switch_opt *switch_opt, uc_cmd_line_opt *next);

UC_EXPORTED const char *uc_cmd_line_opt_format_validation_err(uc_cmd_line_opt *p, uc_cmd_line *cmd);

#endif
