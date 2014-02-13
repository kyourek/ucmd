#include <stdlib.h>
#include "uc_arg_opt_p.h"
#include "uc_memory_manager_p.h"

static const char *numeric_arg_name = "<number>";

UC_MEMORY_MANAGER_INIT(uc_arg_opt, UC_ARG_OPT_COUNT);

static uc_arg_opt *create(const char *name, const char *desc, uc_bool is_required, int min_tok_count, int max_tok_count, uc_bool is_numeric, UC_ARG_OPT_NUMERIC_TYPE numeric_min, UC_ARG_OPT_NUMERIC_TYPE numeric_max, uc_arg_opt *next) {
    return uc_arg_opt_init(
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

static const char *format_is_required_validation_err(uc_arg_opt *p, uc_cmd_line *cmd, uc_arg_tok *arg_tok, const char *switch_name, const char *prefix) {

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

static const char *format_is_numeric_validation_err(uc_arg_opt *p, uc_cmd_line *cmd, uc_arg_tok *arg_tok, const char *switch_name, const char *prefix) {
    double arg_num;

    /* check if this argument option must be numeric */
    if (uc_arg_opt_is_numeric(p)) {

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
            if (uc_arg_opt_get_numeric_min(p) > arg_num) {

                /* the number is below the lower bound, so return the
                   appropriate error message */
                return NULL == switch_name
                    ? uc_cmd_line_format_response(cmd, "%sthe argument \"%f\" is above the minimum value of \"%f\".", prefix, arg_num, uc_arg_opt_get_numeric_min(p))
                    : uc_cmd_line_format_response(cmd, "%sthe \"%s\" argument \"%f\" is above the minimum value of \"%f\".", prefix, switch_name, arg_num, uc_arg_opt_get_numeric_min(p));
            }

            /* check that the number is below the upper bound */
            if (uc_arg_opt_get_numeric_max(p) < arg_num) {

                /* the number is above the upper bound, so return the
                   appropriate error message */
                return NULL == switch_name
                    ? uc_cmd_line_format_response(cmd, "%sthe argument \"%f\" is below the maximum value of \"%f\".", prefix, arg_num, uc_arg_opt_get_numeric_max(p))
                    : uc_cmd_line_format_response(cmd, "%sthe \"%s\" argument \"%f\" is below the maximum value of \"%f\".", prefix, switch_name, arg_num, uc_arg_opt_get_numeric_max(p));
            }
        }
    }

    /* return no error */
    return NULL;
}

static const char *format_arg_tok_validation_err(uc_arg_opt *p, uc_cmd_line *cmd, uc_arg_tok *arg_tok, const char *switch_name, const char *prefix) {
    const char *err;

    err = format_is_required_validation_err(p, cmd, arg_tok, switch_name, prefix);
    if (err) return err;

    err = format_is_numeric_validation_err(p, cmd, arg_tok, switch_name, prefix);
    if (err) return err;

    return NULL;
}

int uc_arg_opt_get_min_tok_count(uc_arg_opt *p) {
    if (NULL == p) return 0;
    return p->min_tok_count;
}

int uc_arg_opt_get_max_tok_count(uc_arg_opt *p) {
    if (NULL == p) return 0;
    return p->max_tok_count;
}

uc_bool uc_arg_opt_is_numeric(uc_arg_opt *p) {
    if (NULL == p) return UC_FALSE;
    return p->is_numeric;
}

UC_ARG_OPT_NUMERIC_TYPE uc_arg_opt_get_numeric_min(uc_arg_opt *p) {
    if (NULL == p) return 0;
    return p->numeric_min;
}

UC_ARG_OPT_NUMERIC_TYPE uc_arg_opt_get_numeric_max(uc_arg_opt* p) {
    if (NULL == p) return 0;
    return p->numeric_max;
}

uc_arg_opt *uc_arg_opt_get_next(uc_arg_opt* p) {
    if (NULL == p) return NULL;
    return p->next;
}

uc_arg_opt *uc_arg_opt_init(uc_arg_opt *p, const char *name, const char *desc, uc_bool is_required, int min_tok_count, int max_tok_count, uc_bool is_numeric, UC_ARG_OPT_NUMERIC_TYPE numeric_min, UC_ARG_OPT_NUMERIC_TYPE numeric_max, uc_arg_opt *next) {

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

uc_arg_opt *uc_arg_opt_create(const char *name, const char *desc, uc_arg_opt *next) {
    return create(name, desc, UC_FALSE, 0, 1, UC_FALSE, 0, 0, next);
}

uc_arg_opt *uc_arg_opt_create_multiple(const char *name, const char *desc, int min_tok_count, int max_tok_count) {
    return create(name, desc, min_tok_count > 0 ? UC_TRUE : UC_FALSE, min_tok_count, max_tok_count, UC_FALSE, 0, 0, NULL);
}

uc_arg_opt *uc_arg_opt_create_required(const char *name, const char *desc, uc_arg_opt *next) {
    return create(name, desc, UC_TRUE, 1, 1, UC_FALSE, 0, 0, next);
}

uc_arg_opt *uc_arg_opt_create_numeric(const char *desc, UC_ARG_OPT_NUMERIC_TYPE numeric_min, UC_ARG_OPT_NUMERIC_TYPE numeric_max, uc_arg_opt *next) {
    return create(numeric_arg_name, desc, UC_FALSE, 0, 1, UC_TRUE, numeric_min, numeric_max, next);
}

uc_arg_opt *uc_arg_opt_create_multiple_numeric(const char *desc, int min_tok_count, int max_tok_count, UC_ARG_OPT_NUMERIC_TYPE numeric_min, UC_ARG_OPT_NUMERIC_TYPE numeric_max) {
    return create(numeric_arg_name, desc, min_tok_count > 0 ? UC_TRUE : UC_FALSE, min_tok_count, max_tok_count, UC_TRUE, numeric_min, numeric_max, NULL);
}

uc_arg_opt *uc_arg_opt_create_required_numeric(const char *desc, UC_ARG_OPT_NUMERIC_TYPE numeric_min, UC_ARG_OPT_NUMERIC_TYPE numeric_max, uc_arg_opt *next) {
    return create(numeric_arg_name, desc, UC_TRUE, 1, 1, UC_TRUE, numeric_min, numeric_max, next);
}

const char *uc_arg_opt_format_validation_err(uc_arg_opt *p, uc_cmd_line *cmd, uc_arg_tok *arg_tok, const char *switch_name) {
    int tok_count, max_tok_count;

    /* if a switch name was given, then this argument belongs
       to a switch. Otherwise, it's a command argument. */
    const char *prefix = switch_name == NULL ? uc_opt_validation_err_invalid_argument_prefix : uc_opt_validation_err_invalid_switch_argument_prefix;

    /* validate the current argument token */
    const char *err = format_arg_tok_validation_err(p, cmd, arg_tok, switch_name, prefix);
    if (err) return err;

    /* check if this argument option allows multiple tokens */
    max_tok_count = uc_arg_opt_get_max_tok_count(p);
    if (1 < max_tok_count) {

        /* loop through the argument tokens */
        tok_count = arg_tok ? 1 : 0;
        while (arg_tok) {

            /* get the next argument token in the list */
            arg_tok = uc_arg_tok_get_next(arg_tok);

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
        if (tok_count < uc_arg_opt_get_min_tok_count(p)) return uc_cmd_line_format_response(cmd, "%snot enough arguments for %s.", prefix, uc_opt_get_name((uc_opt*)p));
    }

    /* if we got here, then no error was found */
    return NULL;
}

void uc_arg_opt_destroy(uc_arg_opt *p) {
    uc_memory_manager_destroy(p);
}

void uc_arg_opt_destroy_chain(uc_arg_opt *p) {
    uc_arg_opt *next = p;
    while (NULL != next) {
        p = next;
        next = uc_arg_opt_get_next(p);
        uc_arg_opt_destroy(p);
    }
}
