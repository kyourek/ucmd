#include <string.h>
#include "ucArgOpt_p.h"
#include "ucArgOptOwner_p.h"
#include "uc_cmd_line.h"
#include "uc_memory_manager_p.h"
#include "uc_switch_opt_p.h"

UC_MEMORY_MANAGER_INIT(uc_switch_opt, UC_SWITCH_OPT_COUNT);

static uc_switch_opt *create_switch_opt(void) {
    return uc_memory_manager_create();
}

ucArgOpt *uc_switch_opt_get_arg_opt(uc_switch_opt *p) {
    return ucArgOptOwner_get_arg_opt((ucArgOptOwner*)p);
}

uc_switch_opt *uc_switch_opt_get_next(uc_switch_opt *p) {
    if (NULL == p) return NULL;
    return p->next;
}

uc_switch_opt *uc_switch_opt_find(uc_switch_opt *p, const char *name) {
    while (NULL != p) {
        if (0 == strcmp(uc_opt_get_name((uc_opt*)p), name)) {
            return p;
        }
        p = uc_switch_opt_get_next(p);
    }
    return NULL;
}

uc_switch_opt *uc_switch_opt_init(uc_switch_opt *p, const char *name, const char *desc, ucBool is_required, ucArgOpt *arg_opt, uc_switch_opt *next) {
    if (NULL == p) return NULL;
    if (NULL == ucArgOptOwner_init((ucArgOptOwner*)p, name, desc, is_required, arg_opt)) return NULL;

    p->next = next;
    return p;
}

uc_switch_opt *uc_switch_opt_create(const char *name, const char *desc, ucArgOpt *arg_opt, uc_switch_opt *next) {
    return uc_switch_opt_init(create_switch_opt(), name, desc, ucBool_false, arg_opt, next);
}

uc_switch_opt *uc_switch_opt_create_required(const char *name, const char *desc, ucArgOpt *arg_opt, uc_switch_opt *next) {
    return uc_switch_opt_init(create_switch_opt(), name, desc, ucBool_true, arg_opt, next);
}

void uc_switch_opt_destroy(uc_switch_opt *p) {
    uc_memory_manager_destroy(p);
}

void uc_switch_opt_destroy_chain(uc_switch_opt *p) {
    uc_switch_opt *next = p;
    while (NULL != next) {
        p = next;
        next = uc_switch_opt_get_next(p);
        ucArgOpt_destroy_chain(uc_switch_opt_get_arg_opt(p));
        uc_switch_opt_destroy(p);
    }
}

const char *uc_switch_opt_format_validation_err(uc_switch_opt *p, uc_cmd_line *cmd, uc_switch_tok *switch_tok) {

    /* check if the switch option is required */
    if (uc_opt_is_required((uc_opt*)p)) {

        /* check if the switch token is missing */
        if (NULL == switch_tok) {

            /* the switch is required, but it is not
               present, so send the error */
            return uc_cmd_line_format_response(cmd, "%sthe switch \"%s\" is required.", uc_opt_validation_err_invalid_switch_prefix, uc_opt_get_name((uc_opt*)p));
        }
    }

    /* return the result of the argument validation */
    return ucArgOptOwner_format_validation_err(
        (ucArgOptOwner*)p, 
        cmd, 
        uc_switch_tok_get_arg(switch_tok), 
        uc_opt_get_name((uc_opt*)p)
    );
}
