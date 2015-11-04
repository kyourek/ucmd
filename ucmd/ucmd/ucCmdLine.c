#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "ucmd_internal.h"

ucInstance_INIT(ucCmdLine, ucCmdLine_COUNT);

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
    if (NULL == p) return NULL;
    return p->cmd_tok;
}

void ucCmdLine_set_cmd_tok(ucCmdLine *p, ucCmdTok *value) {
    if (NULL == p) return;
    p->cmd_tok = value;
}

ucCmdLineToks *ucCmdLine_get_cmd_toks(ucCmdLine *p, ucCmdLineToks *buffer) {
    if (NULL == p) return NULL;
    if (NULL == buffer) return NULL;

    buffer->cmd_tok = ucCmdLine_get_cmd_tok(p);
    buffer->arg_tok = ucCmdTok_get_arg(buffer->cmd_tok);
    buffer->switch_tok = ucCmdTok_get_switch(buffer->cmd_tok);

    return buffer;
}

const char *ucCmdLine_format_response_va(ucCmdLine *p, const char *format, va_list arg_list) {
    if (NULL == p) return NULL;

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
    if (NULL == p) return;
    if (NULL == p->transmit) return;
    if (p->is_quiet) return;
    p->transmit(response, p->transmit_state);
}

void ucCmdLine_set_transmit(ucCmdLine *p, ucCmdLine_TransmitFunc *value) {
    if (NULL == p) return;
    p->transmit = value;
}

ucCmdLine_TransmitFunc *ucCmdLine_get_transmit(ucCmdLine *p) {
    if (NULL == p) return NULL;
    return p->transmit;
}

ucBool ucCmdLine_is_canceled(ucCmdLine *p) {
    if (NULL == p) return ucBool_false;
    if (NULL == p->is_canceled) return ucBool_false;
    return p->is_canceled(p->is_canceled_state);
}

void ucCmdLine_set_is_canceled(ucCmdLine *p, ucCmdLine_IsCanceledFunc *value) {
    if (NULL == p) return;
    p->is_canceled = value;
}

ucCmdLine_IsCanceledFunc *ucCmdLine_get_is_canceled(ucCmdLine *p) {
    if (NULL == p) return NULL;
    return p->is_canceled;
}

void *ucCmdLine_get_transmit_state(ucCmdLine *p) {
    if (NULL == p) return NULL;
    return p->transmit_state;
}

uc_EXPORTED void ucCmdLine_set_transmit_state(ucCmdLine *p, void *value) {
    if (NULL == p) return;
    p->transmit_state = value;
}

void *ucCmdLine_get_is_canceled_state(ucCmdLine *p) {
    if (NULL == p) return NULL;
    return p->is_canceled_state;
}

void ucCmdLine_set_is_canceled_state(ucCmdLine *p, void *value) {
    if (NULL == p) return;
    p->is_canceled_state = value;
}

void ucCmdLine_set_is_quiet(ucCmdLine *p, ucBool value) {
    if (NULL == p) return;
    p->is_quiet = value;
}

ucBool ucCmdLine_get_is_quiet(ucCmdLine *p) {
    if (NULL == p) return ucBool_false;
    return p->is_quiet;
}

void ucCmdLine_set_handle_invalid_command(ucCmdLine *p, ucCmdLine_HandleInvalidCommandFunc *value) {
    if (NULL == p) return;
    p->handle_invalid_command = value;
}

ucCmdLine_HandleInvalidCommandFunc *ucCmdLine_get_handle_invalid_command(ucCmdLine *p) {
    if (NULL == p) return NULL;
    return p->handle_invalid_command;
}

void ucCmdLine_set_handle_invalid_command_state(ucCmdLine *p, void *value) {
    if (NULL == p) return;
    p->handle_invalid_command_state = value;
}

void *ucCmdLine_get_handle_invalid_command_state(ucCmdLine *p) {
    if (NULL == p) return NULL;
    return p->handle_invalid_command_state;
}

ucBool ucCmdLine_handle_invalid_command(ucCmdLine *p, const char *invalid_command) {
    if (NULL == p) return ucBool_false;
    if (NULL == p->handle_invalid_command) return ucBool_false;
    return p->handle_invalid_command(invalid_command, p->handle_invalid_command_state);
}

size_t ucCmdLine_get_response_size_max(ucCmdLine *p) {
    if (NULL == p) return 0;
    return sizeof(p->response);
}

void ucCmdLine_set_response_terminator(ucCmdLine *p, const char *value) {
    if (NULL == p) return;
    p->response_terminator = value;
}

const char *ucCmdLine_get_response_terminator(ucCmdLine *p) {
    if (NULL == p) return NULL;
    return p->response_terminator;
}

void ucCmdLine_set_command_acknowledgment(ucCmdLine *p, const char *value) {
    if (NULL == p) return;
    p->command_acknowledgment = value;
}

const char *ucCmdLine_get_command_acknowledgment(ucCmdLine *p) {
    if (NULL == p) return NULL;
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
