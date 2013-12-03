#ifndef CL_CMD_TOK_H
#define CL_CMD_TOK_H

#include "cl_switch_tok.h"

/*
 * Summary:
 *   Type definition for a command token. This is
 *   the first token (the command part) in a list
 *   of tokens. This type is a child of the base
 *   token type. All functions that take an instance
 *   of the base type can be used with an instance
 *   of this type.
 */
typedef const char cl_cmd_tok;

/*
 * Summary:
 *   Gets the first argument of the command.
 * Parameters:
 *   p: A pointer to the command token whose argument is
 *      returned.
 * Returns:
 *   A pointer to the first argument token, or NULL if no arguments
 *   exist for the command.
 */
CL_EXPORTED cl_arg_tok *cl_cmd_tok_get_arg(cl_cmd_tok *p);

/*
 * Summary:
 *   Gets the first switch of the command.
 * Parameters:
 *   p: A pointer to the command token whose switch is returned.
 * Returns:
 *   A pointer to the first switch token of the command, or NULL
 *   if no switches exist.
 */
CL_EXPORTED cl_switch_tok *cl_cmd_tok_get_switch(cl_cmd_tok *p);

#endif
