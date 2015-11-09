#include <stdio.h>
#include <stdlib.h>
#include "ucmd_internal.h"

ucInstance_INIT(ucCmdLine, ucCmdLine_COUNT);

static ucArgTok *get_arg_x(ucArgTokOwner *owner, int arg_index) {
    ucArgTok *arg_tok = NULL;
    if (owner) {
        arg_tok = ucArgTokOwner_get_arg(owner);
        if (arg_tok) {
            arg_tok = ucArgTok_get_index(arg_tok, arg_index);
        }
    }
    return arg_tok;
}

static ucBool get_arg_x_b(ucArgTokOwner *owner, int arg_index, ucBool default_value) {
    ucArgTok *arg_tok = get_arg_x(owner, arg_index);
    if (arg_tok) {
        ucTok_try_parse_boolean(arg_tok, &default_value);
    }
    return default_value;
}

static int get_arg_x_d(ucArgTokOwner *owner, int arg_index, int default_value) {
    ucArgTok *arg_tok = get_arg_x(owner, arg_index);
    if (arg_tok) {
        ucTok_try_parse_integer(arg_tok, &default_value);
    }
    return default_value;
}

static double get_arg_x_f(ucArgTokOwner *owner, int arg_index, double default_value) {
    ucArgTok *arg_tok = get_arg_x(owner, arg_index);
    if (arg_tok) {
        ucTok_try_parse_numeric(arg_tok, &default_value);
    }
    return default_value;
}

ucCmdLine *ucCmdLine_init(ucCmdLine *p) {
    assert(p);
    p->cmd_tok = NULL;
    p->command_acknowledgment = NULL;
    p->transmit = NULL;
    p->transmit_state = NULL;
    p->is_quiet = ucBool_false;
    p->is_canceled = NULL;
    p->is_canceled_state = NULL;
    p->handle_invalid_command = NULL;
    p->handle_invalid_command_state = NULL;
    p->response_terminator = NULL;
    return p;
}

ucCmdLine *ucCmdLine_create(void) {
    return ucCmdLine_init(ucInstance_create());
}

void ucCmdLine_destroy(ucCmdLine *p) {
    ucInstance_destroy(p);
}

ucCmdTok *ucCmdLine_get_cmd_tok(ucCmdLine *p) {
    assert(p);
    return p->cmd_tok;
}

void ucCmdLine_set_cmd_tok(ucCmdLine *p, ucCmdTok *value) {
    assert(p);
    p->cmd_tok = value;
}

const char *ucCmdLine_format_response_va(ucCmdLine *p, const char *format, va_list arg_list) {
    assert(p);
	/* TODO: Buffer and copy were added because "usage" uses the command's response in the arg list.
	   TODO: There's probably a better-performing way to handle that, though. */
	vsnprintf(p->response_buffer, sizeof(p->response_buffer) - 1, format, arg_list);
	strcpy(p->response, p->response_buffer);
    return p->response;
}

const char *ucCmdLine_format_response(ucCmdLine *p, const char *format, ...) {
    va_list arg_list;
    const char *response;

    va_start(arg_list, format);
    response = ucCmdLine_format_response_va(p, format, arg_list);
    va_end(arg_list);

    return response;
}

void ucCmdLine_respond(ucCmdLine *p, const char *response) {
    assert(p);
    if (p->is_quiet) return;
    if (p->transmit) {
        p->transmit(response, p->transmit_state);
    }
}

void ucCmdLine_set_transmit(ucCmdLine *p, ucCmdLine_TransmitFunc *value) {
    assert(p);
    p->transmit = value;
}

ucCmdLine_TransmitFunc *ucCmdLine_get_transmit(ucCmdLine *p) {
    assert(p);
    return p->transmit;
}

ucBool ucCmdLine_is_canceled(ucCmdLine *p) {
    assert(p);
    if (p->is_canceled) {
        return p->is_canceled(p->is_canceled_state);
    }
    return ucBool_false;
}

void ucCmdLine_set_is_canceled(ucCmdLine *p, ucCmdLine_IsCanceledFunc *value) {
    assert(p);
    p->is_canceled = value;
}

ucCmdLine_IsCanceledFunc *ucCmdLine_get_is_canceled(ucCmdLine *p) {
    assert(p);
    return p->is_canceled;
}

void *ucCmdLine_get_transmit_state(ucCmdLine *p) {
    assert(p);
    return p->transmit_state;
}

void ucCmdLine_set_transmit_state(ucCmdLine *p, void *value) {
    assert(p);
    p->transmit_state = value;
}

void *ucCmdLine_get_is_canceled_state(ucCmdLine *p) {
    assert(p);
    return p->is_canceled_state;
}

void ucCmdLine_set_is_canceled_state(ucCmdLine *p, void *value) {
    assert(p);
    p->is_canceled_state = value;
}

void ucCmdLine_set_is_quiet(ucCmdLine *p, ucBool value) {
    assert(p);
    p->is_quiet = value;
}

ucBool ucCmdLine_get_is_quiet(ucCmdLine *p) {
    assert(p);
    return p->is_quiet;
}

void ucCmdLine_set_handle_invalid_command(ucCmdLine *p, ucCmdLine_HandleInvalidCommandFunc *value) {
    assert(p);
    p->handle_invalid_command = value;
}

ucCmdLine_HandleInvalidCommandFunc *ucCmdLine_get_handle_invalid_command(ucCmdLine *p) {
    assert(p);
    return p->handle_invalid_command;
}

void ucCmdLine_set_handle_invalid_command_state(ucCmdLine *p, void *value) {
    assert(p);
    p->handle_invalid_command_state = value;
}

void *ucCmdLine_get_handle_invalid_command_state(ucCmdLine *p) {
    assert(p);
    return p->handle_invalid_command_state;
}

ucBool ucCmdLine_handle_invalid_command(ucCmdLine *p, const char *invalid_command) {
    assert(p);
    if (p->handle_invalid_command) {
        return p->handle_invalid_command(invalid_command, p->handle_invalid_command_state);
    }
    return ucBool_false;
}

size_t ucCmdLine_get_response_size_max(ucCmdLine *p) {
    assert(p);
    return sizeof(p->response);
}

void ucCmdLine_set_response_terminator(ucCmdLine *p, const char *value) {
    assert(p);
    p->response_terminator = value;
}

const char *ucCmdLine_get_response_terminator(ucCmdLine *p) {
    assert(p);
    return p->response_terminator;
}

void ucCmdLine_set_command_acknowledgment(ucCmdLine *p, const char *value) {
    assert(p);
    p->command_acknowledgment = value;
}

const char *ucCmdLine_get_command_acknowledgment(ucCmdLine *p) {
    assert(p);
    return p->command_acknowledgment;
}

void ucCmdLine_acknowledge_command(ucCmdLine *p) {
    const char *command_acknowledgment = ucCmdLine_get_command_acknowledgment(p);
    if (command_acknowledgment) {
        ucCmdLine_respond(p, command_acknowledgment);
    }
}

void ucCmdLine_terminate_response(ucCmdLine *p) {
    const char *response_terminator = ucCmdLine_get_response_terminator(p);
    if (response_terminator) {
        ucCmdLine_respond(p, response_terminator);
    }
}

ucArgTok *ucCmdLine_get_arg(ucCmdLine *p) {
    ucCmdTok *cmd_tok = ucCmdLine_get_cmd_tok(p);
    return cmd_tok
        ? ucCmdTok_get_arg(cmd_tok)
        : NULL;
}

ucSwitchTok *ucCmdLine_get_switch(ucCmdLine *p) {
    ucCmdTok *cmd_tok = ucCmdLine_get_cmd_tok(p);
    return cmd_tok
        ? ucCmdTok_get_switch(cmd_tok)
        : NULL;
}

ucSwitchTok *ucCmdLine_get_switch_x(ucCmdLine *p, int switch_index) {
    ucSwitchTok *switch_tok = ucCmdLine_get_switch(p);
    if (switch_tok) {
        switch_tok = ucSwitchTok_get_index(switch_tok, switch_index);
    }
    return switch_tok;
}

ucSwitchTok *ucCmdLine_find_switch(ucCmdLine *p, const char *switch_name) {
    ucSwitchTok *switch_tok = ucCmdLine_get_switch(p);
    return switch_tok
        ? ucSwitchTok_find(switch_tok, switch_name)
        : NULL;
}

ucArgTok *ucCmdLine_get_switch_arg(ucCmdLine *p, const char *switch_name) {
    return get_arg_x(ucCmdLine_find_switch(p, switch_name), 0);
}

ucArgTok *ucCmdLine_get_switch_arg_x(ucCmdLine *p, const char *switch_name, int arg_index) {
    return get_arg_x(ucCmdLine_find_switch(p, switch_name), arg_index);
}

int ucCmdLine_get_switch_arg_d(ucCmdLine *p, const char *switch_name, int default_value) {
    return get_arg_x_d(ucCmdLine_find_switch(p, switch_name), 0, default_value);
}

int ucCmdLine_get_switch_arg_x_d(ucCmdLine *p, const char *switch_name, int arg_index, int default_value) {
    return get_arg_x_d(ucCmdLine_find_switch(p, switch_name), arg_index, default_value);
}

double ucCmdLine_get_switch_arg_f(ucCmdLine *p, const char *switch_name, double default_value) {
    return get_arg_x_f(ucCmdLine_find_switch(p, switch_name), 0, default_value);
}

double ucCmdLine_get_switch_arg_x_f(ucCmdLine *p, const char *switch_name, int arg_index, double default_value) {
    return get_arg_x_f(ucCmdLine_find_switch(p, switch_name), arg_index, default_value);
}

ucBool ucCmdLine_get_switch_arg_b(ucCmdLine *p, const char *switch_name, ucBool default_value) {
    return get_arg_x_b(ucCmdLine_find_switch(p, switch_name), 0, default_value);
}

ucBool ucCmdLine_get_switch_arg_x_b(ucCmdLine *p, const char *switch_name, int arg_index, ucBool default_value) {
    return get_arg_x_b(ucCmdLine_find_switch(p, switch_name), arg_index, default_value);
}

ucArgTok *ucCmdLine_get_arg_x(ucCmdLine *p, int arg_index) {
    return get_arg_x(ucCmdLine_get_cmd_tok(p), arg_index);
}

ucBool ucCmdLine_get_arg_b(ucCmdLine *p, ucBool default_value) {
    return get_arg_x_b(ucCmdLine_get_cmd_tok(p), 0, default_value);
}

int ucCmdLine_get_arg_d(ucCmdLine *p, int default_value) {
    return get_arg_x_d(ucCmdLine_get_cmd_tok(p), 0, default_value);
}

double ucCmdLine_get_arg_f(ucCmdLine *p, double default_value) {
    return get_arg_x_f(ucCmdLine_get_cmd_tok(p), 0, default_value);
}

ucBool ucCmdLine_get_arg_x_b(ucCmdLine *p, int arg_index, ucBool default_value) {
    return get_arg_x_b(ucCmdLine_get_cmd_tok(p), arg_index, default_value);
}

int ucCmdLine_get_arg_x_d(ucCmdLine *p, int arg_index, int default_value) {
    return get_arg_x_d(ucCmdLine_get_cmd_tok(p), arg_index, default_value);
}

double ucCmdLine_get_arg_x_f(ucCmdLine *p, int arg_index, double default_value) {
    return get_arg_x_f(ucCmdLine_get_cmd_tok(p), arg_index, default_value);
}
