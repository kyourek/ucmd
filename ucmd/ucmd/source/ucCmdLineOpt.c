#include <stddef.h>
#include <string.h>
#include "ucArgOpt_p.h"
#include "ucArgOptOwner_p.h"
#include "ucCmdLineOpt_p.h"
#include "ucCmdTok.h"
#include "ucMemoryManager_p.h"
#include "ucOpt_p.h"
#include "ucSwitchOpt_p.h"

ucMemoryManager_INIT(ucCmdLineOpt, UC_CMD_LINE_OPT_COUNT);

static ucCmdLineOpt *create_cmd_line_opt(void) {
    return ucMemoryManager_create();
}

static const char *add_opt_to_usage_response(ucOpt *opt, ucCmdLine *cmd, const char *response) {
    static const char *required_format = "%s %s";
    static const char *optional_format = "%s [%s]";
    return ucCmdLine_format_response(
        cmd,
        ucOpt_is_required(opt) ? required_format : optional_format,
        response,
        ucOpt_get_name(opt)
    );
}

static const char *add_arg_opts_to_usage_response(ucArgOpt *arg_opt, ucCmdLine *cmd, const char *response) {
    
    while (NULL != arg_opt) {
        response = add_opt_to_usage_response((ucOpt*)arg_opt, cmd, response);
        arg_opt = ucArgOpt_get_next(arg_opt);
    }

    return response;
}

static void send_arg_opts_help(ucArgOpt *arg_opt, ucCmdLine *cmd, const char *prefix) {
    while (NULL != arg_opt) {
        ucOpt_send_help((ucOpt*)arg_opt, cmd, prefix);
        arg_opt = ucArgOpt_get_next(arg_opt);
    }
}

ucCmdLineOpt *ucCmdLineOpt_get_next(ucCmdLineOpt *p) {
    if (NULL == p) return NULL;
    return p->next;
}

ucArgOpt *ucCmdLineOpt_get_arg_opt(ucCmdLineOpt *p) {
    if (NULL == p) return NULL;
    return ucArgOptOwner_get_arg_opt((ucArgOptOwner*)p);
}

ucSwitchOpt *ucCmdLineOpt_get_switch_opt(ucCmdLineOpt *p) {
    if (NULL == p) return NULL;
    return p->switch_opt;
}

ucCmdLineOpt *ucCmdLineOpt_init(ucCmdLineOpt *p, ucCmdLineOpt_func *func, void* state, const char *name, const char *desc, ucArgOpt *arg_opt, ucSwitchOpt *switch_opt, ucCmdLineOpt* next) {
    if (NULL == p) return NULL;
    if (NULL == ucArgOptOwner_init((ucArgOptOwner*)p, name, desc, ucBool_true, arg_opt)) return NULL;
    p->func = func;
    p->state = state;
    p->switch_opt = switch_opt;
    p->next = next;
    return p;
}

ucCmdLineOpt *ucCmdLineOpt_create(ucCmdLineOpt_func *func, void *state, const char *name, const char *desc, ucArgOpt* arg_opt, ucSwitchOpt *switch_opt, ucCmdLineOpt *next) {
    return ucCmdLineOpt_init(create_cmd_line_opt(), func, state, name, desc, arg_opt, switch_opt, next);
}

ucCmdLineOpt *ucCmdLineOpt_find_by_name(ucCmdLineOpt* p, const char *name) {
    while (NULL != p) {
        if (0 == strcmp(name, ucOpt_get_name((ucOpt*)p))) {
            return p;
        }
        p = ucCmdLineOpt_get_next(p);
    }
    return NULL;
}

ucCmdLineOpt_func *ucCmdLineOpt_get_func(ucCmdLineOpt *p) {
    if (NULL == p) return NULL;
    return p->func;
}

void *ucCmdLineOpt_get_state(ucCmdLineOpt *p) {
    if (NULL == p) return NULL;
    return p->state;
}

void ucCmdLineOpt_send_usage(ucCmdLineOpt *p, ucCmdLine *cmd) {
    ucSwitchOpt *switch_opt;

    /* start the usage string with the name of the command */
    const char *response = ucCmdLine_format_response(cmd, "%s", ucOpt_get_name((ucOpt*)p));

    /* add each available argument option of the command
       to the usage string */
    response = add_arg_opts_to_usage_response(ucCmdLineOpt_get_arg_opt(p), cmd, response);

    /* loop through each available switch option */
    switch_opt = ucCmdLineOpt_get_switch_opt(p);
    while (NULL != switch_opt) {

        /* add it to the usage string */
        response = add_opt_to_usage_response((ucOpt*)switch_opt, cmd, response);

        /* also add each of the switch's argument options to the
           usage string */
        response = add_arg_opts_to_usage_response(ucSwitchOpt_get_arg_opt(switch_opt), cmd, response);

        /* go to the next switch */
        switch_opt = ucSwitchOpt_get_next(switch_opt);
    }

    /* send the completed usage string */
    ucCmdLine_respond(cmd, response);
}

void ucCmdLineOpt_send_help(ucCmdLineOpt *p, ucCmdLine *cmd) {
    static const char *single_tab = "\t";
    static const char *double_tab = "\t\t";

    ucSwitchOpt *switch_opt;

    ucCmdLineOpt_send_usage(p, cmd);

    ucOpt_send_help((ucOpt*)p, cmd, "");
    send_arg_opts_help(ucCmdLineOpt_get_arg_opt(p), cmd, single_tab);    

    switch_opt = ucCmdLineOpt_get_switch_opt(p);
    while (NULL != switch_opt) {
        ucOpt_send_help((ucOpt*)switch_opt, cmd, single_tab);
        send_arg_opts_help(ucSwitchOpt_get_arg_opt(switch_opt), cmd, double_tab);
        switch_opt = ucSwitchOpt_get_next(switch_opt);
    }
}

const char *ucCmdLineOpt_format_validation_err(ucCmdLineOpt *p, ucCmdLine *cmd) {
    static const char *invalid_switch = "Invalid switch: ";

    ucCmdTok *cmd_tok;
    ucSwitchOpt *switch_opt, *found_switch_opt, *next_switch_opt;
    ucSwitchTok *switch_tok, *found_switch_tok, *next_switch_tok;
    const char *switch_name, *validation;

    cmd_tok = ucCmdLine_get_cmd_tok(cmd);
    
    validation = ucArgOptOwner_format_validation_err((ucArgOptOwner*)p, cmd, ucCmdTok_get_arg(cmd_tok), NULL);
    if (NULL != validation) return validation;

    switch_tok = ucCmdTok_get_switch(cmd_tok);
    switch_opt = ucCmdLineOpt_get_switch_opt(p);

    if (NULL == switch_opt) {
        if (NULL != switch_tok) {
            return ucCmdLine_format_response(cmd, "%sno switch options exist for command \"%s\".", invalid_switch, ucTok_get_value((ucTok*)cmd_tok));
        }
        return NULL;
    }

    next_switch_tok = switch_tok;

    while (NULL != next_switch_tok) {
        found_switch_opt = ucSwitchOpt_find(switch_opt, ucTok_get_value((ucTok*)next_switch_tok));
        if (NULL == found_switch_opt) {
            return ucCmdLine_format_response(cmd, "%sno option exists for switch \"%s\".", invalid_switch, ucTok_get_value(next_switch_tok));
        }
        next_switch_tok = ucSwitchTok_get_next(next_switch_tok);
    }

    next_switch_opt = switch_opt;

    while (NULL != next_switch_opt) {
        switch_name = ucOpt_get_name((ucOpt*)next_switch_opt);
        found_switch_tok = ucSwitchTok_find(switch_tok, switch_name);

        if (ucOpt_is_required((ucOpt*)next_switch_opt)) {
            if (NULL == found_switch_tok) {
                return ucCmdLine_format_response(cmd, "%sthe switch \"%s\" is required.", invalid_switch, switch_name);
            }
        }

        if (NULL != found_switch_tok) {
            validation = ucSwitchOpt_format_validation_err(next_switch_opt, cmd, found_switch_tok);
            if (NULL != validation) return validation;
        }

        next_switch_opt = ucSwitchOpt_get_next(next_switch_opt);
    }
    
    return NULL;
}

const char *ucCmdLineOpt_process(ucCmdLineOpt* p, ucCmdLine *cmd) {
    ucCmdTok *cmd_tok;
    ucCmdLineOpt *opt;
    ucCmdLineOpt_func *func;
    const char *cmd_value;
    const char *validation;

    /* get the command token of the command structure */
    cmd_tok = ucCmdLine_get_cmd_tok(cmd);

    /* get the command option that we'll process by finding
       the one that matches the name of the command */
    cmd_value = ucTok_get_value((ucTok*)cmd_tok);
    opt = ucCmdLineOpt_find_by_name(p, cmd_value);
    if (NULL == opt) return ucCmdLine_format_response(cmd, "Invalid command: no option found for \"%s\".", cmd_value);

    /* validate the command structure against the option.
       if validation fails, then return the validation result */
    validation = ucCmdLineOpt_format_validation_err(opt, cmd);
    if (validation) return validation;

    /* get the function callback from the command option */
    func = ucCmdLineOpt_get_func(opt);
    if (NULL == func) return "Invalid function: null pointer.";

    /* invoke the callback */
    return func(cmd, ucCmdLineOpt_get_state(opt));
}

void ucCmdLineOpt_destroy(ucCmdLineOpt *p) {
    ucMemoryManager_destroy(p);
}

void ucCmdLineOpt_destroy_chain(ucCmdLineOpt *p) {
    ucCmdLineOpt *next = p;
    while (NULL != next) {
        p = next;
        next = ucCmdLineOpt_get_next(p);
        ucArgOpt_destroy_chain(ucCmdLineOpt_get_arg_opt(p));
        ucSwitchOpt_destroy_chain(ucCmdLineOpt_get_switch_opt(p));
        ucCmdLineOpt_destroy(p);
    }
}
