#ifndef CL_OPT_P_H
#define CL_OPT_P_H

#include "cl_opt.h"

struct cl_opt {
    const char *name;
    const char *desc;
    cl_bool is_required;
};

extern const char *cl_opt_validation_err_invalid_argument_prefix;
extern const char *cl_opt_validation_err_invalid_switch_prefix;
extern const char *cl_opt_validation_err_invalid_switch_argument_prefix;

CL_EXPORTED cl_opt *cl_opt_init(cl_opt *p, const char *name, const char *desc, cl_bool is_required);

#endif
