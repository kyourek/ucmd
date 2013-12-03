#include <float.h>
#include <stdlib.h>
#include "uc_app_p.h"
#include "uc_math_app.h"
#include "uc_tok.h"
#include "uc_cmd_line.h"
#include "uc_cmd_line_opt.h"
#include "uc_cmd_line_toks.h"

typedef struct number_state {
    double value;
} number_state;

static number_state number_state_s = { 0 };

static const char *to_switch_name = "-to";
static const char *from_switch_name = "-from";
static const char *verbose_switch_name = "-v";

static uc_bool is_verbose(uc_cmd_line *cmd) {
    return uc_switch_tok_contains(uc_cmd_tok_get_switch(uc_cmd_line_get_cmd_tok(cmd)), verbose_switch_name);
}

static const char *add(uc_cmd_line *cmd, void *state) {
    const char *c1, *c2;
    double base_number, add_number, result;
    number_state *s;
    uc_bool verbose;
    uc_switch_tok *to_switch;
    uc_cmd_line_toks toks;

    s = (number_state*)state;
    verbose = is_verbose(cmd);
    uc_cmd_line_get_cmd_toks(cmd, &toks);

    c1 = uc_tok_get_value(toks.arg_tok);
    if (UC_FALSE == uc_tok_is_numeric(c1)) {
        if (verbose) uc_cmd_line_respond(cmd, uc_cmd_line_format_response(cmd, "The value \"%s\" is not numeric.", c1));
        return "NaN";
    }
    add_number = atof(c1);

    base_number = s->value;
    to_switch = uc_switch_tok_find(toks.switch_tok, to_switch_name);
    if (NULL != to_switch) {
        c2 = uc_tok_get_value(uc_switch_tok_get_arg(to_switch));
        if (UC_FALSE == uc_tok_is_numeric(c2)) {
            if (verbose) uc_cmd_line_respond(cmd, uc_cmd_line_format_response(cmd, "The value \"%s\" is not numeric.", c2));
            return "NaN";
        }
        base_number = atof(c2);
    }

    result = add_number + base_number;
    if (NULL == to_switch) {
        s->value = result;
    }

    if (verbose) {
        uc_cmd_line_respond(cmd, uc_cmd_line_format_response(cmd, "The number being added is %f.", add_number));
        uc_cmd_line_respond(cmd, uc_cmd_line_format_response(cmd, "The base number is %f.", base_number));
        uc_cmd_line_respond(cmd, uc_cmd_line_format_response(cmd, "Adding %f + %f...", base_number, add_number));
    }

    return uc_cmd_line_format_response(cmd, "%f", result);
}

static const char *subtract(uc_cmd_line *cmd, void *state) {
    const char *c1, *c2;
    double base_number, subtract_number, result;
    number_state *s;
    uc_bool verbose;
    uc_switch_tok *from_switch;
    uc_cmd_line_toks toks;

    s = (number_state*)state;
    verbose = is_verbose(cmd);
    uc_cmd_line_get_cmd_toks(cmd, &toks);

    c1 = uc_tok_get_value(toks.arg_tok);
    if (UC_FALSE == uc_tok_is_numeric(c1)) {
        if (verbose) uc_cmd_line_respond(cmd, uc_cmd_line_format_response(cmd, "The value \"%s\" is not numeric.", c1));
        return "NaN";
    }
    subtract_number = atof(c1);

    base_number = s->value;
    from_switch = uc_switch_tok_find(toks.switch_tok, from_switch_name);
    if (NULL != from_switch) {
        c2 = uc_tok_get_value(uc_switch_tok_get_arg(from_switch));
        if (UC_FALSE == uc_tok_is_numeric(c2)) {
            if (verbose) uc_cmd_line_respond(cmd, uc_cmd_line_format_response(cmd, "The value \"%s\" is not numeric.", c2));
            return "NaN";
        }
        base_number = atof(c2);
    }
    
    result = base_number - subtract_number;
    if (NULL == from_switch) {
        s->value = result;
    }

    if (verbose) {
        uc_cmd_line_respond(cmd, uc_cmd_line_format_response(cmd, "The number being subtracted is %f.", subtract_number));
        uc_cmd_line_respond(cmd, uc_cmd_line_format_response(cmd, "The base number is %f.", base_number));
        uc_cmd_line_respond(cmd, uc_cmd_line_format_response(cmd, "Subtracting %f - %f...", base_number, subtract_number));
    }

    return uc_cmd_line_format_response(cmd, "%f", result);
}

static uc_cmd_line_opt *create_cmd_opt(uc_app *p) {

    uc_switch_opt *verbose_opt = uc_switch_opt_create(verbose_switch_name, "Print extra information (verbose).", NULL, NULL);

    return 
        uc_cmd_line_opt_create(
            add, &number_state_s, "add", "Adds two numbers together.",
                uc_arg_opt_create_required_numeric("The number to be added.", DBL_MIN, DBL_MAX, NULL),
                uc_switch_opt_create(to_switch_name, "Switch that specifies the value to which the number will be added. If not provided, the number is added to the current state.",
                    uc_arg_opt_create_required_numeric("The value to which the number will be added.", DBL_MIN, DBL_MAX, NULL),
                verbose_opt
        ),
        uc_cmd_line_opt_create(
            subtract, &number_state_s, "subtract", "Subtracts one number from another.",
                uc_arg_opt_create_required_numeric("The number to be subtracted.", DBL_MIN, DBL_MAX, NULL),
                uc_switch_opt_create(from_switch_name, "Switch that specifies the value from which the number will be subtracted. If not provided, the number is subtracted from the current state.",
                    uc_arg_opt_create_required_numeric("The value from which the number will be subtracted.", DBL_MIN, DBL_MAX, NULL),
                verbose_opt
        ),
        NULL
        ));
}

uc_app *uc_math_app_get_instance(void) {
    static uc_app instance;
    static uc_app *instance_p = NULL;
    if (NULL == instance_p) {
        instance_p = uc_app_init(&instance, create_cmd_opt);
    }
    return instance_p;
}
