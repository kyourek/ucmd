#ifndef UCARGOPT_P_H
#define UCARGOPT_P_H

#include "ucArgOpt.h"
#include "uc_cmd_line.h"
#include "uc_opt_p.h"

struct ucArgOpt {
    uc_opt base;

    uc_bool is_numeric;
    UC_ARG_OPT_NUMERIC_TYPE numeric_min;
    UC_ARG_OPT_NUMERIC_TYPE numeric_max;

    int max_tok_count;
    int min_tok_count;

    ucArgOpt *next;
};

UC_EXPORTED ucArgOpt *ucArgOpt_init(ucArgOpt *p, const char *name, const char *desc, uc_bool is_required, int min_tok_count, int max_tok_count, uc_bool is_numeric, UC_ARG_OPT_NUMERIC_TYPE numeric_min, UC_ARG_OPT_NUMERIC_TYPE numeric_max, ucArgOpt *next);

UC_EXPORTED const char *ucArgOpt_format_validation_err(ucArgOpt *p, uc_cmd_line *cmd, uc_arg_tok *arg_tok, const char *switch_name);

#endif
