#ifndef UC_ARG_OPT_P_H
#define UC_ARG_OPT_P_H

#include "uc_arg_opt.h"
#include "uc_cmd_line.h"
#include "uc_opt_p.h"

struct uc_arg_opt {
    uc_opt base;

    uc_bool is_numeric;
    UC_ARG_OPT_NUMERIC_TYPE numeric_min;
    UC_ARG_OPT_NUMERIC_TYPE numeric_max;

    int max_tok_count;
    int min_tok_count;

    uc_arg_opt *next;
};

UC_EXPORTED uc_arg_opt *uc_arg_opt_init(uc_arg_opt *p, const char *name, const char *desc, uc_bool is_required, int min_tok_count, int max_tok_count, uc_bool is_numeric, UC_ARG_OPT_NUMERIC_TYPE numeric_min, UC_ARG_OPT_NUMERIC_TYPE numeric_max, uc_arg_opt *next);

UC_EXPORTED const char *uc_arg_opt_format_validation_err(uc_arg_opt *p, uc_cmd_line *cmd, uc_arg_tok *arg_tok, const char *switch_name);

#endif
