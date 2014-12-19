#ifndef UCCMDPARSER_H
#define UCCMDPARSER_H

#include "ucCmdTok.h"

/*
 * Summary:
 *   Type that can be used to parse command lines.
 *   The result of the parse can be used as the command
 *   token for a command structure.
 */
typedef struct ucCmdParser ucCmdParser;

/*
 * Summary:
 *   Gets a static, default instance of the parser.
 * Returns:
 *   The static, default instance of the parser, or NULL
 *   if an error occurred.
 */
UC_EXPORTED ucCmdParser *ucCmdParser_get_instance(void);

/*
 * Summary:
 *   Parses a command so that it can be used as a command token.
 * Parameters:
 *   p: A pointer to the parser that does the parsing.
 *   cmd: The command string to be parsed. This string is mutated.
 * Returns:
 *   A pointer to the command token that was parsed, or NULL if an
 *   error occurred.
 */
UC_EXPORTED ucCmdTok *ucCmdParser_parse(ucCmdParser *p, char *cmd);

#endif
