#include "ucmd_internal.h"

ucInstance_INIT(ucCmdOpt, ucCmdOpt_COUNT);

static const char *add_opt_to_usage_response(ucOpt *opt, ucCmd *cmd, const char *response) {
    static const char *required_format = "%s %s";
    static const char *optional_format = "%s [%s]";
    const char *format = ucOpt_is_required(opt) 
        ? required_format 
        : optional_format;
    return ucCmd_format_response(
        cmd,
        format,
        response,
        ucOpt_get_name(opt)
    );
}

static const char *add_arg_opts_to_usage_response(ucArgOpt *arg_opt, ucCmd *cmd, const char *response) {
    while (arg_opt) {
        response = add_opt_to_usage_response((ucOpt*)arg_opt, cmd, response);
        arg_opt = ucArgOpt_get_next(arg_opt);
    }
    return response;
}

static void send_arg_opts_help(ucArgOpt *arg_opt, ucCmd *cmd, const char *prefix) {
    while (arg_opt) {
        ucOpt_send_help((ucOpt*)arg_opt, cmd, prefix);
        arg_opt = ucArgOpt_get_next(arg_opt);
    }
}

ucCmdOpt *ucCmdOpt_get_next(ucCmdOpt *p) {
    assert(p);
    return p->next;
}

ucArgOpt *ucCmdOpt_get_arg_opt(ucCmdOpt *p) {
    assert(p);
    return ucArgOptOwner_get_arg_opt((ucArgOptOwner*)p);
}

ucSwitchOpt *ucCmdOpt_get_switch_opt(ucCmdOpt *p) {
    assert(p);
    return p->switch_opt;
}

ucCmdOpt *ucCmdOpt_init(ucCmdOpt *p, ucCmdOpt_WorkFunc *work, void* state, const char *name, const char *desc, ucArgOpt *arg_opt, ucSwitchOpt *switch_opt, ucCmdOpt* next) {
    assert(p);
    assert(ucArgOptOwner_init((ucArgOptOwner*)p, name, desc, ucBool_true, arg_opt));
    p->work = work;
    p->state = state;
    p->switch_opt = switch_opt;
    p->next = next;
    return p;
}

ucCmdOpt *ucCmdOpt_create(ucCmdOpt_WorkFunc *func, void *state, const char *name, const char *desc, ucArgOpt* arg_opt, ucSwitchOpt *switch_opt, ucCmdOpt *next) {
    return ucCmdOpt_init(ucInstance_create(), func, state, name, desc, arg_opt, switch_opt, next);
}

ucCmdOpt *ucCmdOpt_find_by_name(ucCmdOpt* p, const char *name) {
    assert(p);
    while (p) {
        if (uc_STR_EQ(name, ucOpt_get_name((ucOpt*)p))) {
            return p;
        }
        p = ucCmdOpt_get_next(p);
    }
    return NULL;
}

ucCmdOpt_WorkFunc *ucCmdOpt_get_work(ucCmdOpt *p) {
    assert(p);
    return p->work;
}

void *ucCmdOpt_get_state(ucCmdOpt *p) {
    assert(p);
    return p->state;
}

void ucCmdOpt_send_usage(ucCmdOpt *p, ucCmd *cmd) {
    ucSwitchOpt *switch_opt;

    /* Start the usage string with the name of the command. */
    const char *response = ucCmd_format_response(cmd, "%s", ucOpt_get_name((ucOpt*)p));

    /* Add each available argument option of the command to
    the usage string. */
    response = add_arg_opts_to_usage_response(ucCmdOpt_get_arg_opt(p), cmd, response);

    /* Loop through each available switch option. */
    switch_opt = ucCmdOpt_get_switch_opt(p);
    while (switch_opt) {

        /* Add it to the usage string. */
        response = add_opt_to_usage_response((ucOpt*)switch_opt, cmd, response);

        /* Also add each of the switch's argument options to
        the usage string. */
        response = add_arg_opts_to_usage_response(ucSwitchOpt_get_arg_opt(switch_opt), cmd, response);

        /* Go to the next switch. */
        switch_opt = ucSwitchOpt_get_next(switch_opt);
    }

    /* Send the completed usage string. */
    ucCmd_respond(cmd, response);
}

void ucCmdOpt_send_help(ucCmdOpt *p, ucCmd *cmd) {
    static const char *single_tab = "\t";
    static const char *double_tab = "\t" "\t";
    ucSwitchOpt *switch_opt;

    ucCmdOpt_send_usage(p, cmd);

    ucOpt_send_help((ucOpt*)p, cmd, "");
    send_arg_opts_help(ucCmdOpt_get_arg_opt(p), cmd, single_tab);    

    switch_opt = ucCmdOpt_get_switch_opt(p);
    while (switch_opt) {
        ucOpt_send_help((ucOpt*)switch_opt, cmd, single_tab);
        send_arg_opts_help(ucSwitchOpt_get_arg_opt(switch_opt), cmd, double_tab);
        switch_opt = ucSwitchOpt_get_next(switch_opt);
    }
}

const char *ucCmdOpt_format_validation_err(ucCmdOpt *p, ucCmd *cmd) {
    ucArgTok *arg_tok;
    ucCmdTok *cmd_tok;
    ucSwitchOpt *switch_opt, *found_switch_opt, *next_switch_opt;
    ucSwitchTok *switch_tok, *found_switch_tok, *next_switch_tok;
    const char *switch_name, *validation;

    cmd_tok = ucCmd_get_cmd_tok(cmd);
    arg_tok = ucCmdTok_get_arg(cmd_tok);

    validation = ucArgOptOwner_format_validation_err((ucArgOptOwner*)p, cmd, arg_tok, NULL);
    if (validation) return validation;

    switch_tok = ucCmdTok_get_switch(cmd_tok);
    switch_opt = ucCmdOpt_get_switch_opt(p);

    if (!switch_opt) {
        if (switch_tok) {
            return ucCmd_format_response(cmd, ucOpt_INVALID "No switch options exist for command \"%s\".", cmd_tok);
        }
        return NULL;
    }

    next_switch_tok = switch_tok;

    while (next_switch_tok) {
        found_switch_opt = ucSwitchOpt_find(switch_opt, next_switch_tok);
        if (!found_switch_opt) {
            return ucCmd_format_response(cmd, ucOpt_INVALID "No option exists for switch \"%s\".", next_switch_tok);
        }
        next_switch_tok = ucSwitchTok_get_next(next_switch_tok);
    }

    next_switch_opt = switch_opt;

    while (next_switch_opt) {
        switch_name = ucOpt_get_name((ucOpt*)next_switch_opt);
        found_switch_tok = switch_tok 
            ? ucSwitchTok_find(switch_tok, switch_name) 
            : NULL;

        if (ucOpt_is_required((ucOpt*)next_switch_opt)) {
            if (!found_switch_tok) {
                return ucCmd_format_response(cmd, ucOpt_INVALID "the switch \"%s\" is required.", switch_name);
            }
        }

        if (found_switch_tok) {
            validation = ucSwitchOpt_format_validation_err(next_switch_opt, cmd, found_switch_tok);
            if (validation) return validation;
        }

        next_switch_opt = ucSwitchOpt_get_next(next_switch_opt);
    }
    
    return NULL;
}

static const char *internal_process(ucCmdOpt *p, ucCmd *cmd, ucBool *invalid_command_handled) {
    ucCmdTok *cmd_tok;
    ucCmdOpt *opt;
    ucCmdOpt_WorkFunc *work;
    const char *cmd_value;
    const char *validation;

    /* Get the command token of the command structure. */
    cmd_tok = ucCmd_get_cmd_tok(cmd);

    /* Get the command option that we'll process by finding
    the one that matches the name of the command. */
    cmd_value = cmd_tok;
    opt = ucCmdOpt_find_by_name(p, cmd_value);
    if (!opt) {

        /* The command is invalid (meaning it doesn't exist).
        Try to handle it. */
        *invalid_command_handled = ucCmd_handle_invalid_command(cmd, cmd_value);

        /* If the command was handled, then we don't return an error. */
        if (*invalid_command_handled) {
            return NULL;
        }
    }

    /* Send an indication that the command was received. */
    ucCmd_acknowledge_command(cmd);

    /* Check to see if the command is unknown. */
    if (!opt) return ucCmd_format_response(
        cmd, 
        ucOpt_INVALID "No command option found for \"%s\"", 
        cmd_value);

    /* Validate the command structure against the option.
    If validation fails, then return the validation result. */
    validation = ucCmdOpt_format_validation_err(opt, cmd);
    if (validation) return validation;

    /* Get the function callback from the command option
    and make sure it exists. */
    work = ucCmdOpt_get_work(opt);
    if (!work) return ucCmd_format_response(
        cmd,
        "No work found for command \"%s\"",
        cmd_value);

    /* Invoke the callback and return the result. */
    return work(cmd, ucCmdOpt_get_state(opt));
}

const char *ucCmdOpt_process(ucCmdOpt* p, ucCmd *cmd) {
    ucBool invalid_command_handled = ucBool_false;
    const char *response = internal_process(p, cmd, &invalid_command_handled);
    
    if (invalid_command_handled) {
        return response;
    }

    if (response) {
        ucCmd_respond(cmd, response);
    }

    ucCmd_terminate_response(cmd);

    return response;
}

void ucCmdOpt_destroy(ucCmdOpt *p) {
    ucInstance_destroy(p);
}

void ucCmdOpt_destroy_chain(ucCmdOpt *p) {
    ucArgOpt *arg;
    ucSwitchOpt *sw;
    ucCmdOpt *next;
    assert(p);
    next = p;
    while (next) {
        p = next;
        next = ucCmdOpt_get_next(p);
        
        arg = ucCmdOpt_get_arg_opt(p);
        if (arg) {
            ucArgOpt_destroy_chain(arg);
        }

        sw = ucCmdOpt_get_switch_opt(p);
        if (sw) {
            ucSwitchOpt_destroy_chain(sw);
        }

        ucCmdOpt_destroy(p);
    }
}
