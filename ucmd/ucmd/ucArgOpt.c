#include <stdlib.h>
#include "ucmd_internal.h"

static const char *boolean_arg_name = "<boolean>";
static const char *numeric_arg_name = "<numeric>";

ucMemoryManager_INIT(ucArgOpt, ucArgOpt_COUNT);

static ucArgOpt *create(const char *name, const char *desc, ucBool is_required, int min_tok_count, int max_tok_count, ucBool is_boolean, ucBool is_numeric, ucArgOpt_NUMERIC_TYPE numeric_min, ucArgOpt_NUMERIC_TYPE numeric_max, ucArgOpt *next) {
    return ucArgOpt_init(ucMemoryManager_create(), name, desc, is_required, min_tok_count, max_tok_count, is_boolean, is_numeric, numeric_min, numeric_max, next);
}

static const char *format_is_required_validation_err(ucArgOpt *p, ucCmdLine *cmd, ucArgTok *arg_tok, const char *switch_name, const char *prefix) {

    /* check if this argument is required */
    if (ucOpt_is_required((ucOpt*)p)) {

        /* the argument is required, so check that the argument
           token exists */
        if (NULL == arg_tok) {

            /* the argument is required, but the token does NOT exist,
               so there's an error here. Return the appropriate error
               message */
            return NULL == switch_name
                ? ucCmdLine_format_response(cmd, "%sthe argument \"%s\" is required.", prefix, ucOpt_get_name((ucOpt*)p))
                : ucCmdLine_format_response(cmd, "%sthe argument \"%s\" is required for switch \"%s\".", prefix, ucOpt_get_name((ucOpt*)p), switch_name);
        }
    }

    /* return no error */
    return NULL;
}

static const char *format_is_boolean_validation_err(ucArgOpt *p, ucCmdLine *cmd, ucArgTok *arg_tok, const char *switch_name, const char *prefix) {
    ucBool is_bool;
    if (ucArgOpt_is_boolean(p)) {
        if (arg_tok) {
            is_bool = ucTok_is_boolean((ucTok*)arg_tok);
            if (!is_bool) {
                return NULL == switch_name
                    ? ucCmdLine_format_response(cmd, "%sthe argument \"%s\" is not boolean.", prefix, ucTok_get_value(arg_tok))
                    : ucCmdLine_format_response(cmd, "%sthe \"%s\" argument \"%s\" is not boolean.", prefix, switch_name, ucTok_get_value(arg_tok));
            }
        }
    }

    return NULL;
}

static const char *format_is_numeric_validation_err(ucArgOpt *p, ucCmdLine *cmd, ucArgTok *arg_tok, const char *switch_name, const char *prefix) {
    ucBool is_num;
    double arg_num;

    /* Check if this argument option must be numeric. */
    if (ucArgOpt_is_numeric(p)) {

        /* Check if the argument token was provided. */
        if (arg_tok) {
            
            /* Parse the argument token into a number. */
            is_num = ucTok_try_parse_numeric((ucTok*)arg_tok, &arg_num);
            if (!is_num) {

                /* The argument option requires a number, but the
                   provided token is not numeric, so return the
                   appropriate error message. */
                return NULL == switch_name
                    ? ucCmdLine_format_response(cmd, "%sthe argument \"%s\" is not numeric.", prefix, ucTok_get_value(arg_tok))
                    : ucCmdLine_format_response(cmd, "%sthe \"%s\" argument \"%s\" is not numeric.", prefix, switch_name, ucTok_get_value(arg_tok));
            }

            /* Check that that number is above the lower bound. */
            if (ucArgOpt_get_numeric_min(p) > arg_num) {

                /* The number is below the lower bound, so return the
                   appropriate error message. */
                return NULL == switch_name
                    ? ucCmdLine_format_response(cmd, "%sthe argument \"%f\" is above the minimum value of \"%f\".", prefix, arg_num, ucArgOpt_get_numeric_min(p))
                    : ucCmdLine_format_response(cmd, "%sthe \"%s\" argument \"%f\" is above the minimum value of \"%f\".", prefix, switch_name, arg_num, ucArgOpt_get_numeric_min(p));
            }

            /* Check that the number is below the upper bound. */
            if (ucArgOpt_get_numeric_max(p) < arg_num) {

                /* The number is above the upper bound, so return the
                   appropriate error message. */
                return NULL == switch_name
                    ? ucCmdLine_format_response(cmd, "%sthe argument \"%f\" is below the maximum value of \"%f\".", prefix, arg_num, ucArgOpt_get_numeric_max(p))
                    : ucCmdLine_format_response(cmd, "%sthe \"%s\" argument \"%f\" is below the maximum value of \"%f\".", prefix, switch_name, arg_num, ucArgOpt_get_numeric_max(p));
            }
        }
    }

    /* Return no error. */
    return NULL;
}

static const char *format_arg_tok_validation_err(ucArgOpt *p, ucCmdLine *cmd, ucArgTok *arg_tok, const char *switch_name, const char *prefix) {
    const char *err;

    err = format_is_required_validation_err(p, cmd, arg_tok, switch_name, prefix);
    if (err) return err;

    err = format_is_boolean_validation_err(p, cmd, arg_tok, switch_name, prefix);
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

ucBool ucArgOpt_is_boolean(ucArgOpt *p) {
    if (NULL == p) return ucBool_FALSE;
    return p->is_boolean;
}

ucBool ucArgOpt_is_numeric(ucArgOpt *p) {
    if (NULL == p) return ucBool_FALSE;
    return p->is_numeric;
}

ucArgOpt_NUMERIC_TYPE ucArgOpt_get_numeric_min(ucArgOpt *p) {
    if (NULL == p) return 0;
    return p->numeric_min;
}

ucArgOpt_NUMERIC_TYPE ucArgOpt_get_numeric_max(ucArgOpt* p) {
    if (NULL == p) return 0;
    return p->numeric_max;
}

ucArgOpt *ucArgOpt_get_next(ucArgOpt* p) {
    if (NULL == p) return NULL;
    return p->next;
}

ucArgOpt *ucArgOpt_init(ucArgOpt *p, const char *name, const char *desc, ucBool is_required, int min_tok_count, int max_tok_count, ucBool is_boolean, ucBool is_numeric, ucArgOpt_NUMERIC_TYPE numeric_min, ucArgOpt_NUMERIC_TYPE numeric_max, ucArgOpt *next) {

    if (NULL == p) return NULL;
    if (NULL == ucOpt_init((ucOpt*)p, name, desc, is_required)) return NULL;

    p->min_tok_count = min_tok_count;
    p->max_tok_count = max_tok_count;
    p->is_boolean = is_boolean;
    p->is_numeric = is_numeric;
    p->numeric_min = numeric_min;
    p->numeric_max = numeric_max;
    p->next = next;

    return p;
}

ucArgOpt *ucArgOpt_create(const char *name, const char *desc, ucArgOpt *next) {
    return create(name, desc, ucBool_FALSE, 0, 1, ucBool_FALSE, ucBool_FALSE, 0, 0, next);
}

ucArgOpt *ucArgOpt_create_multiple(const char *name, const char *desc, int min_tok_count, int max_tok_count) {
    return create(name, desc, min_tok_count > 0 ? ucBool_TRUE : ucBool_FALSE, min_tok_count, max_tok_count, ucBool_FALSE, ucBool_FALSE, 0, 0, NULL);
}

ucArgOpt *ucArgOpt_create_required(const char *name, const char *desc, ucArgOpt *next) {
    return create(name, desc, ucBool_TRUE, 1, 1, ucBool_FALSE, ucBool_FALSE, 0, 0, next);
}

ucArgOpt *ucArgOpt_create_boolean(const char *desc, ucArgOpt *next) {
    return create(boolean_arg_name, desc, ucBool_FALSE, 0, 1, ucBool_TRUE, ucBool_FALSE, 0, 0, next);
}

ucArgOpt *ucArgOpt_create_required_boolean(const char *desc, ucArgOpt *next) {
    return create(boolean_arg_name, desc, ucBool_TRUE, 0, 1, ucBool_TRUE, ucBool_FALSE, 0, 0, next);
}

ucArgOpt *ucArgOpt_create_numeric(const char *desc, ucArgOpt_NUMERIC_TYPE numeric_min, ucArgOpt_NUMERIC_TYPE numeric_max, ucArgOpt *next) {
    return create(numeric_arg_name, desc, ucBool_FALSE, 0, 1, ucBool_FALSE, ucBool_TRUE, numeric_min, numeric_max, next);
}

ucArgOpt *ucArgOpt_create_multiple_numeric(const char *desc, int min_tok_count, int max_tok_count, ucArgOpt_NUMERIC_TYPE numeric_min, ucArgOpt_NUMERIC_TYPE numeric_max) {
    return create(numeric_arg_name, desc, min_tok_count > 0 ? ucBool_TRUE : ucBool_FALSE, min_tok_count, max_tok_count, ucBool_FALSE, ucBool_TRUE, numeric_min, numeric_max, NULL);
}

ucArgOpt *ucArgOpt_create_required_numeric(const char *desc, ucArgOpt_NUMERIC_TYPE numeric_min, ucArgOpt_NUMERIC_TYPE numeric_max, ucArgOpt *next) {
    return create(numeric_arg_name, desc, ucBool_TRUE, 1, 1, ucBool_FALSE, ucBool_TRUE, numeric_min, numeric_max, next);
}

const char *ucArgOpt_format_validation_err(ucArgOpt *p, ucCmdLine *cmd, ucArgTok *arg_tok, const char *switch_name) {
    int tok_count, max_tok_count;

    /* if a switch name was given, then this argument belongs
       to a switch. Otherwise, it's a command argument. */
    const char *prefix = switch_name == NULL ? ucOpt_validation_err_invalid_argument_prefix : ucOpt_validation_err_invalid_switch_argument_prefix;

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
                if (tok_count > max_tok_count) return ucCmdLine_format_response(cmd, "%stoo many arguments for %s.", prefix, ucOpt_get_name((ucOpt*)p));

                /* validate this token */
                err = format_arg_tok_validation_err(p, cmd, arg_tok, switch_name, prefix);
                if (err) return err;
            }
        }

        /* make sure we have enough tokens */
        if (tok_count < ucArgOpt_get_min_tok_count(p)) return ucCmdLine_format_response(cmd, "%snot enough arguments for %s.", prefix, ucOpt_get_name((ucOpt*)p));
    }

    /* if we got here, then no error was found */
    return NULL;
}

void ucArgOpt_destroy(ucArgOpt *p) {
    ucMemoryManager_destroy(p);
}

void ucArgOpt_destroy_chain(ucArgOpt *p) {
    ucArgOpt *next = p;
    while (NULL != next) {
        p = next;
        next = ucArgOpt_get_next(p);
        ucArgOpt_destroy(p);
    }
}
