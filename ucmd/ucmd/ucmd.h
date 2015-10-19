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
 *  @date 16 October 2015
 *  @version 1.9
 *  @brief Public declarations for the ucmd library and framework.
 *  @see https://github.com/kyourek/ucmd
 *
 *  These are the types and functions made publicly available by ucmd.
 */

#ifndef     _UCMD_H
#define     _UCMD_H

#include    <stdarg.h>
#include    <stddef.h>

/* Sets the size of the command buffer when using
   the command-line application framework. All
   entered commands must have a size equal to or
   less than this buffer's size. */
#ifndef ucCmdLineApp_CMD_STR_SIZE
#define ucCmdLineApp_CMD_STR_SIZE 200
#endif

/* Sets the size of the command response buffer.
   All response strings must have a size equal to
   or less than the size of this buffer to avoid
   truncation. */
#ifndef ucCmdLine_RESPONSE_SIZE
#define ucCmdLine_RESPONSE_SIZE 200
#endif

/* Sets the number of available command options.
   The number of created command options must be
   equal to or less than this number. */
#ifndef ucCmdLineOpt_COUNT
#define ucCmdLineOpt_COUNT 10
#endif

/* Sets the number of available switch options.
   The number of created switch options must be
   equal to or less than this number. */
#ifndef ucSwitchOpt_COUNT
#define ucSwitchOpt_COUNT 50
#endif

/* Sets the number of available argument options.
   This is the total number of options available
   to commands and switches, combined. */
#ifndef ucArgOpt_COUNT
#define ucArgOpt_COUNT 50
#endif

/* Sets the maximum expected length of a single
   token in a command line. */
#ifndef ucTok_LENGTH_MAX
#define ucTok_LENGTH_MAX ucCmdLineApp_CMD_STR_SIZE
#endif

#ifndef ucTok_BOOLEAN_TRUE
#define ucTok_BOOLEAN_TRUE "1", "on", "yes", "true"
#endif

#ifndef ucTok_BOOLEAN_FALSE
#define ucTok_BOOLEAN_FALSE "0", "off", "no", "false"
#endif

/* Sets the numeric type of numeric argument
   options. */
#ifndef ucArgOpt_NUMERIC_TYPE
#define ucArgOpt_NUMERIC_TYPE double
#endif

/* Include this def when using the library with another
   program on Windows.
   Exported functions will be decorated with dllimport
   to make them available to external programs. */
#ifdef uc_DECLSPEC_DLLIMPORT
#define uc_EXPORTED uc_EXTERN_C __declspec(dllimport)
#endif

/* Include this def when compiling this program on
   Windows.
   Exported functions will be decorated with dllexport
   to make them available to external programs. */
#ifdef uc_DECLSPEC_DLLEXPORT
#define uc_EXPORTED uc_EXTERN_C __declspec(dllexport)
#endif

/* Prepend extern "C" if we're in a C++
   compiler. */
#ifdef __cplusplus
#define uc_EXTERN_C extern "C"
#else
#define uc_EXTERN_C
#endif

/* Default to setting uc_EXPORTED to the result
   of our extern "C" check. */
#ifndef uc_EXPORTED
#define uc_EXPORTED uc_EXTERN_C
#endif

/* Define NULL, if it hasn't been defined. */
#ifndef NULL
#define NULL ((void*)0)
#endif

/*
 * Summary:
 *   Definition for the type returned
 *   by functions that use an error code.
 */
typedef int ucErr;

/*
 * Summary:
 *   Defines the value that represents no error.
 */
#define ucErr_NONE 0

/** @brief Boolean type definition.
 *
 *  This definition is used to increase the readability of the source by replacing
 *  integer representations of boolean values with the more familiar "true" and
 *  "false" values. 
 */
typedef enum ucBool {
    ucBool_FALSE = 0,               /**< Falsey value. */
    ucBool_TRUE = !ucBool_FALSE     /**< Truthy value. */
} ucBool;

/*
 * Summary:
 *   Base structure for tokenized values in a command.
 */
typedef const char ucTok;

/** @brief Gets the length of the token.
 *  @returns The number of characters in the token. 
 */
uc_EXPORTED int ucTok_get_length(ucTok*);

/*
 * Summary:
 *   Determines whether or not the given token equals the value.
 * Parameters:
 *   value: The value against which the token is checked for equality.
 * Returns:
 *   ucBool_TRUE if the token value equals the given value. Otherwise, ucBool_FALSE.
 */
uc_EXPORTED ucBool ucTok_equals(ucTok*, const char *value);

uc_EXPORTED ucBool ucTok_is_integer(ucTok *p);
uc_EXPORTED ucBool ucTok_try_parse_integer(ucTok *p, int *value);
uc_EXPORTED int ucTok_parse_integer(ucTok *p);
uc_EXPORTED ucBool ucTok_is_numeric(ucTok*);
uc_EXPORTED ucBool ucTok_try_parse_numeric(ucTok*, double *value);
uc_EXPORTED double ucTok_parse_numeric(ucTok*);
uc_EXPORTED ucBool ucTok_is_boolean(ucTok*);
uc_EXPORTED ucBool ucTok_try_parse_boolean(ucTok*, ucBool *value);
uc_EXPORTED ucBool ucTok_parse_boolean(ucTok*);

/*
 * Summary:
 *   Determines whether or not the given token is considered a switch.
 * Returns:
 *   ucBool_TRUE if the token is a switch. Otherwise, ucBool_FALSE.
 */
uc_EXPORTED ucBool ucTok_is_switch(ucTok*);

/*
 * Summary:
 *   Gets the next token in the list.
 * Returns:
 *   A pointer to the token that comes next in the list, or NULL
 *   if no further tokens exist.
 */
uc_EXPORTED ucTok *ucTok_get_next(ucTok*);

/*
 * Summary:
 *   Counts the number of tokens in the linked list.
 * Returns:
 *   The number of tokens in the list.
 */
uc_EXPORTED int ucTok_count(ucTok*);

/*
 * Summary:
 *   Gets the value of the token.
 * Returns:
 *   The string value of the token.
 */
uc_EXPORTED const char *ucTok_get_value(ucTok*);

/*
 * Summary:
 *   An argument token. This type is a child of the
 *   base token type. All functions that take an 
 *   instance of the base type can be used with an 
 *   instance of this type.
 */
typedef const char ucArgTok;

/*
 * Summary:
 *   Gets the next argument after the given argument.
 * Returns:
 *   The next argument in the list.
 */
uc_EXPORTED ucArgTok *ucArgTok_get_next(ucArgTok*);

/*
 * Summary:
 *   Counts the number of arguments in the linked list.
 * Returns:
 *   The number of arguments in the list.
 */
uc_EXPORTED int ucArgTok_count(ucArgTok*);

/*
 * Summary:
 *   Finds the argument in the list with the specified value.
 * Parameters:
 *   arg_value: The value of the argument to find.
 * Returns:
 *   The argument with the specified value, or NULL if none exists.
 */
uc_EXPORTED ucArgTok *ucArgTok_find(ucArgTok*, const char *arg_value);

/*
 * Summary:
 *   Gets a value indicating whether or not the value exists in
 *   the argument list.
 * Parameters:
 *   arg_value: The value of the argument to be found.
 * Returns:
 *   ucBool_TRUE if an argument token with the given value is found.
 *   Otherwise, ucBool_FALSE.
 */
uc_EXPORTED ucBool ucArgTok_contains(ucArgTok*, const char *arg_value);

/*
 * Summary:
 *   Type definition for tokens (i.e. switch and command tokens)
 *   that contain arguments. This type is a child of the base
 *   token type. All functions that take an instance of the base
 *   type can be used with an instance of this type.
 */
typedef const char ucArgTokOwner;

/*
 * Summary
 *   Gets the first argument that belongs to the given owner.
 * Returns:
 *   The first argument that belongs to the owner, or NULL if
 *   no arguments exist.
 */
uc_EXPORTED ucArgTok *ucArgTokOwner_get_arg(ucArgTokOwner*);

/*
 * Summary:
 *   A switch token. This type is a child of the
 *   base token type. All functions that take an 
 *   instance of the base type can be used with an 
 *   instance of this type.
 */
typedef const char ucSwitchTok;

/*
 * Summary:
 *   Gets the next switch token after the given token.
 * Returns:
 *   The next switch token after the given token.
 */
uc_EXPORTED ucSwitchTok *ucSwitchTok_get_next(ucSwitchTok*);

/*
 * Summary:
 *   Counts the number of switches in the linked list.
 * Returns:
 *   The number of switches in the list.
 */
uc_EXPORTED int ucSwitchTok_count(ucSwitchTok*);

/*
 * Summary:
 *   Finds the switch with the specified value.
 * Parameters:
 *   switch_value: The value of the switch to be found.
 * Returns:
 *   The switch with the specified value, or NULL if none exist.
 */
uc_EXPORTED ucSwitchTok *ucSwitchTok_find(ucSwitchTok*, const char *switch_value);

/*
 * Summary:
 *   Gets a value indicating whether or not a value exists in the switch list.
 * Parameters:
 *   switch_value: The value of the switch to be found.
 * Returns:
 *   ucBool_TRUE if a switch with the given value was found in the list. Otherwise,
 *   ucBool_FALSE.
 */
uc_EXPORTED ucBool ucSwitchTok_contains(ucSwitchTok*, const char *switch_value);

/*
 * Summary:
 *   Gets the first argument token of the switch.
 * Returns:
 *   A pointer to the first argument of the switch, or NULL
 *   if no arguments exist.
 */
uc_EXPORTED ucArgTok *ucSwitchTok_get_arg(ucSwitchTok*);

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
 * Returns:
 *   A pointer to the command token.
 */
uc_EXPORTED ucCmdTok *ucCmdLineToks_get_cmd_tok(ucCmdLineToks*);

/*
 * Summary:
 *   The command's first argument token, or
 *   NULL if no arguments exist.
 * Returns:
 *   A pointer to the argument token, or NULL if no argument
 *   tokens exist.
 */
uc_EXPORTED ucArgTok *ucCmdLineToks_get_arg_tok(ucCmdLineToks*);

/*
 * Summary:
 *   The command's first switch token, or
 *   NULL if no switches exist.
 * Returns:
 *   A pointer to the switch token, or NULL if no switch tokens exist.
 */
uc_EXPORTED ucSwitchTok *ucCmdLineToks_get_switch_tok(ucCmdLineToks*);

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

uc_EXPORTED size_t ucCmdLine_get_response_size_max(ucCmdLine*);

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

uc_EXPORTED void            ucCmdLine_acknowledge_command(ucCmdLine*);
uc_EXPORTED const char*     ucCmdLine_get_command_acknowledgment(ucCmdLine*);
uc_EXPORTED const char*     ucCmdLine_get_response_terminator(ucCmdLine*);
uc_EXPORTED void            ucCmdLine_set_response_terminator(ucCmdLine*, const char *value);
uc_EXPORTED void            ucCmdLine_set_command_acknowledgment(ucCmdLine*, const char *value);
uc_EXPORTED void            ucCmdLine_terminate_response(ucCmdLine*);

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
 *   ucBool_TRUE if the option is required. Otherwise, ucBool_FALSE.
 */
uc_EXPORTED ucBool ucOpt_is_required(ucOpt*);

/*
 * Summary:
 *   Uses the provided command structure to send help information
 *   for this option.
 * Parameters:
 *   cmd: A pointer to the command structure used to respond
 *        with the help information.
 *   prefix: A string used to prefix the help information.
 */
uc_EXPORTED void ucOpt_send_help(ucOpt*, ucCmdLine *cmd, const char *prefix);

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
 *   ucBool_TRUE if the argument is numeric. Otherwise, ucBool_FALSE.
 */
uc_EXPORTED ucBool ucArgOpt_is_numeric(ucArgOpt*);

/*
 * Summary:
 *   Gets the minimum value if this argument option is numeric.
 * Returns:
 *   The minimum numeric value of the argument.
 */
uc_EXPORTED ucArgOpt_NUMERIC_TYPE ucArgOpt_get_numeric_min(ucArgOpt*);

/*
 * Summary:
 *   Gets the maximum value if this argument is numeric.
 * Returns:
 *   The maximum numeric value of the argument.
 */
uc_EXPORTED ucArgOpt_NUMERIC_TYPE ucArgOpt_get_numeric_max(ucArgOpt*);

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
uc_EXPORTED ucArgOpt *ucArgOpt_create_integer(const char *desc, ucArgOpt_NUMERIC_TYPE numeric_min, ucArgOpt_NUMERIC_TYPE numeric_max, ucArgOpt *next);
uc_EXPORTED ucArgOpt *ucArgOpt_create_multiple_integer(const char *desc, int min_tok_count, int max_tok_count, ucArgOpt_NUMERIC_TYPE numeric_min, ucArgOpt_NUMERIC_TYPE numeric_max);
uc_EXPORTED ucArgOpt *ucArgOpt_create_required_integer(const char *desc, ucArgOpt_NUMERIC_TYPE numeric_min, ucArgOpt_NUMERIC_TYPE numeric_max, ucArgOpt *next);

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
uc_EXPORTED ucArgOpt *ucArgOpt_create_numeric(const char *desc, ucArgOpt_NUMERIC_TYPE numeric_min, ucArgOpt_NUMERIC_TYPE numeric_max, ucArgOpt *next);

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
uc_EXPORTED ucArgOpt *ucArgOpt_create_multiple_numeric(const char *desc, int min_tok_count, int max_tok_count, ucArgOpt_NUMERIC_TYPE numeric_min, ucArgOpt_NUMERIC_TYPE numeric_max);

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
uc_EXPORTED ucArgOpt *ucArgOpt_create_required_numeric(const char *desc, ucArgOpt_NUMERIC_TYPE numeric_min, ucArgOpt_NUMERIC_TYPE numeric_max, ucArgOpt *next);

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
 *   Base structure for options (switches and commands)
 *   that contain argument options. This type is a child
 *   of the base option type.
 */
typedef struct ucArgOptOwner ucArgOptOwner;

/*
 * Summary:
 *   Gets the first argument option of the given option owner.
 * Returns:
 *   A pointer to the first argument option of the given option owner.
 */
uc_EXPORTED ucArgOpt *ucArgOptOwner_get_arg_opt(ucArgOptOwner*);

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
uc_EXPORTED ucCmdParser *ucCmdParser_get_instance(void);

/*
 * Summary:
 *   Parses a command so that it can be used as a command token.
 * Parameters:
 *   cmd: The command string to be parsed. This string is mutated.
 * Returns:
 *   A pointer to the command token that was parsed, or NULL if an
 *   error occurred.
 */
uc_EXPORTED ucCmdTok *ucCmdParser_parse(ucCmdParser*, char *cmd);

/*
 * Summary:
 *   A command option. This type is a child of the
 *   base option type.
 */
typedef struct ucCmdLineOpt ucCmdLineOpt;

/*
 * Summary:
 *   Defines the signature of the function called when a command option is processed.
 * Parameters:
 *   cmd: The parsed command structure that represents the function parameters.
 *   state: The state pointer with which the command option was created.
 * Returns:
 *   A message that can be used to respond to the command.
 */
typedef const char *(ucCmdLineOpt_Func)(ucCmdLine *cmd, void *state);

/*
 * Summary:
 *   Creates a new command option.
 * Parameters:
 *   func: A pointer to the function that is called when this command is invoked or selected.
 *   state: A pointer that gets passed to the function to maintain state.
 *   name: The name of the command.
 *   desc: The description of the command.
 *   arg_opt: The argument options available to the command.
 *   switch_opt: The switch options available to the command.
 *   next: The next command that the created command precedes, or NULL if no further commands exist.
 * Returns:
 *   A pointer to the newly created command option.
 */
uc_EXPORTED ucCmdLineOpt *ucCmdLineOpt_create(ucCmdLineOpt_Func *func, void *state, const char *name, const char *desc, ucArgOpt *arg_opt, ucSwitchOpt *switch_opt, ucCmdLineOpt* next);

/*
 * Summary:
 *   Gets the next command option after the given option.
 * Returns:
 *   A pointer to the next command option.
 */
uc_EXPORTED ucCmdLineOpt *ucCmdLineOpt_get_next(ucCmdLineOpt*);

/*
 * Summary:
 *   Gets the first argument option of the command.
 * Returns:
 *   A pointer to the first argument option of the command,
 *   or NULL if no argument options exist.
 */
uc_EXPORTED ucArgOpt *ucCmdLineOpt_get_arg_opt(ucCmdLineOpt*);

/*
 * Summary:
 *   Gets the first switch option of the command option.
 * Returns:
 *   A pointer to the first switch option of the command option,
 *   or NULL if no switch options exist.
 */
uc_EXPORTED ucSwitchOpt *ucCmdLineOpt_get_switch_opt(ucCmdLineOpt*);

/*
 * Summary:
 *   Finds the command option that matches the given name.
 * Parameters:
 *   name: The name of the command whose option is to be found.
 * Returns:
 *   A pointer to the command option that matches the given name, or NULL
 *   if no option is found.
 */
uc_EXPORTED ucCmdLineOpt *ucCmdLineOpt_find_by_name(ucCmdLineOpt*, const char *name);

/*
 * Summary:
 *   Gets the pointer to the function invoked when the command option
 *   is processed.
 * Returns:
 *   A pointer to the function invoked when the command option is processed.
 */
uc_EXPORTED ucCmdLineOpt_Func *ucCmdLineOpt_get_func(ucCmdLineOpt*);

/*
 * Summary:
 *   Gets the state pointer that is passed to the command option's function
 *   when it is invoked.
 * Returns:
 *   A pointer to the command option's state.
 */
uc_EXPORTED void *ucCmdLineOpt_get_state(ucCmdLineOpt*);

/*
 * Summary:
 *   Releases memory used by the command option.
 */
uc_EXPORTED void ucCmdLineOpt_destroy(ucCmdLineOpt*);

/*
 * Summary:
 *   Releases memory used by the command option and all proceeding options
 *   in the list. All memory used by any argument options, switch options,
 *   and switch-argument options is also released.
 */
uc_EXPORTED void ucCmdLineOpt_destroy_chain(ucCmdLineOpt*);

/*
 * Summary:
 *   Invokes the function of the command option that matches the command structure.
 * Parameters:
 *   cmd: The command structure whose option is invoked.
 * Returns:
 *   The response to the command.
 */
uc_EXPORTED const char *ucCmdLineOpt_process(ucCmdLineOpt*, ucCmdLine *cmd);

/*
 * Summary:
 *   Uses the provided command structure to respond with a usage string 
 *   for this command option.
 * Parameters:
 *   cmd: The command structure used to respond with the usage string.
 */
uc_EXPORTED void ucCmdLineOpt_send_usage(ucCmdLineOpt*, ucCmdLine *cmd);

/*
 * Summary:
 *   Uses the provided command structure to respond with help information
 *   for the this command option.
 * Parameters:
 *   cmd: The command structure used to respond with the help information.
 */
uc_EXPORTED void ucCmdLineOpt_send_help(ucCmdLineOpt*, ucCmdLine *cmd);

/*
 * Summary:
 *   An application that runs with a set of command options.
 */
typedef struct ucCmdLineApp ucCmdLineApp;

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
typedef char *(ucCmdLineApp_ReceiveFunc)(char *buf, size_t buf_size, void *state);

/*
 * Summary:
 *   Sets the escape string that will cause the app to exit.
 * Parameters:
 *   value: The escape string that, when returned in a response,
 *          causes the app to exit.
 */
uc_EXPORTED void ucCmdLineApp_set_escape_response(ucCmdLineApp*, const char *value);

/*
 * Summary:
 *   Gets the escape string that causes the app to exit.
 * Returns:
 *   The escape string that, when returned in a response,
 *   causes the app to exit.
 */
uc_EXPORTED const char *ucCmdLineApp_get_escape_response(ucCmdLineApp*);

/*
 * Summary:
 *   Runs the application with the given options.
 * Parameters:
 *   cmd_opt: A pointer to the first command option for the app.
 * Returns:
 *   An error code, if one occurred.
 */ 
uc_EXPORTED ucErr ucCmdLineApp_run(ucCmdLineApp*, ucCmdLineOpt *cmd_opt);

/*
 * Summary:
 *   Gets a static, default instance of the application.
 * Returns:
 *   The static, default instance of the application.
 */
uc_EXPORTED ucCmdLineApp *ucCmdLineApp_get_instance(void);

/*
 * Summary:
 *   Sets the function that the app uses to receive data.
 * Parameters:
 *   value: A pointer to the function used to receive data.
 */
uc_EXPORTED void ucCmdLineApp_set_receive(ucCmdLineApp*, ucCmdLineApp_ReceiveFunc *value);

/*
 * Summary:
 *   Gets the function that the app uses to receive data.
 * Returns:
 *   A pointer to the function used to receive data.
 */ 
uc_EXPORTED ucCmdLineApp_ReceiveFunc *ucCmdLineApp_get_receive(ucCmdLineApp*);

/*
 * Summary:
 *   Gets the stateful object passed to the application's receive function.
 * Returns:
 *   A pointer to the stateful object passed to the application's receive function.
 */
uc_EXPORTED void *ucCmdLineApp_get_receive_state(ucCmdLineApp*);

/*
 * Summary:
 *   Sets the stateful object passed to the application's receive function.
 * Parameters:
 *   value: The stateful object passed to the application's receive function.
 */
uc_EXPORTED void ucCmdLineApp_set_receive_state(ucCmdLineApp*, void *value);

/*
 * Summary:
 *   Sets the command used to quit the application.
 * Parameters:
 *   value: The value of the command that quits the application.
 */
uc_EXPORTED void ucCmdLineApp_set_quit_command(ucCmdLineApp*, const char *value);

/*
 * Summary:
 *   Gets the value of the command that quits the application.
 * Returns:
 *   The value of the command that quits the application.
 */
uc_EXPORTED const char *ucCmdLineApp_get_quit_command(ucCmdLineApp*);

/*
 * Summary:
 *   Sets the value of the command that shows help information.
 * Parameters:
 *   value: The value of the command that shows help information.
 */
uc_EXPORTED void ucCmdLineApp_set_help_command(ucCmdLineApp*, const char *value);

/*
 * Summary:
 *   Gets the value of the command that shows help information.
 * Returns:
 *   The value of the command that shows help information.
 */
uc_EXPORTED const char *ucCmdLineApp_get_help_command(ucCmdLineApp*);

/*
 * Summary:
 *   Sets the command structure that the application uses.
 * Parameters:
 *   value: The command structure to be used by the application.
 */
uc_EXPORTED void ucCmdLineApp_set_cmd(ucCmdLineApp*, ucCmdLine *value);

/*
 * Summary:
 *   Gets the command structure used by the application.
 * Returns:
 *   A pointer to the command structure used by the application.
 */
uc_EXPORTED ucCmdLine *ucCmdLineApp_get_cmd(ucCmdLineApp*);

/*
 * Summary:
 *   Gets the command parser used by the application.
 * Returns:
 *   A pointer to the command parser used by the application.
 */
uc_EXPORTED ucCmdParser *ucCmdLineApp_get_cmd_parser(ucCmdLineApp*);

/*
 * Summary:
 *   Gets the size of the application's command-string buffer.
 * Returns:
 *   The size of the command-string buffer.
 */
uc_EXPORTED size_t ucCmdLineApp_get_cmd_str_size_max(ucCmdLineApp *p);

#endif
