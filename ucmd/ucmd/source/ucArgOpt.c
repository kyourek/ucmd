#include <stdlib.h>
#include "ucArgOpt_p.h"
#include "uc_memory_manager_p.h"

static const char *numeric_arg_name = "<number>";

UC_MEMORY_MANAGER_INIT(ucArgOpt, UC_ARG_OPT_COUNT);

static ucArgOpt *create(const char *name, const char *desc, ucBool is_required, int min_tok_count, int max_tok_count, ucBool is_numeric, UC_ARG_OPT_NUMERIC_TYPE numeric_min, UC_ARG_OPT_NUMERIC_TYPE numeric_max, ucArgOpt *next) {
    return ucArgOpt_init(
        uc_memory_manager_create(),
        name,
        desc,
        is_required,
        min_tok_count,
        max_tok_count,
        is_numeric,
        numeric_min,
        numeric_max,
        next
    );
}

static const char *format_is_required_validation_err(ucArgOpt *p, uc_cmd_line *cmd, ucArgTok *arg_tok, const char *switch_name, const char *prefix) {

    /* check if this argument is required */
    if (uc_opt_is_required((uc_opt*)p)) {

        /* the argument is required, so check that the argument
           token exists */
        if (NULL == arg_tok) {

            /* the argument is required, but the token does NOT exist,
               so there's an error here. Return the appropriate error
               message */
            return NULL == switch_name
                ? uc_cmd_line_format_response(cmd, "%sthe argument \"%s\" is required.", prefix, uc_opt_get_name((uc_opt*)p))
                : uc_cmd_line_format_response(cmd, "%sthe argument \"%s\" is required for switch \"%s\".", prefix, uc_opt_get_name((uc_opt*)p), switch_name);
        }
    }

    /* return no error */
    return NULL;
}

static const char *format_is_numeric_validation_err(ucArgOpt *p, uc_cmd_line *cmd, ucArgTok *arg_tok, const char *switch_name, const char *prefix) {
    double arg_num;

    /* check if this argument option must be numeric */
    if (ucArgOpt_is_numeric(p)) {

        /* check if the argument token was provided */
        if (NULL != arg_tok) {
            
            /* check if the argument token is not numeric */
            if (!uc_tok_is_numeric((uc_tok*)arg_tok)) {

                /* the argument option requires a number, but the
                   provided token is not numeric, so return the
                   appropriate error message */
                return NULL == switch_name
                    ? uc_cmd_line_format_response(cmd, "%sthe argument \"%s\" is not numeric.", prefix, uc_tok_get_value(arg_tok))
                    : uc_cmd_line_format_response(cmd, "%sthe \"%s\" argument \"%s\" is not numeric.", prefix, switch_name, uc_tok_get_value(arg_tok));
            }

            /* the profided argument token is numeric,
               so convert it to a number */
            arg_num = atof(uc_tok_get_value((uc_tok*)arg_tok));

            /* check that that number is above the lower bound */
            if (ucArgOpt_get_numeric_min(p) > arg_num) {

                /* the number is below the lower bound, so return the
                   appropriate error message */
                return NULL == switch_name
                    ? uc_cmd_line_format_response(cmd, "%sthe argument \"%f\" is above the minimum value of \"%f\".", prefix, arg_num, ucArgOpt_get_numeric_min(p))
                    : uc_cmd_line_format_response(cmd, "%sthe \"%s\" argument \"%f\" is above the minimum value of \"%f\".", prefix, switch_name, arg_num, ucArgOpt_get_numeric_min(p));
            }

            /* check that the number is below the upper bound */
            if (ucArgOpt_get_numeric_max(p) < arg_num) {

                /* the number is above the upper bound, so return the
                   appropriate error message */
                return NULL == switch_name
                    ? uc_cmd_line_format_response(cmd, "%sthe argument \"%f\" is below the maximum value of \"%f\".", prefix, arg_num, ucArgOpt_get_numeric_max(p))
                    : uc_cmd_line_format_response(cmd, "%sthe \"%s\" argument \"%f\" is below the maximum value of \"%f\".", prefix, switch_name, arg_num, ucArgOpt_get_numeric_max(p));
            }
        }
    }

    /* return no error */
    return NULL;
}

static const char *format_arg_tok_validation_err(ucArgOpt *p, uc_cmd_line *cmd, ucArgTok *arg_tok, const char *switch_name, const char *prefix) {
    const char *err;

    err = format_is_required_validation_err(p, cmd, arg_tok, switch_name, prefix);
    if (err) return err;

    err = format_is_numeric_validation_err(p, cmd, arg_tok, switch_name, prefix);
    if (err) return err;

    return NULL;
}

int ucArgOpt_get_min_tok_count(ucArgOpt *p) {
    if (NULL == p) return 0;
    return p->min_tok_count;
}

int ucArgOpt_get_max_tok_count(ucArgOpt *p) {
    if (NULL == p) return 0;
    return p->max_tok_count;
}

ucBool ucArgOpt_is_numeric(ucArgOpt *p) {
    if (NULL == p) return ucBool_false;
    return p->is_numeric;
}

UC_ARG_OPT_NUMERIC_TYPE ucArgOpt_get_numeric_min(ucArgOpt *p) {
    if (NULL == p) return 0;
    return p->numeric_min;
}

UC_ARG_OPT_NUMERIC_TYPE ucArgOpt_get_numeric_max(ucArgOpt* p) {
    if (NULL == p) return 0;
    return p->numeric_max;
}

ucArgOpt *ucArgOpt_get_next(ucArgOpt* p) {
    if (NULL == p) return NULL;
    return p->next;
}

ucArgOpt *ucArgOpt_init(ucArgOpt *p, const char *name, const char *desc, ucBool is_required, int min_tok_count, int max_tok_count, ucBool is_numeric, UC_ARG_OPT_NUMERIC_TYPE numeric_min, UC_ARG_OPT_NUMERIC_TYPE numeric_max, ucArgOpt *next) {

    if (NULL == p) return NULL;
    if (NULL == uc_opt_init((uc_opt*)p, name, desc, is_required)) return NULL;

    p->min_tok_count = min_tok_count;
    p->max_tok_count = max_tok_count;
    p->is_numeric = is_numeric;
    p->numeric_min = numeric_min;
    p->numeric_max = numeric_max;
    p->next = next;

    return p;
}

ucArgOpt *ucArgOpt_create(const char *name, const char *desc, ucArgOpt *next) {
    return create(name, desc, ucBool_false, 0, 1, ucBool_false, 0, 0, next);
}

ucArgOpt *ucArgOpt_create_multiple(const char *name, const char *desc, int min_tok_count, int max_tok_count) {
    return create(name, desc, min_tok_count > 0 ? ucBool_true : ucBool_false, min_tok_count, max_tok_count, ucBool_false, 0, 0, NULL);
}

ucArgOpt *ucArgOpt_create_required(const char *name, const char *desc, ucArgOpt *next) {
    return create(name, desc, ucBool_true, 1, 1, ucBool_false, 0, 0, next);
}

ucArgOpt *ucArgOpt_create_numeric(const char *desc, UC_ARG_OPT_NUMERIC_TYPE numeric_min, UC_ARG_OPT_NUMERIC_TYPE numeric_max, ucArgOpt *next) {
    return create(numeric_arg_name, desc, ucBool_false, 0, 1, ucBool_true, numeric_min, numeric_max, next);
}

ucArgOpt *ucArgOpt_create_multiple_numeric(const char *desc, int min_tok_count, int max_tok_count, UC_ARG_OPT_NUMERIC_TYPE numeric_min, UC_ARG_OPT_NUMERIC_TYPE numeric_max) {
    return create(numeric_arg_name, desc, min_tok_count > 0 ? ucBool_true : ucBool_false, min_tok_count, max_tok_count, ucBool_true, numeric_min, numeric_max, NULL);
}

ucArgOpt *ucArgOpt_create_required_numeric(const char *desc, UC_ARG_OPT_NUMERIC_TYPE numeric_min, UC_ARG_OPT_NUMERIC_TYPE numeric_max, ucArgOpt *next) {
    return create(numeric_arg_name, desc, ucBool_true, 1, 1, ucBool_true, numeric_min, numeric_max, next);
}

const char *ucArgOpt_format_validation_err(ucArgOpt *p, uc_cmd_line *cmd, ucArgTok *arg_tok, const char *switch_name) {
    int tok_count, max_tok_count;

    /* if a switch name was given, then this argument belongs
       to a switch. Otherwise, it's a command argument. */
    const char *prefix = switch_name == NULL ? uc_opt_validation_err_invalid_argument_prefix : uc_opt_validation_err_invalid_switch_argument_prefix;

    /* validate the current argument token */
    const char *err = format_arg_tok_validation_err(p, cmd, arg_tok, switch_name, prefix);
    if (err) return err;

    /* check if this argument option allows multiple tokens */
    max_tok_count = ucArgOpt_get_max_tok_count(p);
    if (1 < max_tok_count) {

        /* loop through the argument tokens */
        tok_count = arg_tok ? 1 : 0;
        while (arg_tok) {

            /* get the next argument token in the list */
            arg_tok = ucArgTok_get_next(arg_tok);

            /* check if this token exists */
            if (arg_tok) {

                /* increment the number of tokens and make sure it is valid */
                tok_count++;
                if (tok_count > max_tok_count) return uc_cmd_line_format_response(cmd, "%stoo many arguments for %s.", prefix, uc_opt_get_name((uc_opt*)p));

                /* validate this token */
                err = format_arg_tok_validation_err(p, cmd, arg_tok, switch_name, prefix);
                if (err) return err;
            }
        }

        /* make sure we have enough tokens */
        if (tok_count < ucArgOpt_get_min_tok_count(p)) return uc_cmd_line_format_response(cmd, "%snot enough arguments for %s.", prefix, uc_opt_get_name((uc_opt*)p));
    }

    /* if we got here, then no error was found */
    return NULL;
}

void ucArgOpt_destroy(ucArgOpt *p) {
    uc_memory_manager_destroy(p);
}

void ucArgOpt_destroy_chain(ucArgOpt *p) {
    ucArgOpt *next = p;
    while (NULL != next) {
        p = next;
        next = ucArgOpt_get_next(p);
        ucArgOpt_destroy(p);
    }
}
