#include "ucmd_internal.h"

ucArgOpt *ucArgOptOwner_get_arg_opt(ucArgOptOwner *p) {
    assert(p);
    return p->arg_opt;
}

ucArgOptOwner *ucArgOptOwner_init(ucArgOptOwner *p, const char *name, const char *desc, ucBool is_required, ucArgOpt *arg_opt) {
    assert(p);
    assert(ucOpt_init((ucOpt*)p, name, desc, is_required));
    p->arg_opt = arg_opt;
    return p;
}

const char *ucArgOptOwner_format_validation_err(ucArgOptOwner *p, ucCmd *cmd, ucArgTok *arg_tok, const char *switch_name) {
    int max_arg_tok_count;
    const char *validation;

    /* Get the first argument option. */
    ucArgOpt *arg_opt = ucArgOptOwner_get_arg_opt(p);

    /* Check if an argument option does NOT exist. */
    if (!arg_opt) {

        /* Check if the argument token DOES exist. */
        if (arg_tok) {

            /* The option does NOT exist, but the token DOES, so there's an error. */
            return switch_name
                ? ucCmd_format_response(cmd, ucOpt_INVALID "Switch"  " '%s' requires no arguments.", switch_name)
                : ucCmd_format_response(cmd, ucOpt_INVALID "Command" " '%s' requires no arguments.", ucCmd_get_command(cmd));
        }

        /* Neither the option nor the token exist, so no error here. */
        return NULL;
    }

    /* Loop through all the argument options. */
    max_arg_tok_count = 0;
    while (arg_opt) {

        /* Validate this argument option agains the current token. */
        validation = ucArgOpt_format_validation_err(arg_opt, cmd, arg_tok, switch_name);
        if (validation) return validation;

        /* Get the number of tokens that this option allows. */
        max_arg_tok_count = ucArgOpt_get_max_tok_count(arg_opt);

        /* Move to the next option and the next token. */
        arg_opt = ucArgOpt_get_next(arg_opt);
        arg_tok = arg_tok ? ucArgTok_get_next(arg_tok) : NULL;
    }

    /* Check if we have any remaining tokens. */
    if (arg_tok) {

        /* Check if the last argument option does NOT allow multiple tokens. */
        if (2 > max_arg_tok_count) {

            /* We have remaining tokens but no arguments for them, so there's an error. */
            return switch_name
                ? ucCmd_format_response(cmd, ucOpt_INVALID "Switch"  " '%s' has no option for argument '%s'.", switch_name, arg_tok)
                : ucCmd_format_response(cmd, ucOpt_INVALID "Command" " '%s' has no option for argument '%s'.", ucCmd_get_command(cmd), arg_tok);
        }
    }

    /* Return no error. */
    return NULL;
}
