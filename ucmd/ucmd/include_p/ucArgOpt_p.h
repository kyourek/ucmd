#ifndef UCARGOPT_P_H
#define UCARGOPT_P_H

#include "ucArgOpt.h"
#include "ucCmdLine.h"
#include "ucOpt_p.h"

struct ucArgOpt {
    ucOpt base;

    ucBool is_numeric;
    UC_ARG_OPT_NUMERIC_TYPE numeric_min;
    UC_ARG_OPT_NUMERIC_TYPE numeric_max;

    int max_tok_count;
    int min_tok_count;

    ucArgOpt *next;
};

UC_EXPORTED ucArgOpt *ucArgOpt_init(ucArgOpt *p, const char *name, const char *desc, ucBool is_required, int min_tok_count, int max_tok_count, ucBool is_numeric, UC_ARG_OPT_NUMERIC_TYPE numeric_min, UC_ARG_OPT_NUMERIC_TYPE numeric_max, ucArgOpt *next);

UC_EXPORTED const char *ucArgOpt_format_validation_err(ucArgOpt *p, ucCmdLine *cmd, ucArgTok *arg_tok, const char *switch_name);

#endif
