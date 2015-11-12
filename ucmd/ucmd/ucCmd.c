#include <stdio.h>
#include <stdlib.h>
#include "ucmd_internal.h"

ucInstance_INIT(ucCmd, ucCmd_COUNT)

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

static ucCmd *ucCmd_init(ucCmd *p, ucParser *parser) {
    assert(p);
    p->command = NULL;
    p->command_acknowledgment = NULL;
    p->parser = parser;
    p->receive = NULL;
    p->receive_state = NULL;
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

ucCmd *ucCmd_create(void) {
    return ucCmd_init(ucInstance_create(), ucParser_create());
}

void ucCmd_destroy(ucCmd *p) {
    if (p) {
        if (p->parser) {
            ucParser_destroy(p->parser);
        }
        ucInstance_destroy(p);
    }
}

char *ucCmd_get_command_buffer(ucCmd *p) {
    assert(p);
    return p->command_buffer;
}

int ucCmd_get_command_length_max(ucCmd *p) {
    assert(p);
    return (sizeof(p->command_buffer) / sizeof(p->command_buffer[0])) - 1;
}

ucParser *ucCmd_get_parser(ucCmd *p) {
    assert(p);
    return p->parser;
}

ucCmdTok *ucCmd_parse(ucCmd *p, char *command) {
    ucParser *parser = ucCmd_get_parser(p);
    ucCmdTok *cmd_tok = ucParser_parse(parser, command);
    ucCmd_set_command(p, cmd_tok);
    return ucCmd_get_command(p);
}

ucCmdTok *ucCmd_parse_const(ucCmd *p, const char *command) {
    int length_max = ucCmd_get_command_length_max(p);
    char *buffer = ucCmd_get_command_buffer(p);
    strncpy(buffer, command, (size_t)length_max);
    buffer[length_max - 1] = '\0';
    return ucCmd_parse(p, buffer);
}

ucCmdTok *ucCmd_get_command(ucCmd *p) {
    assert(p);
    return p->command;
}

void ucCmd_set_command(ucCmd *p, ucCmdTok *value) {
    assert(p);
    p->command = value;
}

const char *ucCmd_format_response_va(ucCmd *p, const char *format, va_list arg_list) {
    int r;
    size_t n;
    assert(p);
    /* TODO: Buffer and copy were added because "usage" uses the command's response in the arg list.
    There's probably a better-performing way to handle that, though. */
    n = sizeof(p->response_buffer);
	r = vsnprintf(p->response_buffer, n, format, arg_list);
	if (r < 0) {
	    /* TODO: An encoding error occurred. */
	}
	else if ((size_t)r >= n) {
        /* TODO: Characters were discarded and not stored. */
	}
    n = sizeof(p->response);
    strncpy(p->response, p->response_buffer, n - 1);
    p->response[n - 1] = '\0';
    return p->response;
}

const char *ucCmd_format_response(ucCmd *p, const char *format, ...) {
    va_list arg_list;
    const char *response;

    va_start(arg_list, format);
    response = ucCmd_format_response_va(p, format, arg_list);
    va_end(arg_list);

    return response;
}

void ucCmd_respond(ucCmd *p, const char *response) {
    assert(p);
    if (p->is_quiet) return;
    if (p->transmit) {
        p->transmit(response, p->transmit_state);
    }
}

void ucCmd_set_transmit(ucCmd *p, ucCmd_TransmitFunc *value) {
    assert(p);
    p->transmit = value;
}

void ucCmd_set_transmit_state(ucCmd *p, void *value) {
    assert(p);
    p->transmit_state = value;
}

void ucCmd_set_is_canceled(ucCmd *p, ucCmd_IsCanceledFunc *value) {
    assert(p);
    p->is_canceled = value;
}

void ucCmd_set_is_canceled_state(ucCmd *p, void *value) {
    assert(p);
    p->is_canceled_state = value;
}

ucBool ucCmd_is_canceled(ucCmd *p) {
    assert(p);
    if (p->is_canceled) {
        return p->is_canceled(p->is_canceled_state);
    }
    return ucBool_false;
}

void ucCmd_set_is_quiet(ucCmd *p, ucBool value) {
    assert(p);
    p->is_quiet = value;
}

ucBool ucCmd_get_is_quiet(ucCmd *p) {
    assert(p);
    return p->is_quiet;
}

void ucCmd_set_handle_invalid_command(ucCmd *p, ucCmd_HandleInvalidCommandFunc *value) {
    assert(p);
    p->handle_invalid_command = value;
}

void ucCmd_set_handle_invalid_command_state(ucCmd *p, void *value) {
    assert(p);
    p->handle_invalid_command_state = value;
}

ucBool ucCmd_handle_invalid_command(ucCmd *p, const char *invalid_command) {
    assert(p);
    if (p->handle_invalid_command) {
        return p->handle_invalid_command(invalid_command, p->handle_invalid_command_state);
    }
    return ucBool_false;
}

void ucCmd_set_receive(ucCmd *p, ucCmd_ReceiveFunc *value) {
    assert(p);
    p->receive = value;
}

void ucCmd_set_receive_state(ucCmd *p, void *value) {
    assert(p);
    p->receive_state = value;
}

ucCmdTok *ucCmd_listen(ucCmd *p) {
    size_t i, len;
    assert(p);

    len = sizeof(p->command_buffer) / sizeof(p->command_buffer[0]);
    for (i = 0; i < len; p->command_buffer[i++] = '\0');

    if (p->receive) {
        p->receive(p->command_buffer, len - 1, p->receive_state);
    }

    return ucCmd_parse(p, p->command_buffer);
}

int ucCmd_get_response_length_max(ucCmd *p) {
    assert(p);
    return sizeof(p->response) / sizeof(p->response[0]);
}

void ucCmd_set_response_terminator(ucCmd *p, const char *value) {
    assert(p);
    p->response_terminator = value;
}

const char *ucCmd_get_response_terminator(ucCmd *p) {
    assert(p);
    return p->response_terminator;
}

void ucCmd_set_command_acknowledgment(ucCmd *p, const char *value) {
    assert(p);
    p->command_acknowledgment = value;
}

const char *ucCmd_get_command_acknowledgment(ucCmd *p) {
    assert(p);
    return p->command_acknowledgment;
}

void ucCmd_acknowledge_command(ucCmd *p) {
    const char *command_acknowledgment = ucCmd_get_command_acknowledgment(p);
    if (command_acknowledgment) {
        ucCmd_respond(p, command_acknowledgment);
    }
}

void ucCmd_terminate_response(ucCmd *p) {
    const char *response_terminator = ucCmd_get_response_terminator(p);
    if (response_terminator) {
        ucCmd_respond(p, response_terminator);
    }
}

ucArgTok *ucCmd_get_arg(ucCmd *p) {
    ucCmdTok *cmd_tok = ucCmd_get_command(p);
    return cmd_tok
        ? ucCmdTok_get_arg(cmd_tok)
        : NULL;
}

ucSwitchTok *ucCmd_get_switch(ucCmd *p) {
    ucCmdTok *cmd_tok = ucCmd_get_command(p);
    return cmd_tok
        ? ucCmdTok_get_switch(cmd_tok)
        : NULL;
}

ucSwitchTok *ucCmd_get_switch_x(ucCmd *p, int switch_index) {
    ucSwitchTok *switch_tok = ucCmd_get_switch(p);
    if (switch_tok) {
        switch_tok = ucSwitchTok_get_index(switch_tok, switch_index);
    }
    return switch_tok;
}

ucSwitchTok *ucCmd_find_switch(ucCmd *p, const char *switch_name) {
    ucSwitchTok *switch_tok = ucCmd_get_switch(p);
    return switch_tok
        ? ucSwitchTok_find(switch_tok, switch_name)
        : NULL;
}

ucArgTok *ucCmd_get_switch_arg(ucCmd *p, const char *switch_name) {
    return get_arg_x(ucCmd_find_switch(p, switch_name), 0);
}

ucArgTok *ucCmd_get_switch_arg_x(ucCmd *p, const char *switch_name, int arg_index) {
    return get_arg_x(ucCmd_find_switch(p, switch_name), arg_index);
}

int ucCmd_get_switch_arg_d(ucCmd *p, const char *switch_name, int default_value) {
    return get_arg_x_d(ucCmd_find_switch(p, switch_name), 0, default_value);
}

int ucCmd_get_switch_arg_x_d(ucCmd *p, const char *switch_name, int arg_index, int default_value) {
    return get_arg_x_d(ucCmd_find_switch(p, switch_name), arg_index, default_value);
}

double ucCmd_get_switch_arg_f(ucCmd *p, const char *switch_name, double default_value) {
    return get_arg_x_f(ucCmd_find_switch(p, switch_name), 0, default_value);
}

double ucCmd_get_switch_arg_x_f(ucCmd *p, const char *switch_name, int arg_index, double default_value) {
    return get_arg_x_f(ucCmd_find_switch(p, switch_name), arg_index, default_value);
}

ucBool ucCmd_get_switch_arg_b(ucCmd *p, const char *switch_name, ucBool default_value) {
    return get_arg_x_b(ucCmd_find_switch(p, switch_name), 0, default_value);
}

ucBool ucCmd_get_switch_arg_x_b(ucCmd *p, const char *switch_name, int arg_index, ucBool default_value) {
    return get_arg_x_b(ucCmd_find_switch(p, switch_name), arg_index, default_value);
}

ucArgTok *ucCmd_get_arg_x(ucCmd *p, int arg_index) {
    return get_arg_x(ucCmd_get_command(p), arg_index);
}

ucBool ucCmd_get_arg_b(ucCmd *p, ucBool default_value) {
    return get_arg_x_b(ucCmd_get_command(p), 0, default_value);
}

int ucCmd_get_arg_d(ucCmd *p, int default_value) {
    return get_arg_x_d(ucCmd_get_command(p), 0, default_value);
}

double ucCmd_get_arg_f(ucCmd *p, double default_value) {
    return get_arg_x_f(ucCmd_get_command(p), 0, default_value);
}

ucBool ucCmd_get_arg_x_b(ucCmd *p, int arg_index, ucBool default_value) {
    return get_arg_x_b(ucCmd_get_command(p), arg_index, default_value);
}

int ucCmd_get_arg_x_d(ucCmd *p, int arg_index, int default_value) {
    return get_arg_x_d(ucCmd_get_command(p), arg_index, default_value);
}

double ucCmd_get_arg_x_f(ucCmd *p, int arg_index, double default_value) {
    return get_arg_x_f(ucCmd_get_command(p), arg_index, default_value);
}
