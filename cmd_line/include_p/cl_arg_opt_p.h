#ifndef CL_ARG_OPT_P_H
#define CL_ARG_OPT_P_H

#include "cl_arg_opt.h"
#include "cl_cmd_line.h"
#include "cl_opt_p.h"

struct cl_arg_opt {
    cl_opt base;

    cl_bool is_numeric;
    double numeric_min;
    double numeric_max;

    int max_tok_count;
    int min_tok_count;

    cl_arg_opt *next;
};

CL_EXPORTED cl_arg_opt *cl_arg_opt_init(cl_arg_opt *p, const char *name, const char *desc, cl_bool is_required, int min_tok_count, int max_tok_count, cl_bool is_numeric, double numeric_min, double numeric_max, cl_arg_opt *next);

CL_EXPORTED const char *cl_arg_opt_format_validation_err(cl_arg_opt *p, cl_cmd_line *cmd, cl_arg_tok *arg_tok, const char *switch_name);

#endif
