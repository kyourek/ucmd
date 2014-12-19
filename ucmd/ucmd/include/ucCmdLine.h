#ifndef UCCMDLINE_H
#define UCCMDLINE_H

#include <stdarg.h>
#include "ucCmdLineToks.h"

/*
 * Summary:
 *   A command structure. This structure consists
 *   of the parsed command and the ability to respond.
 */
typedef struct ucCmdLine ucCmdLine;

/*
 * Summary:
 *   The type of function used by a command structure
 *   to transmit responses.
 * Parameters:
 *   response: The string to be transmitted.
 *   state: A stateful object.
 */
typedef void (ucCmdLine_transmit_func)(const char *response, void *state);

/*
 * Summary:
 *   The type of function used by a command structure
 *   to determine whether or not the command is cancelled.
 * Parameters:
 *   state: A stateful object.
 * Returns:
 *   ucBool_true if the command has been cancelled. Otherwise, ucBool_false.
 */
typedef ucBool (ucCmdLine_is_cancelled_func)(void *state);

/*
 * Summary:
 *   Gets the command token from the command structure.
 * Parameters:
 *   p: A pointer to the structure whose token is returned.
 * Returns:
 *   A pointer to the command token of the structure.
 */
uc_EXPORTED ucCmdTok *ucCmdLine_get_cmd_tok(ucCmdLine *p);

/*
 * Summary:
 *   Sets the command token for the structure.
 * Parameters:
 *   p: A pointer to the structure whose property is set.
 *   value: The command token.
 */
uc_EXPORTED void ucCmdLine_set_cmd_tok(ucCmdLine *p, ucCmdTok *value);

/*
 * Summary:
 *   Fills the buffer with the specified tokens of the command structure.
 * Parameters:
 *   p: A pointer to the command structure.
 *   buffer: A pointer to the token structure whose properties will be set per the command.
 * Returns:
 *   A pointer to the buffer.
 */
uc_EXPORTED ucCmdLineToks *ucCmdLine_get_cmd_toks(ucCmdLine *p, ucCmdLineToks *buffer);

/*
 * Summary:
 *   Sets the function used by the command structure to transmit responses.
 * Parameters:
 *   p: A pointer to the structure whose property is set.
 *   value: A pointer to the function used to transmit responses.
 */
uc_EXPORTED void ucCmdLine_set_transmit(ucCmdLine *p, ucCmdLine_transmit_func *value);

/*
 * Summary:
 *   Gets the function used by the command structure to transmit responses.
 * Parameters:
 *   p: A pointer to the structure whose property is returned.
 * Returns:
 *   A pointer to the function used by the structure to transmit responses.
 */
uc_EXPORTED ucCmdLine_transmit_func *ucCmdLine_get_transmit(ucCmdLine *p);

/*
 * Summary:
 *   Gets the stateful object passed to the command's transmit function.
 * Parameters:
 *   p: A pointer to the command structure whose property is returned.
 * Returns:
 *   A pointer to the stateful object passed to the command's transmit function.
 */
uc_EXPORTED void *ucCmdLine_get_transmit_state(ucCmdLine *p);

/*
 * Summary:
 *   Sets the stateful object passed to the command's transmit function.
 * Parameters:
 *   p: A pointer to the command structure whose property is to be set.
 *   value: A pointer to the stateful object that is passed to the command's transmit function.
 */
uc_EXPORTED void ucCmdLine_set_transmit_state(ucCmdLine *p, void *value);

/*
 * Summary:
 *   Determines whether or not the command has been cancelled.
 * Parameters:
 *   p: The structure to check for cancellation.
 * Returns:
 *   ucBool_true if the command has been cancelled. Otherwise, ucBool_false.
 */
uc_EXPORTED ucBool ucCmdLine_is_cancelled(ucCmdLine *p);

/*
 * Summary:
 *   Sets the function used by the command structure to check for cancellation.
 * Parameters:
 *   p: A pointer to the structure whose property is set.
 *   value: A pointer to the function used to check for cancellation.
 */
uc_EXPORTED void ucCmdLine_set_is_cancelled(ucCmdLine *p, ucCmdLine_is_cancelled_func *value);

/*
 * Summary:
 *   Gets the function used by the command structure to check for cancellation.
 * Parameters:
 *   p: A pointer to the structure whose property is returned.
 * Returns:
 *   A pointer to the function used to check for cancellation.
 */
uc_EXPORTED ucCmdLine_is_cancelled_func *ucCmdLine_get_is_cancelled(ucCmdLine *p);

/*
 * Summary:
 *   Gets the stateful object passed to the command's cancellation function.
 * Parameters:
 *   p: A pointer to the command structure whose property is to be returned.
 * Returns:
 *   A pointer to the stateful object passed to the command's cancellation function.
 */
uc_EXPORTED void *ucCmdLine_get_is_cancelled_state(ucCmdLine *p);

/*
 * Summary:
 *   Sets the stateful object passed to the command's cancellation function.
 * Parameters:
 *   p: A pointer to the command structure whose property is to be set.
 *   value: The stateful object passed to the command's cancellation function.
 */
uc_EXPORTED void ucCmdLine_set_is_cancelled_state(ucCmdLine *p, void *value);

/*
 * Summary:
 *   Gets a static, default instance of the command structure.
 * Returns:
 *   The static, default instance of the command structure.
 */
uc_EXPORTED ucCmdLine *ucCmdLine_get_instance(void);

/*
 * Summary:
 *   Formats the command's response using the given parameters.
 * Parameters:
 *   p: A pointer to the command structure whose response is formatted.
 *   format: The format string.
 *   ...: Parameters to the format string.
 * Returns:
 *   The formatted string.
 */
uc_EXPORTED const char *ucCmdLine_format_response(ucCmdLine *p, const char *format, ...);

/*
 * Summary:
 *   Formats the command's response using the given parameters.
 * Parameters:
 *   p: A pointer to the command structure whose response is formatted.
 *   format: The format string.
 *   arg_list: A variable length argument list with parameters to the format string.
 * Returns:
 *   The formatted string.
 */
uc_EXPORTED const char *ucCmdLine_format_response_va(ucCmdLine *p, const char *format, va_list arg_list);

/*
 * Summary
 *   Responds to the command.
 * Parameters:
 *   p: A pointer to the command structure that is responding.
 *   response: The response string.
 */
uc_EXPORTED void ucCmdLine_respond(ucCmdLine *p, const char *response);

/*
 * Summary:
 *   Sets whether or not the command structure is quiet, meaning no response
 *   strings will be sent.
 * Parameters:
 *   p: A pointer to the command structure whose property is set.
 *   value: A boolean true value if the command structure should not send
 *          any response strings. Otherwise, false.
 */
uc_EXPORTED void ucCmdLine_set_is_quiet(ucCmdLine *p, ucBool value);

/*
 * Summary:
 *   Gets a flag that indicates whether or not the command structure is quiet,
 *   meaning no response strings are sent.
 * Parameters:
 *   p: A pointer to the command structure whose property is returned.
 * Returns:
 *   A boolean true value if the command structure is not sending any
 *   response strings. Otherwise, false.
 */
uc_EXPORTED ucBool ucCmdLine_get_is_quiet(ucCmdLine *p);

#endif
