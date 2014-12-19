#include <stddef.h>
#include "ucArgOpt_p.h"
#include "ucArgOpt_owner_p.h"

ucArgOpt *ucArgOpt_owner_get_arg_opt(ucArgOpt_owner *p) {
    if (NULL == p) return NULL;
    return p->arg_opt;
}

ucArgOpt_owner *ucArgOpt_owner_init(ucArgOpt_owner *p, const char *name, const char *desc, uc_bool is_required, ucArgOpt *arg_opt) {
    if (NULL == p) return NULL;
    if (NULL == uc_opt_init((uc_opt*)p, name, desc, is_required)) return NULL;
    p->arg_opt = arg_opt;
    return p;
}

const char *ucArgOpt_owner_format_validation_err(ucArgOpt_owner *p, uc_cmd_line *cmd, uc_arg_tok *arg_tok, const char *switch_name) {
    int max_arg_tok_count;

    /* set the prefix for error messages */
    const char *validation, *prefix = switch_name == NULL ? uc_opt_validation_err_invalid_argument_prefix : uc_opt_validation_err_invalid_switch_argument_prefix;

    /* get the first argument option */
    ucArgOpt *arg_opt = ucArgOpt_owner_get_arg_opt(p);

    /* check if an argument option does NOT exist */
    if (NULL == arg_opt) {

        /* check if the argumen token DOES exist */
        if (NULL != arg_tok) {

            /* the option does NOT exist, but the token DOES, so there's an error */
            return NULL == switch_name
                ? uc_cmd_line_format_response(cmd, "%sno argument options exist for command \"%s\".", prefix, uc_tok_get_value(uc_cmd_line_get_cmd_tok(cmd)))
                : uc_cmd_line_format_response(cmd, "%sno argument options exist for switch \"%s\".", prefix, switch_name);
        }

        /* neither the option nor the token exist, so no error here */
        return NULL;
    }

    /* loop through all the argument options */
    max_arg_tok_count = 0;
    while (NULL != arg_opt) {

        /* validate this argument option agains the current token */
        validation = ucArgOpt_format_validation_err(arg_opt, cmd, arg_tok, switch_name);
        if (NULL != validation) return validation;

        /* get the number of tokens that this option allows */
        max_arg_tok_count = ucArgOpt_get_max_tok_count(arg_opt);

        /* move to the next option and the next token */
        arg_opt = ucArgOpt_get_next(arg_opt);
        arg_tok = uc_arg_tok_get_next(arg_tok);
    }

    /* check if we have any remaining tokens */
    if (NULL != arg_tok) {

        /* check if the last argument option does NOT allow multiple tokens */
        if (2 > max_arg_tok_count) {

            /* we have remaining tokens but no arguments for them, so there's an error */
            return NULL == switch_name
                ? uc_cmd_line_format_response(cmd, "%sno option exists for argument \"%s\".", prefix, uc_tok_get_value(arg_tok))
                : uc_cmd_line_format_response(cmd, "%sno option exists for \"%s\" argument \"%s\".", prefix, switch_name, uc_tok_get_value(arg_tok));
        }
    }

    /* return no error */
    return NULL;
}
