#ifndef CL_CMD_LINE_OPT_P_H
#define CL_CMD_LINE_OPT_P_H

#include "cl_arg_opt_owner_p.h"
#include "cl_cmd_line_opt.h"
#include "cl_switch_opt_p.h"

struct cl_cmd_line_opt {
    cl_arg_opt_owner base;
    cl_cmd_line_opt_func *func;
    void *state;
    cl_switch_opt *switch_opt;
    cl_cmd_line_opt *next;
};

CL_EXPORTED cl_cmd_line_opt *cl_cmd_line_opt_init(cl_cmd_line_opt *p, cl_cmd_line_opt_func *func, void* state, const char *name, const char *desc, cl_arg_opt* arg_opt, cl_switch_opt *switch_opt, cl_cmd_line_opt *next);

CL_EXPORTED const char *cl_cmd_line_opt_format_validation_err(cl_cmd_line_opt *p, cl_cmd_line *cmd);

#endif
