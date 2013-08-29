#ifndef CL_CMD_LINE_TOKS_H
#define CL_CMD_LINE_TOKS_H

#include "cl_arg_tok.h"
#include "cl_cmd_tok.h"
#include "cl_switch_tok.h"

/*
 * Summary:
 *   A group of tokens that represent the first
 *   of each token type in a command.
 */
typedef struct cl_cmd_line_toks {

    /*
     * Summary:
     *   The command token of the command. This token's
     *   value is the invoked command.
     */
    cl_cmd_tok *cmd_tok;

    /*
     * Summary:
     *   The command's first argument token, or
     *   NULL if no arguments exist.
     */
    cl_arg_tok *arg_tok;

    /*
     * Summary:
     *   The command's first switch token, or
     *   NULL if no switches exist.
     */
    cl_switch_tok *switch_tok;

} cl_cmd_line_toks;

#endif
