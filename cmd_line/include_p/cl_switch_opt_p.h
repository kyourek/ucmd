#ifndef CL_SWITCH_OPT_P_H
#define CL_SWITCH_OPT_P_H

#include "cl_arg_opt_owner_p.h"
#include "cl_cmd_line.h"
#include "cl_switch_opt.h"

struct cl_switch_opt {
    cl_arg_opt_owner base;
    cl_switch_opt *next;
};

CL_EXPORTED cl_switch_opt *cl_switch_opt_init(cl_switch_opt *p, const char *name, const char *desc, cl_bool is_required, cl_arg_opt *arg_opt, cl_switch_opt *next);

CL_EXPORTED const char *cl_switch_opt_format_validation_err(cl_switch_opt *p, cl_cmd_line *cmd, cl_switch_tok *switch_tok);

#endif
