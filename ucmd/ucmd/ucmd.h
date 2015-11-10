/********************************************************************************
 *  Copyright (c) 2013-2015 Ken Yourek
 * 
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 *******************************************************************************/

/** @file ucmd.h
 *  @date 4 November 2015
 *  @version 2.0.0-alpha
 *  @brief Public declarations for the ucmd library and framework.
 *  @see https://github.com/kyourek/ucmd
 *
 *  These are the types and functions made publicly available by ucmd.
 */

#ifndef     UCMD_H
#define     UCMD_H

#include    <stdarg.h>
#include    <stddef.h>

#ifndef ucOpt_INVALID
/** @brief The response prefix for notifications about invalid command options.
 *
 *  This prefix will be placed immediately before the description of the invalid
 *  option. It can be used when parsing a response to a command to determine the
 *  command's validity.
 */
#define ucOpt_INVALID "Invalid: "
#endif

#ifndef ucParser_CMD_TERMINATOR
/** @brief The character that terminates command strings.
 *
 *  This character must exist in each command string, and it signals the end of
 *  the command. Usually, a line-feed (LF) is a good choice, as Enter (or Return)
 *  will complete the command.
 */
#define ucParser_CMD_TERMINATOR '\n'
#endif

#ifndef ucParser_COUNT
/** @brief The number of instances of ucParser available to the program.
 *
 *  When static memory allocation is used, this number defines the number of
 *  instances of type ucParser that can be created. Since only one parser
 *  is required for an application, this number is typically 1.
 */
#define ucParser_COUNT 1
#endif

#ifndef ucCmd_COUNT
/** @brief The number of instances of ucCmd available to the program.
 *
 *  When static memory allocation is used, this number defines the number of
 *  instances of type ucCmd that can be created. Since only one of these
 *  objects is required for an application, this number is typically 1.
 */
#define ucCmd_COUNT 1
#endif

#ifndef ucCmdApp_COUNT
/** @brief The number of instances of ucCmdApp available to the program.
 *
 *  When static memory allocation is used, this number defines the number of
 *  instances of type ucCmdApp that can be created. Since only one of these
 *  objects is required for an application, this number is typically 1.
 */
#define ucCmdApp_COUNT 1
#endif

#ifndef ucCmdApp_CMD_STR_SIZE
/** @brief The size of the command buffer when using the command-line application framework.
 *
 *  All entered commands must have a size equal to or less than this buffer's size.
 */
#define ucCmdApp_CMD_STR_SIZE 200
#endif

#ifndef ucCmd_RESPONSE_SIZE
/** @brief The size of the command response buffer.
 *
 *  All response strings must have a size equal to or less than the size of this buffer to 
 *  avoid truncation.
 */
#define ucCmd_RESPONSE_SIZE 200
#endif

#ifndef ucCmdOpt_COUNT
/** @brief The number of command options that may be created.
 *
 *  The number of created command options must be equal to or less than this number.
 */
#define ucCmdOpt_COUNT 10
#endif

#ifndef ucSwitchOpt_COUNT
/** @brief The number of switch options that may be created.
 *
 *  The number of created switch options must be equal to or less than this number.
 */
#define ucSwitchOpt_COUNT 50
#endif

#ifndef ucArgOpt_COUNT
/** @brief The number of argument options that may be created.
 *
 *  This is the total number of options available to commands and switches, combined.
 */
#define ucArgOpt_COUNT 50
#endif

#ifndef ucTok_LENGTH_MAX
/** @brief The maximum expected length of a single token in a command line.
 *
 */
#define ucTok_LENGTH_MAX ucCmdApp_CMD_STR_SIZE
#endif

#ifndef ucTok_BOOLEAN_TRUE
/** @brief Strings that are evaluated as truthy boolean values.
 *
 *  Any of these strings may be entered in a command to satisfy a boolean argument.
 *  When entered, each will be evaluated as true.
 */
#define ucTok_BOOLEAN_TRUE "1", "on", "yes", "true"
#endif

#ifndef ucTok_BOOLEAN_FALSE
/** @brief Strings that are evaluated as falsey boolean values.
 *
 *  Any of these strings may be entered in a command to satisfy a boolean argument.
 *  When entered, each will be evaluated as false.
 */
#define ucTok_BOOLEAN_FALSE "0", "off", "no", "false"
#endif

#ifdef uc_DECLSPEC_DLLIMPORT
/** @brief Include this def when using the library with another program on Windows.
 *  
 *  When defined, exported functions will be decorated with dllimport to make them
 *  available to external programs. 
 */
#define uc_EXPORTED uc_EXTERN_C __declspec(dllimport)
#endif

#ifdef uc_DECLSPEC_DLLEXPORT
/** @brief Include this def when compiling this program on Windows.
 *
 *  When defined, exported functions will be decorated with dllexport to make them
 *  available to external programs.
 */
#define uc_EXPORTED uc_EXTERN_C __declspec(dllexport)
#endif

#ifdef __cplusplus
/** Prepend extern "C" if a C++ compiler is used. */
#define uc_EXTERN_C extern "C"
#else
/** Empty if a C compiler is used. */
#define uc_EXTERN_C
#endif

#ifndef uc_EXPORTED
/** Default to setting uc_EXPORTED to the result of our extern "C" check. */
#define uc_EXPORTED uc_EXTERN_C
#endif

#ifndef NULL
/** Define NULL, if it hasn't been defined already. */
#define NULL ((void*)0)
#endif

/** @defgroup ucBool Boolean values
 *  @{
 */

/** @brief Boolean type definition.
 *
 *  This definition is used to increase the readability of the source by replacing
 *  integer representations of boolean values with the more familiar "true" and
 *  "false" values. 
 */
typedef enum {
    ucBool_false = 0,               /**< Falsey value. */
    ucBool_true = !ucBool_false     /**< Truthy value. */
} ucBool;

/** @} */

/** @defgroup ucTok Command-line token
 *  @{
 */

/** @brief The type definition for a token in a command.
 *
 *  Tokens may be one of command, argument, or switch. A pointer to a token is similar
 *  to a null-terminated string, but it also suggests that other tokens may follow its
 *  null character. The final token in a chain should contain a terminating character.
 */
typedef const char ucTok;

/** @brief      Gets the length of the given token.
 *  @param[in]  p   The token in question.
 *  @returns    The number of characters in the token.
 *  @see        ucTok
 */
uc_EXPORTED int ucTok_get_length(ucTok *p);

/** @brief      Determines whether or not the given token is considered a switch.
 *  @param[in]  p   The given token.
 *  @returns    Boolean true if the token is a switch. Otherwise, false.
 *  @see        ucTok
 */
uc_EXPORTED ucBool ucTok_is_switch(ucTok *p);

/** @brief      Determines whether or not the given token equals the value.
 *  @param[in]  p       The given token.
 *  @param[in]  value   The value to which the token in question is compared.
 *  @returns    Boolean true if the given value equals the given token. Otherwise, false.
 *  @see        ucTok
 */
uc_EXPORTED ucBool ucTok_equals(ucTok *p, const char *value);

/** @brief      Determines whether or not the given token is an integer.
 *  @param[in]  p   The given token.
 *  @returns    Boolean true if the given token is an integer. Otherwise, false.
 *  @see        ucTok
 */
uc_EXPORTED ucBool ucTok_is_integer(ucTok *p);

/** @brief      Attempts to parse the given token to an integer value.
 *  @param[in]  p       The given token.
 *  @param[out] value   A container for the parsed integer value.
 *  @returns    Boolean true if the token was successfully parsed. Otherwise, false.
 *  @see        ucTok
 *
 *  If the token cannot be parsed to an integer, @a value is left unchanged.
 */
uc_EXPORTED ucBool ucTok_try_parse_integer(ucTok *p, int *value);

/** @brief      Parses the given token to an integer value.
 *  @param[in]  p               The given token.
 *  @param[in]  default_value   If the token cannot be parsed to an integer, this value is returned.
 *  @returns    The integer value of the token, or @a default_value if the token cannot be parsed.
 *  @see        ucTok
 */
uc_EXPORTED int ucTok_parse_integer(ucTok *p, int default_value);

/** @brief      Determines whether or not the given token represents a numeric value.
 *  @param[in]  p   The given token.
 *  @returns    Boolean true if the token is numeric. Otherwise, false.
 *  @see        ucTok
 */
uc_EXPORTED ucBool ucTok_is_numeric(ucTok *p);

/** @brief      Attempts to parse the given token to a numeric value.
 *  @param[in]  p       The given token.
 *  @param[out] value   A container for the parsed numeric value.
 *  @returns    Boolean true if the token was successfully parsed. Otherwise, false.
 *  @see        ucTok
 *
 *  If the token cannot be parsed to a numeric value, @a value is left unchanged.
 */
uc_EXPORTED ucBool ucTok_try_parse_numeric(ucTok *p, double *value);

/** @brief      Parses the given token to a numeric value.
 *  @param[in]  p               The given token.
 *  @param[in]  default_value   If the token cannot be parsed to a numeric value, this value is returned.
 *  @returns    The numeric value of the token, or @a default_value if the token cannot be parsed.
 *  @see        ucTok
 */
uc_EXPORTED double ucTok_parse_numeric(ucTok *p, double default_value);

/** @brief      Determines whether or not the given token represents a boolean value.
 *  @param[in]  p   The given token.
 *  @returns    Boolean true if the token is a boolean. Otherwise, false.
 *  @see        ucTok
 *
 *  Boolean values are those defined by @a ucTok_BOOLEAN_TRUE and @a ucTok_BOOLEAN_FALSE.
 */
uc_EXPORTED ucBool ucTok_is_boolean(ucTok *p);

/** @brief      Attempts to parse the given token to a boolean value.
 *  @param[in]  p       The given token.
 *  @param[out] value   A container for the parsed boolean value.
 *  @returns    Boolean true if the token was successfully parsed. Otherwise, false.
 *  @see        ucTok
 *
 *  Boolean values are those defined by @a ucTok_BOOLEAN_TRUE and @a ucTok_BOOLEAN_FALSE.
 *  If the token cannot be parsed to a boolean value, @a value is left unchanged.
 */
uc_EXPORTED ucBool ucTok_try_parse_boolean(ucTok *p, ucBool *value);

/** @brief      Parses the given token to a boolean value.
 *  @param[in]  p               The given token.
 *  @param[in]  default_value   If the token cannot be parsed to a boolean value, this value is returned.
 *  @returns    The boolean value of the token, or @a default_value if the token cannot be parsed.
 *  @see        ucTok
 *
 *  Boolean values are those defined by @a ucTok_BOOLEAN_TRUE and @a ucTok_BOOLEAN_FALSE.
 */
uc_EXPORTED ucBool ucTok_parse_boolean(ucTok *p, ucBool default_value);

/** @brief      Gets the token that proceeds from the given token in a command-line chain of tokens.
 *  @param[in]  p   The given token.
 *  @returns    The next token in the chain, or a null pointer if no further tokens exist.
 *
 *  The returned token's memory location is within the command-line chain. It is not allocated by
 *  this function and should not be freed by the caller.
 */
uc_EXPORTED ucTok *ucTok_get_next(ucTok*);

/** @} */

/*
 * Summary:
 *   A command structure. This structure consists
 *   of the parsed command and the ability to respond.
 */
typedef struct ucCmd ucCmd;

/*
 * Summary:
 *   The type of function used by a command structure
 *   to transmit responses.
 * Parameters:
 *   response: The string to be transmitted.
 *   state: A stateful object.
 */
typedef void (ucCmd_TransmitFunc)(const char *response, void *state);

/*
 * Summary:
 *   The type of function used by a command structure
 *   to determine whether or not the command is canceled.
 * Parameters:
 *   state: A stateful object.
 * Returns:
 *   ucBool_true if the command has been canceled. Otherwise, ucBool_false.
 */
typedef ucBool (ucCmd_IsCanceledFunc)(void *state);

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
 *   ucBool_true if the invalid command was handeled. Otherwise, ucBool_false.
 */
typedef ucBool (ucCmd_HandleInvalidCommandFunc)(const char *invalid_command, void *state);

/*
 * Summary:
 *   Gets the command token from the command structure.
 * Returns:
 *   A pointer to the command token of the structure.
 */
uc_EXPORTED ucTok *ucCmd_get_command(ucCmd*);

uc_EXPORTED ucTok *ucCmd_get_arg(ucCmd*);
uc_EXPORTED ucBool ucCmd_get_arg_b(ucCmd*, ucBool default_value);
uc_EXPORTED int ucCmd_get_arg_d(ucCmd*, int default_value);
uc_EXPORTED double ucCmd_get_arg_f(ucCmd*, double default_value);
uc_EXPORTED ucTok *ucCmd_get_arg_x(ucCmd*, int arg_index);
uc_EXPORTED ucBool ucCmd_get_arg_x_b(ucCmd*, int arg_index, ucBool default_value);
uc_EXPORTED int ucCmd_get_arg_x_d(ucCmd*, int arg_index, int default_value);
uc_EXPORTED double ucCmd_get_arg_x_f(ucCmd*, int arg_index, double default_value);

uc_EXPORTED ucTok *ucCmd_get_switch(ucCmd*);
uc_EXPORTED ucTok *ucCmd_get_switch_x(ucCmd*, int switch_index);

uc_EXPORTED ucTok *ucCmd_get_switch_arg(ucCmd*, const char *switch_name);
uc_EXPORTED int ucCmd_get_switch_arg_d(ucCmd *p, const char *switch_name, int default_value);
uc_EXPORTED int ucCmd_get_switch_arg_x_d(ucCmd *p, const char *switch_name, int arg_index, int default_value);
uc_EXPORTED double ucCmd_get_switch_arg_f(ucCmd *p, const char *switch_name, double default_value);
uc_EXPORTED double ucCmd_get_switch_arg_x_f(ucCmd *p, const char *switch_name, int arg_index, double default_value);

uc_EXPORTED ucBool ucCmd_get_switch_arg_b(ucCmd *p, const char *switch_name, ucBool default_value);
uc_EXPORTED ucTok *ucCmd_get_switch_arg_x(ucCmd *p, const char *switch_name, int arg_index);
uc_EXPORTED ucBool ucCmd_get_switch_arg_x_b(ucCmd *p, const char *switch_name, int arg_index, ucBool default_value);

uc_EXPORTED ucTok *ucCmd_find_switch(ucCmd*, const char *switch_name);

/*
 * Summary:
 *   Sets the command token for the structure.
 * Parameters:
 *   value: The command token.
 */
uc_EXPORTED void ucCmd_set_command(ucCmd*, ucTok *value);

/*
 * Summary:
 *   Sets the function used by the command structure to transmit responses.
 * Parameters:
 *   value: A pointer to the function used to transmit responses.
 */
uc_EXPORTED void ucCmd_set_transmit(ucCmd*, ucCmd_TransmitFunc *value);

/*
 * Summary:
 *   Gets the function used by the command structure to transmit responses.
 * Returns:
 *   A pointer to the function used by the structure to transmit responses.
 */
uc_EXPORTED ucCmd_TransmitFunc *ucCmd_get_transmit(ucCmd*);

/*
 * Summary:
 *   Gets the stateful object passed to the command's transmit function.
 * Returns:
 *   A pointer to the stateful object passed to the command's transmit function.
 */
uc_EXPORTED void *ucCmd_get_transmit_state(ucCmd*);

/*
 * Summary:
 *   Sets the stateful object passed to the command's transmit function.
 * Parameters:
 *   value: A pointer to the stateful object that is passed to the command's transmit function.
 */
uc_EXPORTED void ucCmd_set_transmit_state(ucCmd*, void *value);

/*
 * Summary:
 *   Determines whether or not the command has been canceled.
 * Returns:
 *   ucBool_true if the command has been canceled. Otherwise, ucBool_false.
 */
uc_EXPORTED ucBool ucCmd_is_canceled(ucCmd*);

/*
 * Summary:
 *   Sets the function used by the command structure to check for cancellation.
 * Parameters:
 *   value: A pointer to the function used to check for cancellation.
 */
uc_EXPORTED void ucCmd_set_is_canceled(ucCmd*, ucCmd_IsCanceledFunc *value);

/*
 * Summary:
 *   Gets the function used by the command structure to check for cancellation.
 * Returns:
 *   A pointer to the function used to check for cancellation.
 */
uc_EXPORTED ucCmd_IsCanceledFunc *ucCmd_get_is_canceled(ucCmd*);

/*
 * Summary:
 *   Gets the stateful object passed to the command's cancellation function.
 * Returns:
 *   A pointer to the stateful object passed to the command's cancellation function.
 */
uc_EXPORTED void *ucCmd_get_is_canceled_state(ucCmd*);

/*
 * Summary:
 *   Sets the stateful object passed to the command's cancellation function.
 * Parameters:
 *   value: The stateful object passed to the command's cancellation function.
 */
uc_EXPORTED void ucCmd_set_is_canceled_state(ucCmd*, void *value);

uc_EXPORTED void ucCmd_set_handle_invalid_command(ucCmd*, ucCmd_HandleInvalidCommandFunc *value);

uc_EXPORTED ucCmd_HandleInvalidCommandFunc *ucCmd_get_handle_invalid_command(ucCmd*);

uc_EXPORTED void ucCmd_set_handle_invalid_command_state(ucCmd*, void *value);

uc_EXPORTED void *ucCmd_get_handle_invalid_command_state(ucCmd*);

uc_EXPORTED size_t ucCmd_get_response_size_max(ucCmd*);

uc_EXPORTED ucCmd *ucCmd_create(void);
uc_EXPORTED void ucCmd_destroy(ucCmd*);

/*
 * Summary:
 *   Formats the command's response using the given parameters.
 * Parameters:
 *   format: The format string.
 *   ...: Parameters to the format string.
 * Returns:
 *   The formatted string.
 */
uc_EXPORTED const char *ucCmd_format_response(ucCmd*, const char *format, ...);

/*
 * Summary:
 *   Formats the command's response using the given parameters.
 * Parameters:
 *   format: The format string.
 *   arg_list: A variable length argument list with parameters to the format string.
 * Returns:
 *   The formatted string.
 */
uc_EXPORTED const char *ucCmd_format_response_va(ucCmd*, const char *format, va_list arg_list);

/*
 * Summary
 *   Responds to the command.
 * Parameters:
 *   response: The response string.
 */
uc_EXPORTED void ucCmd_respond(ucCmd*, const char *response);

/*
 * Summary:
 *   Sets whether or not the command structure is quiet, meaning no response
 *   strings will be sent.
 * Parameters:
 *   value: A boolean true value if the command structure should not send
 *          any response strings. Otherwise, false.
 */
uc_EXPORTED void ucCmd_set_is_quiet(ucCmd*, ucBool value);

/*
 * Summary:
 *   Gets a flag that indicates whether or not the command structure is quiet,
 *   meaning no response strings are sent.
 * Returns:
 *   A boolean true value if the command structure is not sending any
 *   response strings. Otherwise, false.
 */
uc_EXPORTED ucBool ucCmd_get_is_quiet(ucCmd*);


uc_EXPORTED const char*     ucCmd_get_command_acknowledgment(ucCmd*);
uc_EXPORTED const char*     ucCmd_get_response_terminator(ucCmd*);
uc_EXPORTED void            ucCmd_set_response_terminator(ucCmd*, const char *value);
uc_EXPORTED void            ucCmd_set_command_acknowledgment(ucCmd*, const char *value);


/*
 * Summary:
 *   Base type for command, argument, and switch options.
 */
typedef struct ucOpt ucOpt;

/*
 * Summary:
 *   Gets the name of the option.
 * Returns:
 *   The name of the option.
 */
uc_EXPORTED const char* ucOpt_get_name(ucOpt*);

/*
 * Summary:
 *   Gets the description of the option.
 * Returns:
 *   The description of the option.
 */
uc_EXPORTED const char *ucOpt_get_desc(ucOpt*);

/*
 * Summary:
 *   Gets a flag that indicates whether or not the option
 *   is required.
 * Returns:
 *   ucBool_true if the option is required. Otherwise, ucBool_false.
 */
uc_EXPORTED ucBool ucOpt_is_required(ucOpt*);

/*
 * Summary:
 *   An argument option. This type is a child
 *   of the base option type.
 */
typedef struct ucArgOpt ucArgOpt;

/*
 * Summary:
 *   Gets the minimum allowed token count of the argument option.
 * Returns:
 *   The minimum number of argument tokens allowed for this option.
 */
uc_EXPORTED int ucArgOpt_get_min_tok_count(ucArgOpt*);

/*
 * Summary:
 *   Gets the maximum allowed token count of the argument option.
 * Returns:
 *   The maximum number of argument tokens allowed for this option.
 */
uc_EXPORTED int ucArgOpt_get_max_tok_count(ucArgOpt*);

uc_EXPORTED ucBool ucArgOpt_is_boolean(ucArgOpt*);
uc_EXPORTED ucBool ucArgOpt_is_integer(ucArgOpt*);

/*
 * Summary:
 *   Gets a flag that indicates whether or not this argument option
 *   is numeric.
 * Returns:
 *   ucBool_true if the argument is numeric. Otherwise, ucBool_false.
 */
uc_EXPORTED ucBool ucArgOpt_is_numeric(ucArgOpt*);

/*
 * Summary:
 *   Gets the minimum value if this argument option is numeric.
 * Returns:
 *   The minimum numeric value of the argument.
 */
uc_EXPORTED double ucArgOpt_get_numeric_min(ucArgOpt*);

/*
 * Summary:
 *   Gets the maximum value if this argument is numeric.
 * Returns:
 *   The maximum numeric value of the argument.
 */
uc_EXPORTED double ucArgOpt_get_numeric_max(ucArgOpt*);

/*
 * Summary:
 *   Creates a new argument option.
 * Parameters:
 *   name: The name of the option.
 *   desc: The description of the option.
 *   next: The next option in the chain that the created option precedes,
 *         or NULL if the created option is the last.
 * Returns:
 *   A pointer to the newly created argument option.
 */
uc_EXPORTED ucArgOpt *ucArgOpt_create(const char *name, const char *desc, ucArgOpt *next);

/*
 * Summary:
 *   Creates a new argument option that allows multiple tokens.
 * Parameters:
 *   name: The name of the option.
 *   desc: The description of the option.
 *   min_tok_count: The minimum number of argument tokens allowed for this option.
 *   max_tok_count: The maximum number of argument tokens allowed for this option.
 * Returns:
 *   A pointer to the newly created argument option.
 */
uc_EXPORTED ucArgOpt *ucArgOpt_create_multiple(const char *name, const char *desc, int min_tok_count, int max_tok_count);

/*
 * Summary:
 *   Creates a new, required argument option.
 * Parameters:
 *   name: The name of the option.
 *   desc: The description of the option.
 *   next: The next option in the chain that the created option precedes,
 *         or NULL if the created option is the last.
 * Returns:
 *   A pointer to the newly created argument option. The option will have its
 *   'required' property set to true.
 */
uc_EXPORTED ucArgOpt *ucArgOpt_create_required(const char *name, const char *desc, ucArgOpt *next);

uc_EXPORTED ucArgOpt *ucArgOpt_create_boolean(const char *desc, ucArgOpt *next);
uc_EXPORTED ucArgOpt *ucArgOpt_create_required_boolean(const char *desc, ucArgOpt *next);
uc_EXPORTED ucArgOpt *ucArgOpt_create_integer(const char *desc, double numeric_min, double numeric_max, ucArgOpt *next);
uc_EXPORTED ucArgOpt *ucArgOpt_create_multiple_integer(const char *desc, int min_tok_count, int max_tok_count, double numeric_min, double numeric_max);
uc_EXPORTED ucArgOpt *ucArgOpt_create_required_integer(const char *desc, double numeric_min, double numeric_max, ucArgOpt *next);

/*
 * Summary:
 *   Creates a new, numeric argument option.
 * Parameters:
 *   desc: The description of the argument.
 *   numeric_min: The minimum value of the argument.
 *   numeric_max: The maximum value of the argument.
 *   next: A pointer to the next option in the chain that the created option precedes,
 *         or NULL if the created option is the last.
 * Returns:
 *   A pointer to the newly created argument option.
 */
uc_EXPORTED ucArgOpt *ucArgOpt_create_numeric(const char *desc, double numeric_min, double numeric_max, ucArgOpt *next);

/*
 * Summary:
 *   Creates a new, numeric argument option that accepts multiple argument tokens.
 * Parameters:
 *   desc: The description of the argument.
 *   min_tok_count: The minimum number of allowed argument tokens.
 *   max_tok_count: The maximum number of allowed argument tokens.
 *   numeric_min: The minimum value of the argument.
 *   numeric_max: The maximum value of the argument.
 * Returns:
 *   A pointer to the newly created argument option.
 */
uc_EXPORTED ucArgOpt *ucArgOpt_create_multiple_numeric(const char *desc, int min_tok_count, int max_tok_count, double numeric_min, double numeric_max);

/*
 * Summary:
 *   Creates a new, numeric, required argument option.
 * Parameters:
 *   desc: The description of the argument.
 *   numeric_min: The minimum value of the argument.
 *   numeric_max: The maximum value of the argument.
 *   next: A pointer to the next option in the chain that the created option precedes,
 *         or NULL if the created option is the last.
 * Returns:
 *   A pointer to the newly created argument option. The option will have its
 *   'required' property set to true.
 */
uc_EXPORTED ucArgOpt *ucArgOpt_create_required_numeric(const char *desc, double numeric_min, double numeric_max, ucArgOpt *next);

/*
 * Summary:
 *   Gets the next argument option after the given option.
 * Returns:
 *   A pointer to the option that the given option precedes,
 *   or NULL of no further options exist.
 */
uc_EXPORTED ucArgOpt *ucArgOpt_get_next(ucArgOpt*);

/*
 * Summary:
 *   Releases memory used by the argument option.
 */
uc_EXPORTED void ucArgOpt_destroy(ucArgOpt*);

/*
 * Summary:
 *   Releases memory used by the argument option
 *   and all proceeding options in the list.
 */
uc_EXPORTED void ucArgOpt_destroy_chain(ucArgOpt*);

/*
 * Summary:
 *   A command switch option. This type is a child
 *   of the base option type.
 */
typedef struct ucSwitchOpt ucSwitchOpt;

/*
 * Summary:
 *   Creates a new switch option.
 * Parameters:
 *   name: The name of the switch.
 *   desc: A description of the switch.
 *   arg_opt: The first argument option of the switch.
 *   next: The switch option that the created option precedes,
 *         or NULL if no further switch options exist.
 * Returns:
 *   A pointer to the newly created switch option.
 */
uc_EXPORTED ucSwitchOpt *ucSwitchOpt_create(const char *name, const char *desc, ucArgOpt *arg_opt, ucSwitchOpt *next);

/*
 * Summary:
 *   Creates a new, required switch option.
 * Parameters:
 *   name: The name of the switch.
 *   desc: A description of the switch.
 *   arg_opt: The first argument option of the switch.
 *   next: The switch option that the created option precedes,
 *         or NULL if no further switch options exist.
 * Returns:
 *   A pointer to the newly created switch option. The option's
 *   'required' property will be set to true.
 */
uc_EXPORTED ucSwitchOpt *ucSwitchOpt_create_required(const char *name, const char *desc, ucArgOpt *arg_opt, ucSwitchOpt *next);

/*
 * Summary:
 *   Gets the first argument option of the given switch.
 * Returns:
 *   A pointer to the first argument option of the switch, or NULL
 *   if no argument options exist.
 */
uc_EXPORTED ucArgOpt *ucSwitchOpt_get_arg_opt(ucSwitchOpt*);

/*
 * Summary:
 *   Finds the switch option in the linked list with the given name.
 * Parameters:
 *   name: The name of the switch option to be found.
 * Returns:
 *   The switch option with the given name, or NULL if
 *   no switch option is found.
 */
uc_EXPORTED ucSwitchOpt *ucSwitchOpt_find(ucSwitchOpt*, const char *name);

/*
 * Summary:
 *   Gets the next switch option.
 * Returns:
 *   A pointer to the next switch option in the list, or NULL
 *   if no further options exist.
 */
uc_EXPORTED ucSwitchOpt *ucSwitchOpt_get_next(ucSwitchOpt*);

/*
 * Summary:
 *   Releases memory used by the switch option.
 */
uc_EXPORTED void ucSwitchOpt_destroy(ucSwitchOpt*);

/*
 * Summary:
 *   Releases memory used by the switch option and all
 *   proceeding options in the list. All memory used by
 *   any argument options belonging to the switches in
 *   the list is also released.
 */
uc_EXPORTED void ucSwitchOpt_destroy_chain(ucSwitchOpt*);

/*
 * Summary:
 *   A command option. This type is a child of the
 *   base option type.
 */
typedef struct ucCmdOpt ucCmdOpt;

/*
 * Summary:
 *   Defines the signature of the function called when a command option is processed.
 * Parameters:
 *   cmd: The parsed command structure that represents the function parameters.
 *   state: The state pointer with which the command option was created.
 * Returns:
 *   A message that can be used to respond to the command.
 */
typedef const char *(ucCmdOpt_WorkFunc)(ucCmd *cmd, void *state);

/*
 * Summary:
 *   Creates a new command option.
 * Parameters:
 *   work: A pointer to the function that is called when this command is invoked or selected.
 *   state: A pointer that gets passed to the function to maintain state.
 *   name: The name of the command.
 *   desc: The description of the command.
 *   arg_opt: The argument options available to the command.
 *   switch_opt: The switch options available to the command.
 *   next: The next command that the created command precedes, or NULL if no further commands exist.
 * Returns:
 *   A pointer to the newly created command option.
 */
uc_EXPORTED ucCmdOpt *ucCmdOpt_create(ucCmdOpt_WorkFunc *work, void *state, const char *name, const char *desc, ucArgOpt *arg_opt, ucSwitchOpt *switch_opt, ucCmdOpt* next);

/*
 * Summary:
 *   Gets the next command option after the given option.
 * Returns:
 *   A pointer to the next command option.
 */
uc_EXPORTED ucCmdOpt *ucCmdOpt_get_next(ucCmdOpt*);

/*
 * Summary:
 *   Gets the first argument option of the command.
 * Returns:
 *   A pointer to the first argument option of the command,
 *   or NULL if no argument options exist.
 */
uc_EXPORTED ucArgOpt *ucCmdOpt_get_arg_opt(ucCmdOpt*);

/*
 * Summary:
 *   Gets the first switch option of the command option.
 * Returns:
 *   A pointer to the first switch option of the command option,
 *   or NULL if no switch options exist.
 */
uc_EXPORTED ucSwitchOpt *ucCmdOpt_get_switch_opt(ucCmdOpt*);

/*
 * Summary:
 *   Finds the command option that matches the given name.
 * Parameters:
 *   name: The name of the command whose option is to be found.
 * Returns:
 *   A pointer to the command option that matches the given name, or NULL
 *   if no option is found.
 */
uc_EXPORTED ucCmdOpt *ucCmdOpt_find_by_name(ucCmdOpt*, const char *name);

/*
 * Summary:
 *   Gets the pointer to the function invoked when the command option
 *   is processed.
 * Returns:
 *   A pointer to the function invoked when the command option is processed.
 */
uc_EXPORTED ucCmdOpt_WorkFunc *ucCmdOpt_get_work(ucCmdOpt*);

/*
 * Summary:
 *   Gets the state pointer that is passed to the command option's function
 *   when it is invoked.
 * Returns:
 *   A pointer to the command option's state.
 */
uc_EXPORTED void *ucCmdOpt_get_state(ucCmdOpt*);

/*
 * Summary:
 *   Releases memory used by the command option.
 */
uc_EXPORTED void ucCmdOpt_destroy(ucCmdOpt*);

/*
 * Summary:
 *   Releases memory used by the command option and all proceeding options
 *   in the list. All memory used by any argument options, switch options,
 *   and switch-argument options is also released.
 */
uc_EXPORTED void ucCmdOpt_destroy_chain(ucCmdOpt*);

/*
 * Summary:
 *   An application that runs with a set of command options.
 */
typedef struct ucCmdApp ucCmdApp;

/*
 * Summary:
 *   The type of the function used by an application to
 *   receive data.
 * Parameters:
 *   buf: A string buffer that can be used to store the data received.
 *   buf_size: The size of the string buffer used to store received data.
 *   state: A stateful object.
 * Returns:
 *   The data that was received.
 */
typedef char *(ucCmdApp_ReceiveFunc)(char *buf, size_t buf_size, void *state);

/*
 * Summary:
 *   Sets the escape string that will cause the app to exit.
 * Parameters:
 *   value: The escape string that, when returned in a response,
 *          causes the app to exit.
 */
uc_EXPORTED void ucCmdApp_set_escape_response(ucCmdApp*, const char *value);

/*
 * Summary:
 *   Gets the escape string that causes the app to exit.
 * Returns:
 *   The escape string that, when returned in a response,
 *   causes the app to exit.
 */
uc_EXPORTED const char *ucCmdApp_get_escape_response(ucCmdApp*);

/*
 * Summary:
 *   Runs the application with the given options.
 * Parameters:
 *   cmd_opt: A pointer to the first command option for the app.
 */ 
uc_EXPORTED void ucCmdApp_run(ucCmdApp*, ucCmdOpt *cmd_opt);

uc_EXPORTED ucCmdApp *ucCmdApp_create(void);
uc_EXPORTED void ucCmdApp_destroy(ucCmdApp *p);

/*
 * Summary:
 *   Sets the function that the app uses to receive data.
 * Parameters:
 *   value: A pointer to the function used to receive data.
 */
uc_EXPORTED void ucCmdApp_set_receive(ucCmdApp*, ucCmdApp_ReceiveFunc *value);

/*
 * Summary:
 *   Gets the function that the app uses to receive data.
 * Returns:
 *   A pointer to the function used to receive data.
 */ 
uc_EXPORTED ucCmdApp_ReceiveFunc *ucCmdApp_get_receive(ucCmdApp*);

/*
 * Summary:
 *   Gets the stateful object passed to the application's receive function.
 * Returns:
 *   A pointer to the stateful object passed to the application's receive function.
 */
uc_EXPORTED void *ucCmdApp_get_receive_state(ucCmdApp*);

/*
 * Summary:
 *   Sets the stateful object passed to the application's receive function.
 * Parameters:
 *   value: The stateful object passed to the application's receive function.
 */
uc_EXPORTED void ucCmdApp_set_receive_state(ucCmdApp*, void *value);

/*
 * Summary:
 *   Sets the command used to quit the application.
 * Parameters:
 *   value: The value of the command that quits the application.
 */
uc_EXPORTED void ucCmdApp_set_quit_command(ucCmdApp*, const char *value);

/*
 * Summary:
 *   Gets the value of the command that quits the application.
 * Returns:
 *   The value of the command that quits the application.
 */
uc_EXPORTED const char *ucCmdApp_get_quit_command(ucCmdApp*);

/*
 * Summary:
 *   Sets the value of the command that shows help information.
 * Parameters:
 *   value: The value of the command that shows help information.
 */
uc_EXPORTED void ucCmdApp_set_help_command(ucCmdApp*, const char *value);

/*
 * Summary:
 *   Gets the value of the command that shows help information.
 * Returns:
 *   The value of the command that shows help information.
 */
uc_EXPORTED const char *ucCmdApp_get_help_command(ucCmdApp*);

/*
 * Summary:
 *   Gets the command structure used by the application.
 * Returns:
 *   A pointer to the command structure used by the application.
 */
uc_EXPORTED ucCmd *ucCmdApp_get_cmd(ucCmdApp*);

/*
 * Summary:
 *   Gets the size of the application's command-string buffer.
 * Returns:
 *   The size of the command-string buffer.
 */
uc_EXPORTED size_t ucCmdApp_get_cmd_str_size_max(ucCmdApp *p);

#endif
