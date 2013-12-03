#include <stdlib.h>
#include "uc_opt_p.h"

const char *uc_opt_validation_err_invalid_argument_prefix = "Invalid argument: ";
const char *uc_opt_validation_err_invalid_switch_prefix = "Invalid switch: ";
const char *uc_opt_validation_err_invalid_switch_argument_prefix = "Invalid switch argument: ";

const char *uc_opt_get_name(uc_opt *p) {
    if (NULL == p) return NULL;
    return p->name;
}

const char *uc_opt_get_desc(uc_opt *p) {
    if (NULL == p) return NULL;
    return p->desc;
}

uc_bool uc_opt_is_required(uc_opt *p) {
    if (NULL == p) return UC_FALSE;
    return p->is_required;
}

void uc_opt_send_help(uc_opt *p, uc_cmd_line *cmd, const char *prefix) {
    static const char *required_format = "%s%s: %s";
    static const char *optional_format = "%s[%s]: %s";
    uc_cmd_line_respond(cmd, uc_cmd_line_format_response(
        cmd,
        uc_opt_is_required(p) ? required_format : optional_format,
        prefix,
        uc_opt_get_name(p),
        uc_opt_get_desc(p)
    ));
}

uc_opt *uc_opt_init(uc_opt *p, const char *name, const char *desc, uc_bool is_required) {
    if (NULL == p) return NULL;
    p->name = name;
    p->desc = desc;
    p->is_required = is_required;
    return p;
}
