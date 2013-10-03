#include <stdlib.h>
#include <stdarg.h>
#include "cl_arg_opt.h"
#include "cl_arg_opt_p.h"
#include "cl_arg_opt_owner.h"
#include "cl_arg_opt_owner_p.h"
#include "cl_arg_tok.h"
#include "cl_cmd_line.h"
#include "cl_common.h"
#include "cl_opt.h"
#include "cl_opt_p.h"
#include "cl_tok.h"

cl_arg_opt *cl_arg_opt_owner_get_arg_opt(cl_arg_opt_owner *p) {
    if (NULL == p) return NULL;
    return p->arg_opt;
}

cl_arg_opt_owner *cl_arg_opt_owner_init(cl_arg_opt_owner *p, const char *name, const char *desc, cl_bool is_required, cl_arg_opt *arg_opt) {
    if (NULL == p) return NULL;
    if (NULL == cl_opt_init((cl_opt*)p, name, desc, is_required)) return NULL;
    p->arg_opt = arg_opt;
    return p;
}

const char *cl_arg_opt_owner_format_validation_err(cl_arg_opt_owner *p, cl_cmd_line *cmd, cl_arg_tok *arg_tok, const char *switch_name) {
    int max_arg_tok_count;

    /* set the prefix for error messages */
    const char *validation, *prefix = switch_name == NULL ? cl_opt_validation_err_invalid_argument_prefix : cl_opt_validation_err_invalid_switch_argument_prefix;

    /* get the first argument option */
    cl_arg_opt *arg_opt = cl_arg_opt_owner_get_arg_opt(p);

    /* check if an argument option does NOT exist */
    if (NULL == arg_opt) {

        /* check if the argumen token DOES exist */
        if (NULL != arg_tok) {

            /* the option does NOT exist, but the token DOES, so there's an error */
            return NULL == switch_name
                ? cl_cmd_line_format_response(cmd, "%sno argument options exist for command \"%s\".", prefix, cl_tok_get_value(cl_cmd_line_get_cmd_tok(cmd)))
                : cl_cmd_line_format_response(cmd, "%sno argument options exist for switch \"%s\".", prefix, switch_name);
        }

        /* neither the option nor the token exist, so no error here */
        return NULL;
    }

    /* loop through all the argument options */
    max_arg_tok_count = 0;
    while (NULL != arg_opt) {

        /* validate this argument option agains the current token */
        validation = cl_arg_opt_format_validation_err(arg_opt, cmd, arg_tok, switch_name);
        if (NULL != validation) return validation;

        /* get the number of tokens that this option allows */
        max_arg_tok_count = cl_arg_opt_get_max_tok_count(arg_opt);

        /* move to the next option and the next token */
        arg_opt = cl_arg_opt_get_next(arg_opt);
        arg_tok = cl_arg_tok_get_next(arg_tok);
    }

    /* check if we have any remaining tokens */
    if (NULL != arg_tok) {

        /* check if the last argument option does NOT allow multiple tokens */
        if (2 > max_arg_tok_count) {

            /* we have remaining tokens but no arguments for them, so there's an error */
            return NULL == switch_name
                ? cl_cmd_line_format_response(cmd, "%sno option exists for argument \"%s\".", prefix, cl_tok_get_value(arg_tok))
                : cl_cmd_line_format_response(cmd, "%sno option exists for \"%s\" argument \"%s\".", prefix, switch_name, cl_tok_get_value(arg_tok));
        }
    }

    /* return no error */
    return NULL;
}
