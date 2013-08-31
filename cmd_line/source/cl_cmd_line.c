#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "cl_tok.h"
#include "cl_tok_p.h"
#include "cl_cmd_parser.h"
#include "cl_common.h"
#include "cl_arg_tok.h"
#include "cl_arg_tok_owner.h"
#include "cl_switch_tok.h"
#include "cl_cmd_line.h"
#include "cl_cmd_line_p.h"
#include "cl_cmd_line_toks.h"

static void transmit(const char *response) {
#ifdef CL_CMD_LINE_TRANSMIT_PRINTF
    printf("%s\n", response);
#endif
}

static CL_BOOL is_cancelled(void) {
    return CL_FALSE;
}

cl_cmd_line *cl_cmd_line_get_instance(void) {
    static cl_cmd_line instance = { 0 };
    static cl_cmd_line *p = NULL;
    if (NULL == p) {
        p = &instance;
        p->cmd_tok = NULL;
        p->transmit = transmit;
        p->is_cancelled = is_cancelled;
    }
    return p;
}

cl_cmd_tok *cl_cmd_line_get_cmd_tok(cl_cmd_line *p) {
    if (NULL == p) return NULL;
    return p->cmd_tok;
}

void cl_cmd_line_set_cmd_tok(cl_cmd_line *p, cl_cmd_tok *value) {
    if (NULL == p) return;
    p->cmd_tok = value;
}

cl_cmd_line_toks *cl_cmd_line_get_cmd_toks(cl_cmd_line *p, cl_cmd_line_toks *buffer) {
    if (NULL == p) return NULL;
    if (NULL == buffer) return NULL;

    buffer->cmd_tok = cl_cmd_line_get_cmd_tok(p);
    buffer->arg_tok = cl_cmd_tok_get_arg(buffer->cmd_tok);
    buffer->switch_tok = cl_cmd_tok_get_switch(buffer->cmd_tok);

    return buffer;
}

const char *cl_cmd_line_format_response(cl_cmd_line *p, const char *format, ...) {
    va_list arg_list;
    if (NULL == p) return NULL;

    va_start(arg_list, format);
    vsnprintf(p->response, (sizeof(p->response) / sizeof(char)) - 1, format, arg_list);
    va_end(arg_list);

    return p->response;
}

void cl_cmd_line_respond(cl_cmd_line *p, const char *response) {
    if (NULL == p) return;
    if (NULL == p->transmit) return;
    p->transmit(response);
}

void cl_cmd_line_set_transmit(cl_cmd_line *p, cl_cmd_line_transmit_func *value) {
    if (NULL == p) return;
    p->transmit = value;
}

cl_cmd_line_transmit_func *cl_cmd_line_get_transmit(cl_cmd_line *p) {
    if (NULL == p) return NULL;
    return p->transmit;
}

CL_BOOL cl_cmd_line_is_cancelled(cl_cmd_line *p) {
    if (NULL == p) return CL_FALSE;
    if (NULL == p->is_cancelled) return CL_FALSE;
    return p->is_cancelled();
}

void cl_cmd_line_set_is_cancelled(cl_cmd_line *p, cl_cmd_line_is_cancelled_func *value) {
    if (NULL == p) return;
    p->is_cancelled = value;
}

cl_cmd_line_is_cancelled_func *cl_cmd_line_get_is_cancelled(cl_cmd_line *p) {
    if (NULL == p) return NULL;
    return p->is_cancelled;
}
