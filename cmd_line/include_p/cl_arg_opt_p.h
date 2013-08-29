#ifndef CL_ARG_OPT_P_H
#define CL_ARG_OPT_P_H

#include "cl_opt.h"
#include "cl_opt_p.h"

struct cl_arg_opt {
    cl_opt base;

    CL_BOOL is_numeric;

    double numeric_min;
    double numeric_max;

    cl_arg_opt *next;
};

cl_arg_opt *cl_arg_opt_init(cl_arg_opt *p, const char *name, const char *desc, CL_BOOL is_required, CL_BOOL is_numeric, double numeric_min, double numeric_max, cl_arg_opt *next);

#endif
