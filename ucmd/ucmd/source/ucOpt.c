#include <stdlib.h>
#include "ucmd_internal.h"

const char *ucOpt_validation_err_invalid_argument_prefix = "Invalid argument: ";
const char *ucOpt_validation_err_invalid_switch_prefix = "Invalid switch: ";
const char *ucOpt_validation_err_invalid_switch_argument_prefix = "Invalid switch argument: ";

const char *ucOpt_get_name(ucOpt *p) {
    if (NULL == p) return NULL;
    return p->name;
}

const char *ucOpt_get_desc(ucOpt *p) {
    if (NULL == p) return NULL;
    return p->desc;
}

ucBool ucOpt_is_required(ucOpt *p) {
    if (NULL == p) return ucBool_FALSE;
    return p->is_required;
}

void ucOpt_send_help(ucOpt *p, ucCmdLine *cmd, const char *prefix) {
    static const char *required_format = "%s%s: %s";
    static const char *optional_format = "%s[%s]: %s";
    ucCmdLine_respond(cmd, ucCmdLine_format_response(
        cmd,
        ucOpt_is_required(p) ? required_format : optional_format,
        prefix,
        ucOpt_get_name(p),
        ucOpt_get_desc(p)
    ));
}

ucOpt *ucOpt_init(ucOpt *p, const char *name, const char *desc, ucBool is_required) {
    if (NULL == p) return NULL;
    p->name = name;
    p->desc = desc;
    p->is_required = is_required;
    return p;
}
