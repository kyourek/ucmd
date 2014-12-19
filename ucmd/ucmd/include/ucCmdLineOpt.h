#ifndef UCCMDLINEOPT_H
#define UCCMDLINEOPT_H

#include "ucArgOptOwner.h"
#include "ucCmdLine.h"
#include "ucCmdTok.h"
#include "ucSwitchOpt.h"

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
typedef const char *(ucCmdLineOpt_func)(ucCmdLine *cmd, void *state);

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
UC_EXPORTED ucCmdLineOpt *ucCmdLineOpt_create(ucCmdLineOpt_func *func, void *state, const char *name, const char *desc, ucArgOpt *arg_opt, ucSwitchOpt *switch_opt, ucCmdLineOpt* next);

/*
 * Summary:
 *   Gets the next command option after the given option.
 * Parameters:
 *   p: The command option that precedes the option to be returned.
 * Returns:
 *   A pointer to the next command option.
 */
UC_EXPORTED ucCmdLineOpt *ucCmdLineOpt_get_next(ucCmdLineOpt *p);

/*
 * Summary:
 *   Gets the first argument option of the command.
 * Parameters:
 *   p: A pointer to the command option.
 * Returns:
 *   A pointer to the first argument option of the command,
 *   or NULL if no argument options exist.
 */
UC_EXPORTED ucArgOpt *ucCmdLineOpt_get_arg_opt(ucCmdLineOpt *p);

/*
 * Summary:
 *   Gets the first switch option of the command option.
 * Parameters:
 *   p: A pointer to the command option whose switch argument is to be returned.
 * Returns:
 *   A pointer to the first switch option of the command option,
 *   or NULL if no switch options exist.
 */
UC_EXPORTED ucSwitchOpt *ucCmdLineOpt_get_switch_opt(ucCmdLineOpt *p);

/*
 * Summary:
 *   Finds the command option that matches the given name.
 * Parameters:
 *   p: A pointer to the first command option in the list of options.
 *   name: The name of the command whose option is to be found.
 * Returns:
 *   A pointer to the command option that matches the given name, or NULL
 *   if no option is found.
 */
UC_EXPORTED ucCmdLineOpt *ucCmdLineOpt_find_by_name(ucCmdLineOpt *p, const char *name);

/*
 * Summary:
 *   Gets the pointer to the function invoked when the command option
 *   is processed.
 * Parameters:
 *   p: A pointer to the command option whose function is to be returned.
 * Returns:
 *   A pointer to the function invoked when the command option is processed.
 */
UC_EXPORTED ucCmdLineOpt_func *ucCmdLineOpt_get_func(ucCmdLineOpt *p);

/*
 * Summary:
 *   Gets the state pointer that is passed to the command option's function
 *   when it is invoked.
 * Parameters:
 *   p: A pointer to the command option.
 * Returns:
 *   A pointer to the command option's state.
 */
UC_EXPORTED void *ucCmdLineOpt_get_state(ucCmdLineOpt *p);

/*
 * Summary:
 *   Releases memory used by the command option.
 * Parameters:
 *   p: A pointer to the command option whose memory is released.
 */
UC_EXPORTED void ucCmdLineOpt_destroy(ucCmdLineOpt *p);

/*
 * Summary:
 *   Releases memory used by the command option and all proceeding options
 *   in the list. All memory used by any argument options, switch options,
 *   and switch-argument options is also released.
 * Parameters:
 *   p: A pointer to the first command option in the list to be released.
 */
UC_EXPORTED void ucCmdLineOpt_destroy_chain(ucCmdLineOpt *p);

/*
 * Summary:
 *   Invokes the function of the command option that matches the command structure.
 * Parameters:
 *   p: A pointer to the first command option in a list. The list will
 *      be traversed until the appropriate option is found for the given
 *      command.
 *   cmd: The command structure whose option is invoked.
 * Returns:
 *   The response to the command.
 */
UC_EXPORTED const char *ucCmdLineOpt_process(ucCmdLineOpt *p, ucCmdLine *cmd);

/*
 * Summary:
 *   Uses the provided command structure to respond with a usage string 
 *   for this command option.
 * Parameters:
 *   p: A pointer to the option whose usage string will be sent.
 *   cmd: The command structure used to respond with the usage string.
 */
UC_EXPORTED void ucCmdLineOpt_send_usage(ucCmdLineOpt *p, ucCmdLine *cmd);

/*
 * Summary:
 *   Uses the provided command structure to respond with help information
 *   for the this command option.
 * Parameters:
 *   p: A pointer to the command option for which help is sent.
 *   cmd: The command structure used to respond with the help information.
 */
UC_EXPORTED void ucCmdLineOpt_send_help(ucCmdLineOpt *p, ucCmdLine *cmd);

#endif
