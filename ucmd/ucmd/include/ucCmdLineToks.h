#ifndef UC_CMD_LINE_TOKS_H
#define UC_CMD_LINE_TOKS_H

#include "ucCmdTok.h"

/*
 * Summary:
 *   A group of tokens that represent the first
 *   of each token type in a command.
 */
typedef struct ucCmdLineToks {

    /*
     * Summary:
     *   The command token of the command. This token's
     *   value is the invoked command.
     */
    ucCmdTok *cmd_tok;

    /*
     * Summary:
     *   The command's first argument token, or
     *   NULL if no arguments exist.
     */
    ucArgTok *arg_tok;

    /*
     * Summary:
     *   The command's first switch token, or
     *   NULL if no switches exist.
     */
    ucSwitchTok *switch_tok;

} ucCmdLineToks;

/*
 * Summary:
 *   Gets the command token of the command. This token's
 *   value is the invoked command.
 * Parameters:
 *   p: A pointer to the object whose property value is returned.
 * Returns:
 *   A pointer to the command token.
 */
UC_EXPORTED ucCmdTok *ucCmdLineToks_get_cmd_tok(ucCmdLineToks *p);

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
UC_EXPORTED ucArgTok *ucCmdLineToks_get_arg_tok(ucCmdLineToks *p);

/*
 * Summary:
 *   The command's first switch token, or
 *   NULL if no switches exist.
 * Parameters:
 *   p: A pointer to the object whose property value is returned.
 * Returns:
 *   A pointer to the switch token, or NULL if no switch tokens exist.
 */
UC_EXPORTED ucSwitchTok *ucCmdLineToks_get_switch_tok(ucCmdLineToks *p);

#endif
