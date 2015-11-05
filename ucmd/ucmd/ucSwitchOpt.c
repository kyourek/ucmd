#include "ucmd_internal.h"

ucInstance_INIT(ucSwitchOpt, ucSwitchOpt_COUNT);

ucArgOpt *ucSwitchOpt_get_arg_opt(ucSwitchOpt *p) {
    return ucArgOptOwner_get_arg_opt((ucArgOptOwner*)p);
}

ucSwitchOpt *ucSwitchOpt_get_next(ucSwitchOpt *p) {
    assert(p);
    return p->next;
}

ucSwitchOpt *ucSwitchOpt_find(ucSwitchOpt *p, const char *name) {
    assert(p);
    while (p) {
        if (uc_STR_EQ(ucOpt_get_name((ucOpt*)p), name)) {
            return p;
        }
        p = ucSwitchOpt_get_next(p);
    }
    return NULL;
}

ucSwitchOpt *ucSwitchOpt_init(ucSwitchOpt *p, const char *name, const char *desc, ucBool is_required, ucArgOpt *arg_opt, ucSwitchOpt *next) {
    assert(p);
    assert(ucArgOptOwner_init((ucArgOptOwner*)p, name, desc, is_required, arg_opt));
    p->next = next;
    return p;
}

ucSwitchOpt *ucSwitchOpt_create(const char *name, const char *desc, ucArgOpt *arg_opt, ucSwitchOpt *next) {
    return ucSwitchOpt_init(ucInstance_create(), name, desc, ucBool_false, arg_opt, next);
}

ucSwitchOpt *ucSwitchOpt_create_required(const char *name, const char *desc, ucArgOpt *arg_opt, ucSwitchOpt *next) {
    return ucSwitchOpt_init(ucInstance_create(), name, desc, ucBool_true, arg_opt, next);
}

void ucSwitchOpt_destroy(ucSwitchOpt *p) {
    ucInstance_destroy(p);
}

void ucSwitchOpt_destroy_chain(ucSwitchOpt *p) {
    ucArgOpt *arg;
    ucSwitchOpt *next;
    assert(p);
    next = p;
    while (next) {
        p = next;
        next = ucSwitchOpt_get_next(p);
        arg = ucSwitchOpt_get_arg_opt(p);
        if (arg) {
            ucArgOpt_destroy_chain(arg);
        }
        ucSwitchOpt_destroy(p);
    }
}

const char *ucSwitchOpt_format_validation_err(ucSwitchOpt *p, ucCmdLine *cmd, ucSwitchTok *switch_tok) {
    /* Check if the switch option is required,
    and then if it is missing. If it is required,
    but not present, send the error. */
    if (ucOpt_is_required((ucOpt*)p)) {
        if (!switch_tok) {
            return ucCmdLine_format_response(
                cmd, 
                ucOpt_INVALID "The switch \"%s\" is required.",
                ucOpt_get_name((ucOpt*)p));
        }
    }

    /* Return the result of the argument validation. */
    return ucArgOptOwner_format_validation_err(
        (ucArgOptOwner*)p, 
        cmd, 
        ucSwitchTok_get_arg(switch_tok), 
        ucOpt_get_name((ucOpt*)p)
    );
}
