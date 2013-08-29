#include <string.h>
#include "cl_arg_opt.h"
#include "cl_arg_tok.h"
#include "cl_cmd_tok.h"
#include "cl_opt.h"
#include "cl_switch_opt.h"
#include "cl_switch_tok.h"
#include "cl_tok.h"
#include "cl_cmd_line.h"
#include "cl_cmd_line_opt.h"
#include "cl_cmd_line_validator.h"
#include "cl_cmd_line_validator_p.h"

static const char *validate_arg_opt(cl_arg_opt *arg_opt, cl_arg_tok *arg_tok, cl_cmd_line *cmd, const char *switch_name) {
    const char *prefix = switch_name == NULL ? "Invalid argument: " : "Invalid switch argument: ";

    if (NULL == arg_opt) {
        if (NULL != arg_tok) {
            if (NULL == switch_name) {
                return cl_cmd_line_format_response(cmd, "%sno argument options exist for command \"%s\".", prefix, cl_tok_get_value(cl_cmd_line_get_cmd_tok(cmd)));
            }
            return cl_cmd_line_format_response(cmd, "%sno argument options exist for switch \"%s\".", prefix, switch_name);
        }
        return NULL;
    }

    while (NULL != arg_opt) {
        if (cl_opt_is_required((cl_opt*)arg_opt)) {
            if (NULL == arg_tok) {
                if (NULL == switch_name) {
                    return cl_cmd_line_format_response(cmd, "%sthe argument \"%s\" is required.", prefix, cl_opt_get_name((cl_opt*)arg_opt));
                }
                return cl_cmd_line_format_response(cmd, "%sthe argument \"%s\" is required for switch \"%s\".", prefix, cl_opt_get_name((cl_opt*)arg_opt), switch_name);
            }
        }
        if (cl_arg_opt_is_numeric(arg_opt) && (NULL != arg_tok)) {
            if (cl_tok_is_numeric((cl_tok*)arg_tok) == CL_FALSE) {
                if (NULL == switch_name) {
                    return cl_cmd_line_format_response(cmd, "%sthe argument \"%s\" is not numeric.", prefix, cl_tok_get_value(arg_tok));
                }
                return cl_cmd_line_format_response(cmd, "%sthe \"%s\" argument \"%s\" is not numeric.", prefix, switch_name, cl_tok_get_value(arg_tok));
            }
        }

        arg_opt = cl_arg_opt_get_next(arg_opt);
        arg_tok = cl_arg_tok_get_next(arg_tok);
    }
    if (NULL != arg_tok) {
        if (NULL == switch_name) {
            return cl_cmd_line_format_response(cmd, "%sno option exists for argument \"%s\".", prefix, cl_tok_get_value(arg_tok));
        }
        return cl_cmd_line_format_response(cmd, "%sno option exists for \"%s\" argument \"%s\".", prefix, switch_name, cl_tok_get_value(arg_tok));
    }

    return NULL;
}

static const char *validate_switch_opt(cl_switch_opt *switch_opt, cl_switch_tok *switch_tok, cl_cmd_line *cmd) {
    cl_switch_opt *found_switch_opt, *next_switch_opt;
    cl_switch_tok *found_switch_tok, *next_switch_tok;
    const char *switch_name, *validation, *invalid_switch = "Invalid switch: ";

    if (NULL == switch_opt) {
        if (NULL != switch_tok) {
            return cl_cmd_line_format_response(cmd, "%sno switch options exist for command \"%s\".", invalid_switch, cl_tok_get_value(cl_cmd_line_get_cmd_tok(cmd)));
        }
        return NULL;
    }

    next_switch_tok = switch_tok;

    while (NULL != next_switch_tok) {
        found_switch_opt = cl_switch_opt_find(switch_opt, cl_tok_get_value(next_switch_tok));
        if (NULL == found_switch_opt) {
            return cl_cmd_line_format_response(cmd, "%sno option exists for switch \"%s\".", invalid_switch, cl_tok_get_value(next_switch_tok));
        }
        next_switch_tok = cl_switch_tok_get_next(next_switch_tok);
    }

    next_switch_opt = switch_opt;

    while (NULL != next_switch_opt) {
        switch_name = cl_opt_get_name((cl_opt*)next_switch_opt);
        found_switch_tok = cl_switch_tok_find(switch_tok, switch_name);

        if (cl_opt_is_required((cl_opt*)next_switch_opt)) {
            if (NULL == found_switch_tok) {
                return cl_cmd_line_format_response(cmd, "%sthe switch \"%s\" is required.", invalid_switch, switch_name);
            }
        }

        if (NULL != found_switch_tok) {
            validation = validate_arg_opt(cl_switch_opt_get_arg_opt(next_switch_opt), cl_switch_tok_get_arg(found_switch_tok), cmd, switch_name);
            if (NULL != validation) return validation;
        }

        next_switch_opt = cl_switch_opt_get_next(next_switch_opt);
    }

    return NULL;
}

static const char *validate(cl_cmd_line_validator *p, cl_cmd_line *cmd, cl_cmd_line_opt *cmd_opt) {
    cl_arg_opt *arg_opt;
    cl_arg_tok *arg_tok;
    cl_cmd_tok *cmd_tok;
    cl_switch_opt *switch_opt;
    cl_switch_tok *switch_tok;
    const char *validation;

    if (NULL == cmd) return "Invalid command: null pointer.";

    cmd_tok = cl_cmd_line_get_cmd_tok(cmd);
    
    arg_tok = cl_cmd_tok_get_arg(cmd_tok);
    arg_opt = cl_cmd_line_opt_get_arg_opt(cmd_opt);
    validation = validate_arg_opt(arg_opt, arg_tok, cmd, NULL);
    if (NULL != validation) return validation;

    switch_tok = cl_cmd_tok_get_switch(cmd_tok);
    switch_opt = cl_cmd_line_opt_get_switch_opt(cmd_opt);
    validation = validate_switch_opt(switch_opt, switch_tok, cmd);
    if (NULL != validation) return validation;
    
    return NULL;
}

const char *cl_cmd_line_validator_validate(cl_cmd_line_validator *p, cl_cmd_line *cmd, cl_cmd_line_opt *cmd_opt) {
    if (NULL == p) return NULL;
    if (NULL == p->validate) return NULL;
    return p->validate(p, cmd, cmd_opt);
}

cl_cmd_line_validator *cl_cmd_line_validator_get_instance(void) {
    static cl_cmd_line_validator instance;
    static cl_cmd_line_validator *p = NULL;
    if (NULL == p) {
        p = &instance;
        p->validate = validate;
    }
    return p;
}

