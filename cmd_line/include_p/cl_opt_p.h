#ifndef CL_OPT_P_H
#define CL_OPT_P_H

#include "cl_common.h"
#include "cl_opt.h"

struct cl_opt {
    const char *name;
    const char *desc;
    CL_BOOL is_required;
};

CL_EXPORTED cl_opt *cl_opt_init(cl_opt *p, const char *name, const char *desc, CL_BOOL is_required);

#endif
