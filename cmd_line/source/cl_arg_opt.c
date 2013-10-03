#include <stdlib.h>
#include "cl_arg_opt_p.h"
#include "cl_memory_manager_p.h"
#include "cl_opt_p.h"
#include "cl_tok.h"

#ifndef CL_ARG_OPT_COUNT
#define CL_ARG_OPT_COUNT        50
#endif

static const char *numeric_name = "<number>";

CL_MEMORY_MANAGER_INIT(cl_arg_opt, CL_ARG_OPT_COUNT);

static cl_arg_opt *create(const char *name, const char *desc, cl_bool is_required, int min_tok_count, int max_tok_count, cl_bool is_numeric, double numeric_min, double numeric_max, cl_arg_opt *next) {
    return cl_arg_opt_init(
        cl_memory_manager_create(),
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

static const char *format_is_required_validation_err(cl_arg_opt *p, cl_cmd_line *cmd, cl_arg_tok *arg_tok, const char *switch_name, const char *prefix) {

    /* check if this argument is required */
    if (cl_opt_is_required((cl_opt*)p)) {

        /* the argument is required, so check that the argument
           token exists */
        if (NULL == arg_tok) {

            /* the argument is required, but the token does NOT exist,
               so there's an error here. Return the appropriate error
               message */
            return NULL == switch_name
                ? cl_cmd_line_format_response(cmd, "%sthe argument \"%s\" is required.", prefix, cl_opt_get_name((cl_opt*)p))
                : cl_cmd_line_format_response(cmd, "%sthe argument \"%s\" is required for switch \"%s\".", prefix, cl_opt_get_name((cl_opt*)p), switch_name);
        }
    }

    /* return no error */
    return NULL;
}

static const char *format_is_numeric_validation_err(cl_arg_opt *p, cl_cmd_line *cmd, cl_arg_tok *arg_tok, const char *switch_name, const char *prefix) {
    double arg_num;

    /* check if this argument option must be numeric */
    if (cl_arg_opt_is_numeric(p)) {

        /* check if the argument token was provided */
        if (NULL != arg_tok) {
            
            /* check if the argument token is not numeric */
            if (!cl_tok_is_numeric((cl_tok*)arg_tok)) {

                /* the argument option requires a number, but the
                   provided token is not numeric, so return the
                   appropriate error message */
                return NULL == switch_name
                    ? cl_cmd_line_format_response(cmd, "%sthe argument \"%s\" is not numeric.", prefix, cl_tok_get_value(arg_tok))
                    : cl_cmd_line_format_response(cmd, "%sthe \"%s\" argument \"%s\" is not numeric.", prefix, switch_name, cl_tok_get_value(arg_tok));
            }

            /* the profided argument token is numeric,
               so convert it to a number */
            arg_num = atof(cl_tok_get_value((cl_tok*)arg_tok));

            /* check that that number is above the lower bound */
            if (cl_arg_opt_get_numeric_min(p) > arg_num) {

                /* the number is below the lower bound, so return the
                   appropriate error message */
                return NULL == switch_name
                    ? cl_cmd_line_format_response(cmd, "%sthe argument \"%f\" is above the minimum value of \"%f\".", prefix, arg_num, cl_arg_opt_get_numeric_min(p))
                    : cl_cmd_line_format_response(cmd, "%sthe \"%s\" argument \"%f\" is above the minimum value of \"%f\".", prefix, switch_name, arg_num, cl_arg_opt_get_numeric_min(p));
            }

            /* check that the number is below the upper bound */
            if (cl_arg_opt_get_numeric_max(p) < arg_num) {

                /* the number is above the upper bound, so return the
                   appropriate error message */
                return NULL == switch_name
                    ? cl_cmd_line_format_response(cmd, "%sthe argument \"%f\" is below the maximum value of \"%f\".", prefix, arg_num, cl_arg_opt_get_numeric_max(p))
                    : cl_cmd_line_format_response(cmd, "%sthe \"%s\" argument \"%f\" is below the maximum value of \"%f\".", prefix, switch_name, arg_num, cl_arg_opt_get_numeric_max(p));
            }
        }
    }

    /* return no error */
    return NULL;
}

static const char *format_arg_tok_validation_err(cl_arg_opt *p, cl_cmd_line *cmd, cl_arg_tok *arg_tok, const char *switch_name, const char *prefix) {
    const char *err;

    err = format_is_required_validation_err(p, cmd, arg_tok, switch_name, prefix);
    if (err) return err;

    err = format_is_numeric_validation_err(p, cmd, arg_tok, switch_name, prefix);
    if (err) return err;

    return NULL;
}

int cl_arg_opt_get_min_tok_count(cl_arg_opt *p) {
    if (NULL == p) return 0;
    return p->min_tok_count;
}

int cl_arg_opt_get_max_tok_count(cl_arg_opt *p) {
    if (NULL == p) return 0;
    return p->max_tok_count;
}

cl_bool cl_arg_opt_is_numeric(cl_arg_opt *p) {
    if (NULL == p) return CL_FALSE;
    return p->is_numeric;
}

double cl_arg_opt_get_numeric_min(cl_arg_opt *p) {
    if (NULL == p) return 0;
    return p->numeric_min;
}

double cl_arg_opt_get_numeric_max(cl_arg_opt* p) {
    if (NULL == p) return 0;
    return p->numeric_max;
}

cl_arg_opt *cl_arg_opt_get_next(cl_arg_opt* p) {
    if (NULL == p) return NULL;
    return p->next;
}

cl_arg_opt *cl_arg_opt_init(cl_arg_opt *p, const char *name, const char *desc, cl_bool is_required, int min_tok_count, int max_tok_count, cl_bool is_numeric, double numeric_min, double numeric_max, cl_arg_opt *next) {

    if (NULL == p) return NULL;
    if (NULL == cl_opt_init((cl_opt*)p, name, desc, is_required)) return NULL;

    p->min_tok_count = min_tok_count;
    p->max_tok_count = max_tok_count;
    p->is_numeric = is_numeric;
    p->numeric_min = numeric_min;
    p->numeric_max = numeric_max;
    p->next = next;

    return p;
}

cl_arg_opt *cl_arg_opt_create(const char *name, const char *desc, cl_arg_opt *next) {
    return create(name, desc, CL_FALSE, 0, 1, CL_FALSE, 0, 0, next);
}

cl_arg_opt *cl_arg_opt_create_multiple(const char *name, const char *desc, int min_tok_count, int max_tok_count) {
    return create(name, desc, min_tok_count > 0 ? CL_TRUE : CL_FALSE, min_tok_count, max_tok_count, CL_FALSE, 0, 0, NULL);
}

cl_arg_opt *cl_arg_opt_create_required(const char *name, const char *desc, cl_arg_opt *next) {
    return create(name, desc, CL_TRUE, 1, 1, CL_FALSE, 0, 0, next);
}

cl_arg_opt *cl_arg_opt_create_numeric(const char *desc, double numeric_min, double numeric_max, cl_arg_opt *next) {
    return create(numeric_name, desc, CL_FALSE, 0, 1, CL_TRUE, numeric_min, numeric_max, next);
}

cl_arg_opt *cl_arg_opt_create_multiple_numeric(const char *desc, int min_tok_count, int max_tok_count, double numeric_min, double numeric_max) {
    return create(numeric_name, desc, min_tok_count > 0 ? CL_TRUE : CL_FALSE, min_tok_count, max_tok_count, CL_TRUE, numeric_min, numeric_max, NULL);
}

cl_arg_opt *cl_arg_opt_create_required_numeric(const char *desc, double numeric_min, double numeric_max, cl_arg_opt *next) {
    return create(numeric_name, desc, CL_TRUE, 1, 1, CL_TRUE, numeric_min, numeric_max, next);
}

const char *cl_arg_opt_format_validation_err(cl_arg_opt *p, cl_cmd_line *cmd, cl_arg_tok *arg_tok, const char *switch_name) {
    int tok_count, max_tok_count;

    /* if a switch name was given, then this argument belongs
       to a switch. Otherwise, it's a command argument. */
    const char *prefix = switch_name == NULL ? cl_opt_validation_err_invalid_argument_prefix : cl_opt_validation_err_invalid_switch_argument_prefix;

    /* validate the current argument token */
    const char *err = format_arg_tok_validation_err(p, cmd, arg_tok, switch_name, prefix);
    if (err) return err;

    /* check if this argument option allows multiple tokens */
    max_tok_count = cl_arg_opt_get_max_tok_count(p);
    if (1 < max_tok_count) {

        /* loop through the argument tokens */
        tok_count = 0;
        while (arg_tok) {

            /* increment the number of tokens and make sure it is valid */
            tok_count++;
            if (tok_count > max_tok_count) return cl_cmd_line_format_response(cmd, "%stoo many arguments for %s.", prefix, cl_opt_get_name((cl_opt*)p));

            /* validate this token */
            err = format_arg_tok_validation_err(p, cmd, arg_tok, switch_name, prefix);
            if (err) return err;

            /* get the next argument token in the list */
            arg_tok = cl_arg_tok_get_next(arg_tok);
        }

        /* make sure we have enough tokens */
        if (tok_count < cl_arg_opt_get_min_tok_count(p)) return cl_cmd_line_format_response(cmd, "%snot enough arguments for %s.", prefix, cl_opt_get_name((cl_opt*)p));
    }

    /* if we got here, then no error was found */
    return NULL;
}

void cl_arg_opt_destroy(cl_arg_opt *p) {
    cl_memory_manager_destroy(p);
}

void cl_arg_opt_destroy_chain(cl_arg_opt *p) {
    cl_arg_opt *next = p;
    while (NULL != next) {
        p = next;
        next = cl_arg_opt_get_next(p);
        cl_arg_opt_destroy(p);
    }
}
