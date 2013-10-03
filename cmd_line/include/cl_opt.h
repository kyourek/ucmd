#ifndef CL_OPT_H
#define CL_OPT_H

#include "cl_cmd_line.h"

/*
 * Summary:
 *   Base type for command, argument, and switch options.
 */
typedef struct cl_opt cl_opt;

/*
 * Summary:
 *   Gets the name of the option.
 * Parameters:
 *   p: A pointer to the option.
 * Returns:
 *   The name of the option.
 */
CL_EXPORTED const char* cl_opt_get_name(cl_opt *p);

/*
 * Summary:
 *   Gets the description of the option.
 * Parameters:
 *   p: A pointer to the option.
 * Returns:
 *   The description of the option.
 */
CL_EXPORTED const char *cl_opt_get_desc(cl_opt *p);

/*
 * Summary:
 *   Gets a flag that indicates whether or not the option
 *   is required.
 * Parameters:
 *   p: A pointer to the option in question.
 * Returns:
 *   CL_TRUE if the option is required. Otherwise, CL_FALSE.
 */
CL_EXPORTED cl_bool cl_opt_is_required(cl_opt *p);

/*
 * Summary:
 *   Uses the provided command structure to send help information
 *   for this option.
 * Parameters:
 *   p: A pointer to the option for which help is sent.
 *   cmd: A pointer to the command structure used to respond
 *        with the help information.
 *   prefix: A string used to prefix the help information.
 */
CL_EXPORTED void cl_opt_send_help(cl_opt *p, cl_cmd_line *cmd, const char *prefix);

#endif
