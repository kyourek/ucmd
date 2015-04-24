#ifndef UCOPT_H
#define UCOPT_H

#include "ucCmdLine.h"

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

#endif
