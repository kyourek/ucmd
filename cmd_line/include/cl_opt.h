#ifndef CL_OPT_H
#define CL_OPT_H

#include "cl_common.h"

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
CL_EXPORTED CL_BOOL cl_opt_is_required(cl_opt *p);

#endif
