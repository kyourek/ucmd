#ifndef CL_ARG_OPT_OWNER_P_H
#define CL_ARG_OPT_OWNER_P_H

#include "cl_arg_opt_p.h"
#include "cl_arg_opt_owner.h"
#include "cl_opt_p.h"

struct cl_arg_opt_owner {
    cl_opt base;
    cl_arg_opt *arg_opt;
};

CL_EXPORTED cl_arg_opt_owner *cl_arg_opt_owner_init(cl_arg_opt_owner *p, const char *name, const char *desc, CL_BOOL is_required, cl_arg_opt *arg_opt);

CL_EXPORTED const char *cl_arg_opt_owner_format_validation_err(cl_arg_opt_owner *p, cl_cmd_line *cmd, cl_arg_tok *arg_tok, const char *switch_name);

#endif
