#ifndef UC_OPT_H
#define UC_OPT_H

#include "uc_cmd_line.h"

/*
 * Summary:
 *   Base type for command, argument, and switch options.
 */
typedef struct uc_opt uc_opt;

/*
 * Summary:
 *   Gets the name of the option.
 * Parameters:
 *   p: A pointer to the option.
 * Returns:
 *   The name of the option.
 */
UC_EXPORTED const char* uc_opt_get_name(uc_opt *p);

/*
 * Summary:
 *   Gets the description of the option.
 * Parameters:
 *   p: A pointer to the option.
 * Returns:
 *   The description of the option.
 */
UC_EXPORTED const char *uc_opt_get_desc(uc_opt *p);

/*
 * Summary:
 *   Gets a flag that indicates whether or not the option
 *   is required.
 * Parameters:
 *   p: A pointer to the option in question.
 * Returns:
 *   UC_TRUE if the option is required. Otherwise, UC_FALSE.
 */
UC_EXPORTED uc_bool uc_opt_is_required(uc_opt *p);

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
UC_EXPORTED void uc_opt_send_help(uc_opt *p, uc_cmd_line *cmd, const char *prefix);

#endif
