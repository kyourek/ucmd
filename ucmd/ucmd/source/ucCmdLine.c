#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "ucTok_p.h"
#include "ucCmdLine_p.h"
#include "ucCmdParser.h"
#include "ucArgTokOwner.h"
#include "ucSwitchTok.h"

ucCmdLine *ucCmdLine_get_instance(void) {
    static ucCmdLine instance = { 0 };
    static ucCmdLine *p = NULL;
    if (NULL == p) {
        p = &instance;
        p->cmd_tok = NULL;
        p->transmit = NULL;
        p->transmit_state = NULL;
        p->is_quiet = ucBool_false;
        p->is_cancelled = NULL;
        p->is_cancelled_state = NULL;
    }
    return p;
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

ucBool ucCmdLine_is_cancelled(ucCmdLine *p) {
    if (NULL == p) return ucBool_false;
    if (NULL == p->is_cancelled) return ucBool_false;
    return p->is_cancelled(p->is_cancelled_state);
}

void ucCmdLine_set_is_cancelled(ucCmdLine *p, ucCmdLine_IsCancelledFunc *value) {
    if (NULL == p) return;
    p->is_cancelled = value;
}

ucCmdLine_IsCancelledFunc *ucCmdLine_get_is_cancelled(ucCmdLine *p) {
    if (NULL == p) return NULL;
    return p->is_cancelled;
}

void *ucCmdLine_get_transmit_state(ucCmdLine *p) {
    if (NULL == p) return NULL;
    return p->transmit_state;
}

uc_EXPORTED void ucCmdLine_set_transmit_state(ucCmdLine *p, void *value) {
    if (NULL == p) return;
    p->transmit_state = value;
}

void *ucCmdLine_get_is_cancelled_state(ucCmdLine *p) {
    if (NULL == p) return NULL;
    return p->is_cancelled_state;
}

void ucCmdLine_set_is_cancelled_state(ucCmdLine *p, void *value) {
    if (NULL == p) return;
    p->is_cancelled_state = value;
}

void ucCmdLine_set_is_quiet(ucCmdLine *p, ucBool value) {
    if (NULL == p) return;
    p->is_quiet = value;
}

ucBool ucCmdLine_get_is_quiet(ucCmdLine *p) {
    if (NULL == p) return ucBool_false;
    return p->is_quiet;
}
