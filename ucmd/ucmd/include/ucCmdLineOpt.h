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

#endif
