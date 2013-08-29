#ifndef CL_CMD_PARSER_H
#define CL_CMD_PARSER_H

#include "cl_cmd_tok.h"
#include "cl_common.h"

/*
 * Summary:
 *   Type that can be used to parse command lines.
 *   The result of the parse can be used as the command
 *   token for a command structure.
 */
typedef struct cl_cmd_parser cl_cmd_parser;

/*
 * Summary:
 *   Gets a static, default instance of the parser.
 * Returns:
 *   The static, default instance of the parser, or NULL
 *   if an error occurred.
 */
CL_EXPORTED cl_cmd_parser *cl_cmd_parser_get_instance(void);

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
CL_EXPORTED cl_cmd_tok *cl_cmd_parser_parse(cl_cmd_parser *p, char *cmd);

#endif
