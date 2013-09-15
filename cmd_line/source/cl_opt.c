#include <stdlib.h>
#include "cl_cmd_line.h"
#include "cl_common.h"
#include "cl_opt.h"
#include "cl_opt_p.h"

const char *cl_opt_validation_err_invalid_argument_prefix = "Invalid argument: ";
const char *cl_opt_validation_err_invalid_switch_prefix = "Invalid switch: ";
const char *cl_opt_validation_err_invalid_switch_argument_prefix = "Invalid switch argument: ";

const char *cl_opt_get_name(cl_opt *p) {
    if (NULL == p) return NULL;
    return p->name;
}

const char *cl_opt_get_desc(cl_opt *p) {
    if (NULL == p) return NULL;
    return p->desc;
}

CL_BOOL cl_opt_is_required(cl_opt *p) {
    if (NULL == p) return CL_FALSE;
    return p->is_required;
}

void cl_opt_send_help(cl_opt *p, cl_cmd_line *cmd, const char *prefix) {
    static const char *required_format = "%s%s: %s";
    static const char *optional_format = "%s[%s]: %s";
    cl_cmd_line_respond(cmd, cl_cmd_line_format_response(
        cmd,
        cl_opt_is_required(p) ? required_format : optional_format,
        prefix,
        cl_opt_get_name(p),
        cl_opt_get_desc(p)
    ));
}

cl_opt *cl_opt_init(cl_opt *p, const char *name, const char *desc, CL_BOOL is_required) {
    if (NULL == p) return NULL;
    p->name = name;
    p->desc = desc;
    p->is_required = is_required;
    return p;
}
