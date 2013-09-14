#include <stdlib.h>
#include <string.h>
#include "cl_arg_opt.h"
#include "cl_arg_opt_p.h"
#include "cl_arg_opt_owner.h"
#include "cl_arg_opt_owner_p.h"
#include "cl_cmd_line_opt.h"
#include "cl_cmd_line_opt_p.h"
#include "cl_cmd_line_validator.h"
#include "cl_cmd_line_validator_p.h"
#include "cl_cmd_tok.h"
#include "cl_memory_manager_p.h"
#include "cl_opt.h"
#include "cl_opt_p.h"
#include "cl_switch_opt.h"
#include "cl_switch_opt_p.h"
#include "cl_tok.h"

CL_MEMORY_MANAGER_INIT(cl_cmd_line_opt, CL_CMD_COUNT_MAX);

static cl_cmd_line_opt *create_cmd_line_opt(void) {
    return cl_memory_manager_create();
}

static const char *add_opt_to_usage_response(cl_opt *opt, cl_cmd_line *cmd, const char *response) {
    static const char *required_format = "%s %s";
    static const char *optional_format = "%s [%s]";
    return cl_cmd_line_format_response(
        cmd,
        cl_opt_is_required(opt) ? required_format : optional_format,
        response,
        cl_opt_get_name(opt)
    );
}

static const char *add_arg_opts_to_usage_response(cl_arg_opt *arg_opt, cl_cmd_line *cmd, const char *response) {
    
    while (NULL != arg_opt) {
        response = add_opt_to_usage_response((cl_opt*)arg_opt, cmd, response);
        arg_opt = cl_arg_opt_get_next(arg_opt);
    }

    return response;
}

static void send_arg_opts_help(cl_arg_opt *arg_opt, cl_cmd_line *cmd, const char *prefix) {
    while (NULL != arg_opt) {
        cl_opt_send_help((cl_opt*)arg_opt, cmd, prefix);
        arg_opt = cl_arg_opt_get_next(arg_opt);
    }
}

cl_cmd_line_opt *cl_cmd_line_opt_get_next(cl_cmd_line_opt *p) {
    if (NULL == p) return NULL;
    return p->next;
}

cl_arg_opt *cl_cmd_line_opt_get_arg_opt(cl_cmd_line_opt *p) {
    if (NULL == p) return NULL;
    return cl_arg_opt_owner_get_arg_opt((cl_arg_opt_owner*)p);
}

cl_switch_opt *cl_cmd_line_opt_get_switch_opt(cl_cmd_line_opt *p) {
    if (NULL == p) return NULL;
    return p->switch_opt;
}

cl_cmd_line_opt *cl_cmd_line_opt_init(cl_cmd_line_opt *p, cl_cmd_line_opt_func *func, void* state, const char *name, const char *desc, cl_arg_opt *arg_opt, cl_switch_opt *switch_opt, cl_cmd_line_opt* next) {
    if (NULL == p) return NULL;
    if (NULL == cl_arg_opt_owner_init((cl_arg_opt_owner*)p, name, desc, CL_TRUE, arg_opt)) return NULL;
    p->func = func;
    p->state = state;
    p->switch_opt = switch_opt;
    p->cmd_validator = cl_cmd_line_validator_get_instance();
    p->next = next;
    return p;
}

cl_cmd_line_opt *cl_cmd_line_opt_create(cl_cmd_line_opt_func *func, void *state, const char *name, const char *desc, cl_arg_opt* arg_opt, cl_switch_opt *switch_opt, cl_cmd_line_opt *next) {
    return cl_cmd_line_opt_init(create_cmd_line_opt(), func, state, name, desc, arg_opt, switch_opt, next);
}

cl_cmd_line_opt *cl_cmd_line_opt_find_by_name(cl_cmd_line_opt* p, const char *name) {
    while (NULL != p) {
        if (0 == strcmp(name, cl_opt_get_name((cl_opt*)p))) {
            return p;
        }
        p = cl_cmd_line_opt_get_next(p);
    }
    return NULL;
}

cl_cmd_line_opt_func *cl_cmd_line_opt_get_func(cl_cmd_line_opt *p) {
    if (NULL == p) return NULL;
    return p->func;
}

void *cl_cmd_line_opt_get_state(cl_cmd_line_opt *p) {
    if (NULL == p) return NULL;
    return p->state;
}

cl_cmd_line_validator *cl_cmd_line_opt_get_cmd_validator(cl_cmd_line_opt *p) {
    if (NULL == p) return NULL;
    return p->cmd_validator;
}

void cl_cmd_line_opt_send_usage(cl_cmd_line_opt *p, cl_cmd_line *cmd) {
    cl_switch_opt *switch_opt;

    /* start the usage string with the name of the command */
    const char *response = cl_cmd_line_format_response(cmd, "%s", cl_opt_get_name((cl_opt*)p));

    /* add each available argument option of the command
       to the usage string */
    response = add_arg_opts_to_usage_response(cl_cmd_line_opt_get_arg_opt(p), cmd, response);

    /* loop through each available switch option */
    switch_opt = cl_cmd_line_opt_get_switch_opt(p);
    while (NULL != switch_opt) {

        /* add it to the usage string */
        response = add_opt_to_usage_response((cl_opt*)switch_opt, cmd, response);

        /* also add each of the switch's argument options to the
           usage string */
        response = add_arg_opts_to_usage_response(cl_switch_opt_get_arg_opt(switch_opt), cmd, response);

        /* go to the next switch */
        switch_opt = cl_switch_opt_get_next(switch_opt);
    }

    /* send the completed usage string */
    cl_cmd_line_respond(cmd, response);
}

void cl_cmd_line_opt_send_help(cl_cmd_line_opt *p, cl_cmd_line *cmd) {
    static const char *single_tab = "\t";
    static const char *double_tab = "\t\t";

    cl_switch_opt *switch_opt;

    cl_cmd_line_opt_send_usage(p, cmd);

    cl_opt_send_help((cl_opt*)p, cmd, "");
    send_arg_opts_help(cl_cmd_line_opt_get_arg_opt(p), cmd, single_tab);    

    switch_opt = cl_cmd_line_opt_get_switch_opt(p);
    while (NULL != switch_opt) {
        cl_opt_send_help((cl_opt*)switch_opt, cmd, single_tab);
        send_arg_opts_help(cl_switch_opt_get_arg_opt(switch_opt), cmd, double_tab);
        switch_opt = cl_switch_opt_get_next(switch_opt);
    }
}

const char *cl_cmd_line_opt_process(cl_cmd_line_opt* p, cl_cmd_line *cmd) {
    cl_cmd_tok *cmd_tok;
    cl_cmd_line_opt *opt;
    cl_cmd_line_opt_func *func;
    const char *cmd_value;
    const char *validation;

    /* get the command token of the command structure */
    cmd_tok = cl_cmd_line_get_cmd_tok(cmd);

    /* get the command option that we'll process by finding
       the one that matches the name of the command */
    cmd_value = cl_tok_get_value((cl_tok*)cmd_tok);
    opt = cl_cmd_line_opt_find_by_name(p, cmd_value);
    if (NULL == opt) return cl_cmd_line_format_response(cmd, "Invalid command: no option found for \"%s\".", cmd_value);

    /* validate the command structure against the option.
       if validation fails, then return the validation result */
    validation = cl_cmd_line_validator_validate(cl_cmd_line_opt_get_cmd_validator(opt), cmd, opt);
    if (validation) return validation;

    /* get the function callback from the command option */
    func = cl_cmd_line_opt_get_func(opt);
    if (NULL == func) return "Invalid function: null pointer.";

    /* invoke the callback */
    return func(cmd, cl_cmd_line_opt_get_state(opt));
}

void cl_cmd_line_opt_destroy(cl_cmd_line_opt *p) {
    cl_memory_manager_destroy(p);
}

void cl_cmd_line_opt_destroy_chain(cl_cmd_line_opt *p) {
    cl_cmd_line_opt *next = p;
    while (NULL != next) {
        p = next;
        next = cl_cmd_line_opt_get_next(p);
        cl_arg_opt_destroy_chain(cl_cmd_line_opt_get_arg_opt(p));
        cl_switch_opt_destroy_chain(cl_cmd_line_opt_get_switch_opt(p));
        cl_cmd_line_opt_destroy(p);
    }
}
