#include <stddef.h>
#include <string.h>
#include "uc_arg_opt_p.h"
#include "uc_arg_opt_owner_p.h"
#include "uc_cmd_line_opt_p.h"
#include "uc_cmd_tok.h"
#include "uc_memory_manager_p.h"
#include "uc_opt_p.h"
#include "uc_switch_opt_p.h"

UC_MEMORY_MANAGER_INIT(uc_cmd_line_opt, UC_CMD_LINE_OPT_COUNT);

static uc_cmd_line_opt *create_cmd_line_opt(void) {
    return uc_memory_manager_create();
}

static const char *add_opt_to_usage_response(uc_opt *opt, uc_cmd_line *cmd, const char *response) {
    static const char *required_format = "%s %s";
    static const char *optional_format = "%s [%s]";
    return uc_cmd_line_format_response(
        cmd,
        uc_opt_is_required(opt) ? required_format : optional_format,
        response,
        uc_opt_get_name(opt)
    );
}

static const char *add_arg_opts_to_usage_response(uc_arg_opt *arg_opt, uc_cmd_line *cmd, const char *response) {
    
    while (NULL != arg_opt) {
        response = add_opt_to_usage_response((uc_opt*)arg_opt, cmd, response);
        arg_opt = uc_arg_opt_get_next(arg_opt);
    }

    return response;
}

static void send_arg_opts_help(uc_arg_opt *arg_opt, uc_cmd_line *cmd, const char *prefix) {
    while (NULL != arg_opt) {
        uc_opt_send_help((uc_opt*)arg_opt, cmd, prefix);
        arg_opt = uc_arg_opt_get_next(arg_opt);
    }
}

uc_cmd_line_opt *uc_cmd_line_opt_get_next(uc_cmd_line_opt *p) {
    if (NULL == p) return NULL;
    return p->next;
}

uc_arg_opt *uc_cmd_line_opt_get_arg_opt(uc_cmd_line_opt *p) {
    if (NULL == p) return NULL;
    return uc_arg_opt_owner_get_arg_opt((uc_arg_opt_owner*)p);
}

uc_switch_opt *uc_cmd_line_opt_get_switch_opt(uc_cmd_line_opt *p) {
    if (NULL == p) return NULL;
    return p->switch_opt;
}

uc_cmd_line_opt *uc_cmd_line_opt_init(uc_cmd_line_opt *p, uc_cmd_line_opt_func *func, void* state, const char *name, const char *desc, uc_arg_opt *arg_opt, uc_switch_opt *switch_opt, uc_cmd_line_opt* next) {
    if (NULL == p) return NULL;
    if (NULL == uc_arg_opt_owner_init((uc_arg_opt_owner*)p, name, desc, UC_TRUE, arg_opt)) return NULL;
    p->func = func;
    p->state = state;
    p->switch_opt = switch_opt;
    p->next = next;
    return p;
}

uc_cmd_line_opt *uc_cmd_line_opt_create(uc_cmd_line_opt_func *func, void *state, const char *name, const char *desc, uc_arg_opt* arg_opt, uc_switch_opt *switch_opt, uc_cmd_line_opt *next) {
    return uc_cmd_line_opt_init(create_cmd_line_opt(), func, state, name, desc, arg_opt, switch_opt, next);
}

uc_cmd_line_opt *uc_cmd_line_opt_find_by_name(uc_cmd_line_opt* p, const char *name) {
    while (NULL != p) {
        if (0 == strcmp(name, uc_opt_get_name((uc_opt*)p))) {
            return p;
        }
        p = uc_cmd_line_opt_get_next(p);
    }
    return NULL;
}

uc_cmd_line_opt_func *uc_cmd_line_opt_get_func(uc_cmd_line_opt *p) {
    if (NULL == p) return NULL;
    return p->func;
}

void *uc_cmd_line_opt_get_state(uc_cmd_line_opt *p) {
    if (NULL == p) return NULL;
    return p->state;
}

void uc_cmd_line_opt_send_usage(uc_cmd_line_opt *p, uc_cmd_line *cmd) {
    uc_switch_opt *switch_opt;

    /* start the usage string with the name of the command */
    const char *response = uc_cmd_line_format_response(cmd, "%s", uc_opt_get_name((uc_opt*)p));

    /* add each available argument option of the command
       to the usage string */
    response = add_arg_opts_to_usage_response(uc_cmd_line_opt_get_arg_opt(p), cmd, response);

    /* loop through each available switch option */
    switch_opt = uc_cmd_line_opt_get_switch_opt(p);
    while (NULL != switch_opt) {

        /* add it to the usage string */
        response = add_opt_to_usage_response((uc_opt*)switch_opt, cmd, response);

        /* also add each of the switch's argument options to the
           usage string */
        response = add_arg_opts_to_usage_response(uc_switch_opt_get_arg_opt(switch_opt), cmd, response);

        /* go to the next switch */
        switch_opt = uc_switch_opt_get_next(switch_opt);
    }

    /* send the completed usage string */
    uc_cmd_line_respond(cmd, response);
}

void uc_cmd_line_opt_send_help(uc_cmd_line_opt *p, uc_cmd_line *cmd) {
    static const char *single_tab = "\t";
    static const char *double_tab = "\t\t";

    uc_switch_opt *switch_opt;

    uc_cmd_line_opt_send_usage(p, cmd);

    uc_opt_send_help((uc_opt*)p, cmd, "");
    send_arg_opts_help(uc_cmd_line_opt_get_arg_opt(p), cmd, single_tab);    

    switch_opt = uc_cmd_line_opt_get_switch_opt(p);
    while (NULL != switch_opt) {
        uc_opt_send_help((uc_opt*)switch_opt, cmd, single_tab);
        send_arg_opts_help(uc_switch_opt_get_arg_opt(switch_opt), cmd, double_tab);
        switch_opt = uc_switch_opt_get_next(switch_opt);
    }
}

const char *uc_cmd_line_opt_format_validation_err(uc_cmd_line_opt *p, uc_cmd_line *cmd) {
    static const char *invalid_switch = "Invalid switch: ";

    uc_cmd_tok *cmd_tok;
    uc_switch_opt *switch_opt, *found_switch_opt, *next_switch_opt;
    uc_switch_tok *switch_tok, *found_switch_tok, *next_switch_tok;
    const char *switch_name, *validation;

    cmd_tok = uc_cmd_line_get_cmd_tok(cmd);
    
    validation = uc_arg_opt_owner_format_validation_err((uc_arg_opt_owner*)p, cmd, uc_cmd_tok_get_arg(cmd_tok), NULL);
    if (NULL != validation) return validation;

    switch_tok = uc_cmd_tok_get_switch(cmd_tok);
    switch_opt = uc_cmd_line_opt_get_switch_opt(p);

    if (NULL == switch_opt) {
        if (NULL != switch_tok) {
            return uc_cmd_line_format_response(cmd, "%sno switch options exist for command \"%s\".", invalid_switch, uc_tok_get_value((uc_tok*)cmd_tok));
        }
        return NULL;
    }

    next_switch_tok = switch_tok;

    while (NULL != next_switch_tok) {
        found_switch_opt = uc_switch_opt_find(switch_opt, uc_tok_get_value((uc_tok*)next_switch_tok));
        if (NULL == found_switch_opt) {
            return uc_cmd_line_format_response(cmd, "%sno option exists for switch \"%s\".", invalid_switch, uc_tok_get_value(next_switch_tok));
        }
        next_switch_tok = uc_switch_tok_get_next(next_switch_tok);
    }

    next_switch_opt = switch_opt;

    while (NULL != next_switch_opt) {
        switch_name = uc_opt_get_name((uc_opt*)next_switch_opt);
        found_switch_tok = uc_switch_tok_find(switch_tok, switch_name);

        if (uc_opt_is_required((uc_opt*)next_switch_opt)) {
            if (NULL == found_switch_tok) {
                return uc_cmd_line_format_response(cmd, "%sthe switch \"%s\" is required.", invalid_switch, switch_name);
            }
        }

        if (NULL != found_switch_tok) {
            validation = uc_switch_opt_format_validation_err(next_switch_opt, cmd, found_switch_tok);
            if (NULL != validation) return validation;
        }

        next_switch_opt = uc_switch_opt_get_next(next_switch_opt);
    }
    
    return NULL;
}

const char *uc_cmd_line_opt_process(uc_cmd_line_opt* p, uc_cmd_line *cmd) {
    uc_cmd_tok *cmd_tok;
    uc_cmd_line_opt *opt;
    uc_cmd_line_opt_func *func;
    const char *cmd_value;
    const char *validation;

    /* get the command token of the command structure */
    cmd_tok = uc_cmd_line_get_cmd_tok(cmd);

    /* get the command option that we'll process by finding
       the one that matches the name of the command */
    cmd_value = uc_tok_get_value((uc_tok*)cmd_tok);
    opt = uc_cmd_line_opt_find_by_name(p, cmd_value);
    if (NULL == opt) return uc_cmd_line_format_response(cmd, "Invalid command: no option found for \"%s\".", cmd_value);

    /* validate the command structure against the option.
       if validation fails, then return the validation result */
    validation = uc_cmd_line_opt_format_validation_err(opt, cmd);
    if (validation) return validation;

    /* get the function callback from the command option */
    func = uc_cmd_line_opt_get_func(opt);
    if (NULL == func) return "Invalid function: null pointer.";

    /* invoke the callback */
    return func(cmd, uc_cmd_line_opt_get_state(opt));
}

void uc_cmd_line_opt_destroy(uc_cmd_line_opt *p) {
    uc_memory_manager_destroy(p);
}

void uc_cmd_line_opt_destroy_chain(uc_cmd_line_opt *p) {
    uc_cmd_line_opt *next = p;
    while (NULL != next) {
        p = next;
        next = uc_cmd_line_opt_get_next(p);
        uc_arg_opt_destroy_chain(uc_cmd_line_opt_get_arg_opt(p));
        uc_switch_opt_destroy_chain(uc_cmd_line_opt_get_switch_opt(p));
        uc_cmd_line_opt_destroy(p);
    }
}
