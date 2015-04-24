#ifndef UCCMDTOK_H
#define UCCMDTOK_H

#include "ucSwitchTok.h"

/*
 * Summary:
 *   Type definition for a command token. This is
 *   the first token (the command part) in a list
 *   of tokens. This type is a child of the base
 *   token type. All functions that take an instance
 *   of the base type can be used with an instance
 *   of this type.
 */
typedef const char ucCmdTok;

/*
 * Summary:
 *   Gets the first argument of the command.
 * Returns:
 *   A pointer to the first argument token, or NULL if no arguments
 *   exist for the command.
 */
uc_EXPORTED ucArgTok *ucCmdTok_get_arg(ucCmdTok*);

/*
 * Summary:
 *   Gets the first switch of the command.
 * Returns:
 *   A pointer to the first switch token of the command, or NULL
 *   if no switches exist.
 */
uc_EXPORTED ucSwitchTok *ucCmdTok_get_switch(ucCmdTok*);

#endif
