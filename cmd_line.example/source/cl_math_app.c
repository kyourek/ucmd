#include <float.h>
#include <stdlib.h>
#include "cl_app.h"
#include "cl_app_p.h"
#include "cl_arg_opt.h"
#include "cl_cmd_tok.h"
#include "cl_common.h"
#include "cl_math_app.h"
#include "cl_switch_opt.h"
#include "cl_switch_tok.h"
#include "cl_tok.h"
#include "cl_cmd_line.h"
#include "cl_cmd_line_opt.h"
#include "cl_cmd_line_toks.h"

typedef struct number_state {
    double value;
} number_state;

static number_state number_state_s = { 0 };

static const char *to_switch_name = "-to";
static const char *from_switch_name = "-from";
static const char *verbose_switch_name = "-v";

static cl_switch_opt *get_verbose_opt(void) {
    static cl_switch_opt *verbose_opt = NULL;
    if (NULL == verbose_opt) {
        verbose_opt = cl_switch_opt_create(verbose_switch_name, "Print extra information (verbose).", NULL, NULL);
    }
    return verbose_opt;
}

static CL_BOOL is_verbose(cl_cmd_line *cmd) {
    return cl_switch_tok_contains(cl_cmd_tok_get_switch(cl_cmd_line_get_cmd_tok(cmd)), verbose_switch_name);
}

static const char *add(cl_cmd_line *cmd, void *state) {
    const char *c1, *c2;
    double base_number, add_number, result;
    number_state *s;
    CL_BOOL verbose;
    cl_switch_tok *to_switch;
    cl_cmd_line_toks toks;

    s = (number_state*)state;
    verbose = is_verbose(cmd);
    cl_cmd_line_get_cmd_toks(cmd, &toks);

    c1 = cl_tok_get_value(toks.arg_tok);
    if (CL_FALSE == cl_tok_is_numeric(c1)) {
        if (verbose) cl_cmd_line_respond(cmd, cl_cmd_line_format_response(cmd, "The value \"%s\" is not numeric.", c1));
        return "NaN";
    }
    add_number = atof(c1);

    base_number = s->value;
    to_switch = cl_switch_tok_find(toks.switch_tok, to_switch_name);
    if (NULL != to_switch) {
        c2 = cl_tok_get_value(cl_switch_tok_get_arg(to_switch));
        if (CL_FALSE == cl_tok_is_numeric(c2)) {
            if (verbose) cl_cmd_line_respond(cmd, cl_cmd_line_format_response(cmd, "The value \"%s\" is not numeric.", c2));
            return "NaN";
        }
        base_number = atof(c2);
    }

    result = add_number + base_number;
    if (NULL == to_switch) {
        s->value = result;
    }

    if (verbose) {
        cl_cmd_line_respond(cmd, cl_cmd_line_format_response(cmd, "The number being added is %f.", add_number));
        cl_cmd_line_respond(cmd, cl_cmd_line_format_response(cmd, "The base number is %f.", base_number));
        cl_cmd_line_respond(cmd, cl_cmd_line_format_response(cmd, "Adding %f + %f...", base_number, add_number));
    }

    return cl_cmd_line_format_response(cmd, "%f", result);
}

static const char *subtract(cl_cmd_line *cmd, void *state) {
    const char *c1, *c2;
    double base_number, subtract_number, result;
    number_state *s;
    CL_BOOL verbose;
    cl_switch_tok *from_switch;
    cl_cmd_line_toks toks;

    s = (number_state*)state;
    verbose = is_verbose(cmd);
    cl_cmd_line_get_cmd_toks(cmd, &toks);

    c1 = cl_tok_get_value(toks.arg_tok);
    if (CL_FALSE == cl_tok_is_numeric(c1)) {
        if (verbose) cl_cmd_line_respond(cmd, cl_cmd_line_format_response(cmd, "The value \"%s\" is not numeric.", c1));
        return "NaN";
    }
    subtract_number = atof(c1);

    base_number = s->value;
    from_switch = cl_switch_tok_find(toks.switch_tok, from_switch_name);
    if (NULL != from_switch) {
        c2 = cl_tok_get_value(cl_switch_tok_get_arg(from_switch));
        if (CL_FALSE == cl_tok_is_numeric(c2)) {
            if (verbose) cl_cmd_line_respond(cmd, cl_cmd_line_format_response(cmd, "The value \"%s\" is not numeric.", c2));
            return "NaN";
        }
        base_number = atof(c2);
    }
    
    result = base_number - subtract_number;
    if (NULL == from_switch) {
        s->value = result;
    }

    if (verbose) {
        cl_cmd_line_respond(cmd, cl_cmd_line_format_response(cmd, "The number being subtracted is %f.", subtract_number));
        cl_cmd_line_respond(cmd, cl_cmd_line_format_response(cmd, "The base number is %f.", base_number));
        cl_cmd_line_respond(cmd, cl_cmd_line_format_response(cmd, "Subtracting %f - %f...", base_number, subtract_number));
    }

    return cl_cmd_line_format_response(cmd, "%f", result);
}

static cl_cmd_line_opt *create_cmd_opt(cl_app *p) {
    return 
        cl_cmd_line_opt_create(
            add, &number_state_s, "add", "Adds two numbers together.",
                cl_arg_opt_create_required_numeric("The number to be added.", DBL_MIN, DBL_MAX, NULL),
                cl_switch_opt_create(to_switch_name, "Switch that specifies the value to which the number will be added. If not provided, the number is added to the current state.",
                    cl_arg_opt_create_required_numeric("The value to which the number will be added.", DBL_MIN, DBL_MAX, NULL),
                get_verbose_opt()
        ),
        cl_cmd_line_opt_create(
            subtract, &number_state_s, "subtract", "Subtracts one number from another.",
                cl_arg_opt_create_required_numeric("The number to be subtracted.", DBL_MIN, DBL_MAX, NULL),
                cl_switch_opt_create(from_switch_name, "Switch that specifies the value from which the number will be subtracted. If not provided, the number is subtracted from the current state.",
                    cl_arg_opt_create_required_numeric("The value from which the number will be subtracted.", DBL_MIN, DBL_MAX, NULL),
                get_verbose_opt()
        ),
        NULL
        ));
}

cl_app *cl_math_app_get_instance(void) {
	static cl_app instance;
	static cl_app *instance_p = NULL;
	if (NULL == instance_p) {
		instance_p = cl_app_init(&instance, create_cmd_opt);
	}
	return instance_p;
}
