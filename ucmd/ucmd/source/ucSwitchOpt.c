#include <string.h>
#include "ucArgOpt_p.h"
#include "ucArgOptOwner_p.h"
#include "ucCmdLine.h"
#include "ucMemoryManager_p.h"
#include "ucSwitchOpt_p.h"

UC_MEMORY_MANAGER_INIT(ucSwitchOpt, UC_SWITCH_OPT_COUNT);

static ucSwitchOpt *create_switch_opt(void) {
    return ucMemoryManager_create();
}

ucArgOpt *ucSwitchOpt_get_arg_opt(ucSwitchOpt *p) {
    return ucArgOptOwner_get_arg_opt((ucArgOptOwner*)p);
}

ucSwitchOpt *ucSwitchOpt_get_next(ucSwitchOpt *p) {
    if (NULL == p) return NULL;
    return p->next;
}

ucSwitchOpt *ucSwitchOpt_find(ucSwitchOpt *p, const char *name) {
    while (NULL != p) {
        if (0 == strcmp(ucOpt_get_name((ucOpt*)p), name)) {
            return p;
        }
        p = ucSwitchOpt_get_next(p);
    }
    return NULL;
}

ucSwitchOpt *ucSwitchOpt_init(ucSwitchOpt *p, const char *name, const char *desc, ucBool is_required, ucArgOpt *arg_opt, ucSwitchOpt *next) {
    if (NULL == p) return NULL;
    if (NULL == ucArgOptOwner_init((ucArgOptOwner*)p, name, desc, is_required, arg_opt)) return NULL;

    p->next = next;
    return p;
}

ucSwitchOpt *ucSwitchOpt_create(const char *name, const char *desc, ucArgOpt *arg_opt, ucSwitchOpt *next) {
    return ucSwitchOpt_init(create_switch_opt(), name, desc, ucBool_false, arg_opt, next);
}

ucSwitchOpt *ucSwitchOpt_create_required(const char *name, const char *desc, ucArgOpt *arg_opt, ucSwitchOpt *next) {
    return ucSwitchOpt_init(create_switch_opt(), name, desc, ucBool_true, arg_opt, next);
}

void ucSwitchOpt_destroy(ucSwitchOpt *p) {
    ucMemoryManager_destroy(p);
}

void ucSwitchOpt_destroy_chain(ucSwitchOpt *p) {
    ucSwitchOpt *next = p;
    while (NULL != next) {
        p = next;
        next = ucSwitchOpt_get_next(p);
        ucArgOpt_destroy_chain(ucSwitchOpt_get_arg_opt(p));
        ucSwitchOpt_destroy(p);
    }
}

const char *ucSwitchOpt_format_validation_err(ucSwitchOpt *p, ucCmdLine *cmd, ucSwitchTok *switch_tok) {

    /* check if the switch option is required */
    if (ucOpt_is_required((ucOpt*)p)) {

        /* check if the switch token is missing */
        if (NULL == switch_tok) {

            /* the switch is required, but it is not
               present, so send the error */
            return ucCmdLine_format_response(cmd, "%sthe switch \"%s\" is required.", ucOpt_validation_err_invalid_switch_prefix, ucOpt_get_name((ucOpt*)p));
        }
    }

    /* return the result of the argument validation */
    return ucArgOptOwner_format_validation_err(
        (ucArgOptOwner*)p, 
        cmd, 
        ucSwitchTok_get_arg(switch_tok), 
        ucOpt_get_name((ucOpt*)p)
    );
}
