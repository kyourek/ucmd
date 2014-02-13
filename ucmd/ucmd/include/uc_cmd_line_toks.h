#ifndef UC_CMD_LINE_TOKS_H
#define UC_CMD_LINE_TOKS_H

#include "uc_cmd_tok.h"

/*
 * Summary:
 *   A group of tokens that represent the first
 *   of each token type in a command.
 */
typedef struct uc_cmd_line_toks {

    /*
     * Summary:
     *   The command token of the command. This token's
     *   value is the invoked command.
     */
    uc_cmd_tok *cmd_tok;

    /*
     * Summary:
     *   The command's first argument token, or
     *   NULL if no arguments exist.
     */
    uc_arg_tok *arg_tok;

    /*
     * Summary:
     *   The command's first switch token, or
     *   NULL if no switches exist.
     */
    uc_switch_tok *switch_tok;

} uc_cmd_line_toks;

/*
 * Summary:
 *   Gets the command token of the command. This token's
 *   value is the invoked command.
 * Parameters:
 *   p: A pointer to the object whose property value is returned.
 * Returns:
 *   A pointer to the command token.
 */
UC_EXPORTED uc_cmd_tok *uc_cmd_line_toks_get_cmd_tok(uc_cmd_line_toks *p);

/*
 * Summary:
 *   The command's first argument token, or
 *   NULL if no arguments exist.
 * Parameters:
 *   p: A pointer to the object whose property value is returned.
 * Returns:
 *   A pointer to the argument token, or NULL if no argument
 *   tokens exist.
 */
UC_EXPORTED uc_arg_tok *uc_cmd_line_toks_get_arg_tok(uc_cmd_line_toks *p);

/*
 * Summary:
 *   The command's first switch token, or
 *   NULL if no switches exist.
 * Parameters:
 *   p: A pointer to the object whose property value is returned.
 * Returns:
 *   A pointer to the switch token, or NULL if no switch tokens exist.
 */
UC_EXPORTED uc_switch_tok *uc_cmd_line_toks_get_switch_tok(uc_cmd_line_toks *p);

#endif
