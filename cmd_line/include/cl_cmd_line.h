#ifndef CL_CMD_LINE_H
#define CL_CMD_LINE_H

#include <stdarg.h>
#include "cl_cmd_line_toks.h"

/*
 * Summary:
 *   A command structure. This structure consists
 *   of the parsed command and the ability to respond.
 */
typedef struct cl_cmd_line cl_cmd_line;

/*
 * Summary:
 *   The type of function used by a command structure
 *   to transmit responses.
 * Parameters:
 *   response: The string to be transmitted.
 *   state: A stateful object.
 */
typedef void (cl_cmd_line_transmit_func)(const char *response, void *state);

/*
 * Summary:
 *   The type of function used by a command structure
 *   to determine whether or not the command is cancelled.
 * Parameters:
 *   state: A stateful object.
 * Returns:
 *   CL_TRUE if the command has been cancelled. Otherwise, CL_FALSE.
 */
typedef cl_bool (cl_cmd_line_is_cancelled_func)(void *state);

/*
 * Summary:
 *   Gets the command token from the command structure.
 * Parameters:
 *   p: A pointer to the structure whose token is returned.
 * Returns:
 *   A pointer to the command token of the structure.
 */
CL_EXPORTED cl_cmd_tok *cl_cmd_line_get_cmd_tok(cl_cmd_line *p);

/*
 * Summary:
 *   Sets the command token for the structure.
 * Parameters:
 *   p: A pointer to the structure whose property is set.
 *   value: The command token.
 */
CL_EXPORTED void cl_cmd_line_set_cmd_tok(cl_cmd_line *p, cl_cmd_tok *value);

/*
 * Summary:
 *   Fills the buffer with the specified tokens of the command structure.
 * Parameters:
 *   p: A pointer to the command structure.
 *   buffer: A pointer to the token structure whose properties will be set per the command.
 * Returns:
 *   A pointer to the buffer.
 */
CL_EXPORTED cl_cmd_line_toks *cl_cmd_line_get_cmd_toks(cl_cmd_line *p, cl_cmd_line_toks *buffer);

/*
 * Summary:
 *   Sets the function used by the command structure to transmit responses.
 * Parameters:
 *   p: A pointer to the structure whose property is set.
 *   value: A pointer to the function used to transmit responses.
 */
CL_EXPORTED void cl_cmd_line_set_transmit(cl_cmd_line *p, cl_cmd_line_transmit_func *value);

/*
 * Summary:
 *   Gets the function used by the command structure to transmit responses.
 * Parameters:
 *   p: A pointer to the structure whose property is returned.
 * Returns:
 *   A pointer to the function used by the structure to transmit responses.
 */
CL_EXPORTED cl_cmd_line_transmit_func *cl_cmd_line_get_transmit(cl_cmd_line *p);

/*
 * Summary:
 *   Gets the stateful object passed to the command's transmit function.
 * Parameters:
 *   p: A pointer to the command structure whose property is returned.
 * Returns:
 *   A pointer to the stateful object passed to the command's transmit function.
 */
CL_EXPORTED void *cl_cmd_line_get_transmit_state(cl_cmd_line *p);

/*
 * Summary:
 *   Sets the stateful object passed to the command's transmit function.
 * Parameters:
 *   p: A pointer to the command structure whose property is to be set.
 *   value: A pointer to the stateful object that is passed to the command's transmit function.
 */
CL_EXPORTED void cl_cmd_line_set_transmit_state(cl_cmd_line *p, void *value);

/*
 * Summary:
 *   Determines whether or not the command has been cancelled.
 * Parameters:
 *   p: The structure to check for cancellation.
 * Returns:
 *   CL_TRUE if the command has been cancelled. Otherwise, CL_FALSE.
 */
CL_EXPORTED cl_bool cl_cmd_line_is_cancelled(cl_cmd_line *p);

/*
 * Summary:
 *   Sets the function used by the command structure to check for cancellation.
 * Parameters:
 *   p: A pointer to the structure whose property is set.
 *   value: A pointer to the function used to check for cancellation.
 */
CL_EXPORTED void cl_cmd_line_set_is_cancelled(cl_cmd_line *p, cl_cmd_line_is_cancelled_func *value);

/*
 * Summary:
 *   Gets the function used by the command structure to check for cancellation.
 * Parameters:
 *   p: A pointer to the structure whose property is returned.
 * Returns:
 *   A pointer to the function used to check for cancellation.
 */
CL_EXPORTED cl_cmd_line_is_cancelled_func *cl_cmd_line_get_is_cancelled(cl_cmd_line *p);

/*
 * Summary:
 *   Gets the stateful object passed to the command's cancellation function.
 * Parameters:
 *   p: A pointer to the command structure whose property is to be returned.
 * Returns:
 *   A pointer to the stateful object passed to the command's cancellation function.
 */
CL_EXPORTED void *cl_cmd_line_get_is_cancelled_state(cl_cmd_line *p);

/*
 * Summary:
 *   Sets the stateful object passed to the command's cancellation function.
 * Parameters:
 *   p: A pointer to the command structure whose property is to be set.
 *   value: The stateful object passed to the command's cancellation function.
 */
CL_EXPORTED void cl_cmd_line_set_is_cancelled_state(cl_cmd_line *p, void *value);

/*
 * Summary:
 *   Gets a static, default instance of the command structure.
 * Returns:
 *   The static, default instance of the command structure.
 */
CL_EXPORTED cl_cmd_line *cl_cmd_line_get_instance(void);

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
CL_EXPORTED const char *cl_cmd_line_format_response(cl_cmd_line *p, const char *format, ...);

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
CL_EXPORTED const char *cl_cmd_line_format_response_va(cl_cmd_line *p, const char *format, va_list arg_list);

/*
 * Summary
 *   Responds to the command.
 * Parameters:
 *   p: A pointer to the command structure that is responding.
 *   response: The response string.
 */
CL_EXPORTED void cl_cmd_line_respond(cl_cmd_line *p, const char *response);

#endif
