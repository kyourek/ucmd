#ifndef CL_SWITCH_OPT_P_H
#define CL_SWITCH_OPT_P_H

#include "cl_arg_opt_owner.h"
#include "cl_arg_opt_owner_p.h"
#include "cl_opt.h"
#include "cl_opt_p.h"
#include "cl_common.h"

struct cl_switch_opt {
    cl_arg_opt_owner base;
    cl_switch_opt *next;
};

cl_switch_opt *cl_switch_opt_init(cl_switch_opt *p, const char *name, const char *desc, CL_BOOL is_required, cl_arg_opt *arg_opt, cl_switch_opt *next);

#endif
