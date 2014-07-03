#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "uc_tok_p.h"
#include "uc_cmd_line_p.h"
#include "uc_cmd_parser.h"
#include "uc_arg_tok_owner.h"
#include "uc_switch_tok.h"

uc_cmd_line *uc_cmd_line_get_instance(void) {
    static uc_cmd_line instance = { 0 };
    static uc_cmd_line *p = NULL;
    if (NULL == p) {
        p = &instance;
        p->cmd_tok = NULL;
        p->transmit = NULL;
        p->transmit_state = NULL;
        p->is_quiet = UC_FALSE;
        p->is_cancelled = NULL;
        p->is_cancelled_state = NULL;
    }
    return p;
}

uc_cmd_tok *uc_cmd_line_get_cmd_tok(uc_cmd_line *p) {
    if (NULL == p) return NULL;
    return p->cmd_tok;
}

void uc_cmd_line_set_cmd_tok(uc_cmd_line *p, uc_cmd_tok *value) {
    if (NULL == p) return;
    p->cmd_tok = value;
}

uc_cmd_line_toks *uc_cmd_line_get_cmd_toks(uc_cmd_line *p, uc_cmd_line_toks *buffer) {
    if (NULL == p) return NULL;
    if (NULL == buffer) return NULL;

    buffer->cmd_tok = uc_cmd_line_get_cmd_tok(p);
    buffer->arg_tok = uc_cmd_tok_get_arg(buffer->cmd_tok);
    buffer->switch_tok = uc_cmd_tok_get_switch(buffer->cmd_tok);

    return buffer;
}

const char *uc_cmd_line_format_response_va(uc_cmd_line *p, const char *format, va_list arg_list) {
    if (NULL == p) return NULL;

	/* TODO: Buffer and copy were added because "usage" uses the command's response in the arg list.
	   TODO: There's probably a better-performing way to handle that, though. */
	vsnprintf(p->response_buffer, sizeof(p->response_buffer) - 1, format, arg_list);
	strcpy(p->response, p->response_buffer);
    return p->response;
}

const char *uc_cmd_line_format_response(uc_cmd_line *p, const char *format, ...) {
    va_list arg_list;
    const char *response;

    va_start(arg_list, format);
    response = uc_cmd_line_format_response_va(p, format, arg_list);
    va_end(arg_list);

    return response;
}

void uc_cmd_line_respond(uc_cmd_line *p, const char *response) {
    if (NULL == p) return;
    if (NULL == p->transmit) return;
    if (p->is_quiet) return;
    p->transmit(response, p->transmit_state);
}

void uc_cmd_line_set_transmit(uc_cmd_line *p, uc_cmd_line_transmit_func *value) {
    if (NULL == p) return;
    p->transmit = value;
}

uc_cmd_line_transmit_func *uc_cmd_line_get_transmit(uc_cmd_line *p) {
    if (NULL == p) return NULL;
    return p->transmit;
}

uc_bool uc_cmd_line_is_cancelled(uc_cmd_line *p) {
    if (NULL == p) return UC_FALSE;
    if (NULL == p->is_cancelled) return UC_FALSE;
    return p->is_cancelled(p->is_cancelled_state);
}

void uc_cmd_line_set_is_cancelled(uc_cmd_line *p, uc_cmd_line_is_cancelled_func *value) {
    if (NULL == p) return;
    p->is_cancelled = value;
}

uc_cmd_line_is_cancelled_func *uc_cmd_line_get_is_cancelled(uc_cmd_line *p) {
    if (NULL == p) return NULL;
    return p->is_cancelled;
}

void *uc_cmd_line_get_transmit_state(uc_cmd_line *p) {
    if (NULL == p) return NULL;
    return p->transmit_state;
}

UC_EXPORTED void uc_cmd_line_set_transmit_state(uc_cmd_line *p, void *value) {
    if (NULL == p) return;
    p->transmit_state = value;
}

void *uc_cmd_line_get_is_cancelled_state(uc_cmd_line *p) {
    if (NULL == p) return NULL;
    return p->is_cancelled_state;
}

void uc_cmd_line_set_is_cancelled_state(uc_cmd_line *p, void *value) {
    if (NULL == p) return;
    p->is_cancelled_state = value;
}

void uc_cmd_line_set_is_quiet(uc_cmd_line *p, uc_bool value) {
    if (NULL == p) return;
    p->is_quiet = value;
}

uc_bool uc_cmd_line_get_is_quiet(uc_cmd_line *p) {
    if (NULL == p) return UC_FALSE;
    return p->is_quiet;
}
