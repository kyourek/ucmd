#ifndef UC_CMD_PARSER_H
#define UC_CMD_PARSER_H

#include "uc_cmd_tok.h"

/*
 * Summary:
 *   Type that can be used to parse command lines.
 *   The result of the parse can be used as the command
 *   token for a command structure.
 */
typedef struct uc_cmd_parser uc_cmd_parser;

/*
 * Summary:
 *   Gets a static, default instance of the parser.
 * Returns:
 *   The static, default instance of the parser, or NULL
 *   if an error occurred.
 */
UC_EXPORTED uc_cmd_parser *uc_cmd_parser_get_instance(void);

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
UC_EXPORTED uc_cmd_tok *uc_cmd_parser_parse(uc_cmd_parser *p, char *cmd);

#endif
