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
typedef void (ucCmdLine_TransmitFunc)(const char *response, void *state);

/*
 * Summary:
 *   The type of function used by a command structure
 *   to determine whether or not the command is canceled.
 * Parameters:
 *   state: A stateful object.
 * Returns:
 *   ucBool_TRUE if the command has been canceled. Otherwise, ucBool_FALSE.
 */
typedef ucBool (ucCmdLine_IsCanceledFunc)(void *state);

/*
 * Summary:
 *   The type of the function that is invoked when an
 *   invalid command is encountered. An invalid command
 *   is one that does not exist in the list of command
 *   options.
 * Parameters:
 *   invalid_command: The invalid command string that was encountered.
 *   state: The stateful object for this callback.
 * Returns:
 *   ucBool_TRUE if the invalid command was handeled. Otherwise, ucBool_FALSE.
 */
typedef ucBool (ucCmdLine_HandleInvalidCommandFunc)(const char *invalid_command, void *state);

/*
 * Summary:
 *   Gets the command token from the command structure.
 * Returns:
 *   A pointer to the command token of the structure.
 */
uc_EXPORTED ucCmdTok *ucCmdLine_get_cmd_tok(ucCmdLine*);

/*
 * Summary:
 *   Sets the command token for the structure.
 * Parameters:
 *   value: The command token.
 */
uc_EXPORTED void ucCmdLine_set_cmd_tok(ucCmdLine*, ucCmdTok *value);

/*
 * Summary:
 *   Fills the buffer with the specified tokens of the command structure.
 * Parameters:
 *   buffer: A pointer to the token structure whose properties will be set per the command.
 * Returns:
 *   A pointer to the buffer.
 */
uc_EXPORTED ucCmdLineToks *ucCmdLine_get_cmd_toks(ucCmdLine*, ucCmdLineToks *buffer);

/*
 * Summary:
 *   Sets the function used by the command structure to transmit responses.
 * Parameters:
 *   value: A pointer to the function used to transmit responses.
 */
uc_EXPORTED void ucCmdLine_set_transmit(ucCmdLine*, ucCmdLine_TransmitFunc *value);

/*
 * Summary:
 *   Gets the function used by the command structure to transmit responses.
 * Returns:
 *   A pointer to the function used by the structure to transmit responses.
 */
uc_EXPORTED ucCmdLine_TransmitFunc *ucCmdLine_get_transmit(ucCmdLine*);

/*
 * Summary:
 *   Gets the stateful object passed to the command's transmit function.
 * Returns:
 *   A pointer to the stateful object passed to the command's transmit function.
 */
uc_EXPORTED void *ucCmdLine_get_transmit_state(ucCmdLine*);

/*
 * Summary:
 *   Sets the stateful object passed to the command's transmit function.
 * Parameters:
 *   value: A pointer to the stateful object that is passed to the command's transmit function.
 */
uc_EXPORTED void ucCmdLine_set_transmit_state(ucCmdLine*, void *value);

/*
 * Summary:
 *   Determines whether or not the command has been canceled.
 * Returns:
 *   ucBool_TRUE if the command has been canceled. Otherwise, ucBool_FALSE.
 */
uc_EXPORTED ucBool ucCmdLine_is_canceled(ucCmdLine*);

/*
 * Summary:
 *   Sets the function used by the command structure to check for cancellation.
 * Parameters:
 *   value: A pointer to the function used to check for cancellation.
 */
uc_EXPORTED void ucCmdLine_set_is_canceled(ucCmdLine*, ucCmdLine_IsCanceledFunc *value);

/*
 * Summary:
 *   Gets the function used by the command structure to check for cancellation.
 * Returns:
 *   A pointer to the function used to check for cancellation.
 */
uc_EXPORTED ucCmdLine_IsCanceledFunc *ucCmdLine_get_is_canceled(ucCmdLine*);

/*
 * Summary:
 *   Gets the stateful object passed to the command's cancellation function.
 * Returns:
 *   A pointer to the stateful object passed to the command's cancellation function.
 */
uc_EXPORTED void *ucCmdLine_get_is_canceled_state(ucCmdLine*);

/*
 * Summary:
 *   Sets the stateful object passed to the command's cancellation function.
 * Parameters:
 *   value: The stateful object passed to the command's cancellation function.
 */
uc_EXPORTED void ucCmdLine_set_is_canceled_state(ucCmdLine*, void *value);

uc_EXPORTED void ucCmdLine_set_handle_invalid_command(ucCmdLine*, ucCmdLine_HandleInvalidCommandFunc *value);

uc_EXPORTED ucCmdLine_HandleInvalidCommandFunc *ucCmdLine_get_handle_invalid_command(ucCmdLine*);

uc_EXPORTED void ucCmdLine_set_handle_invalid_command_state(ucCmdLine*, void *value);

uc_EXPORTED void *ucCmdLine_get_handle_invalid_command_state(ucCmdLine*);

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
 *   format: The format string.
 *   ...: Parameters to the format string.
 * Returns:
 *   The formatted string.
 */
uc_EXPORTED const char *ucCmdLine_format_response(ucCmdLine*, const char *format, ...);

/*
 * Summary:
 *   Formats the command's response using the given parameters.
 * Parameters:
 *   format: The format string.
 *   arg_list: A variable length argument list with parameters to the format string.
 * Returns:
 *   The formatted string.
 */
uc_EXPORTED const char *ucCmdLine_format_response_va(ucCmdLine*, const char *format, va_list arg_list);

/*
 * Summary
 *   Responds to the command.
 * Parameters:
 *   response: The response string.
 */
uc_EXPORTED void ucCmdLine_respond(ucCmdLine*, const char *response);

/*
 * Summary:
 *   Sets whether or not the command structure is quiet, meaning no response
 *   strings will be sent.
 * Parameters:
 *   value: A boolean true value if the command structure should not send
 *          any response strings. Otherwise, false.
 */
uc_EXPORTED void ucCmdLine_set_is_quiet(ucCmdLine*, ucBool value);

/*
 * Summary:
 *   Gets a flag that indicates whether or not the command structure is quiet,
 *   meaning no response strings are sent.
 * Returns:
 *   A boolean true value if the command structure is not sending any
 *   response strings. Otherwise, false.
 */
uc_EXPORTED ucBool ucCmdLine_get_is_quiet(ucCmdLine*);

#endif
