#ifndef CL_ARG_OPT_H
#define CL_ARG_OPT_H

#include "cl_bool.h"
#include "cl_common.h"

/*
 * Summary:
 *   An argument option.
 */
typedef struct cl_arg_opt cl_arg_opt;

/*
 * Summary:
 *   Gets a flag that indicates whether or not this argument option
 *   is numeric.
 * Parameters:
 *   p: A pointer to the argument option in question.
 * Returns:
 *   CL_TRUE if the argument is numeric. Otherwise, CL_FALSE.
 */
CL_EXPORTED cl_bool cl_arg_opt_is_numeric(cl_arg_opt *p);

/*
 * Summary:
 *   Gets the minimum value if this argument option is numeric.
 * Parameters:
 *   p: A pointer to the argument option in question.
 * Returns:
 *   The minimum numeric value of the argument.
 */
CL_EXPORTED double cl_arg_opt_get_numeric_min(cl_arg_opt *p);

/*
 * Summary:
 *   Gets the maximum value if this argument is numeric.
 * Parameters:
 *   p: A pointer to the argument option in question.
 * Returns:
 *   The maximum numeric value of the argument.
 */
CL_EXPORTED double cl_arg_opt_get_numeric_max(cl_arg_opt* p);

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
CL_EXPORTED cl_arg_opt *cl_arg_opt_create(const char *name, const char *desc, cl_arg_opt *next);

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
CL_EXPORTED cl_arg_opt *cl_arg_opt_create_required(const char *name, const char *desc, cl_arg_opt *next);

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
CL_EXPORTED cl_arg_opt *cl_arg_opt_create_numeric(const char *desc, double numeric_min, double numeric_max, cl_arg_opt *next);

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
CL_EXPORTED cl_arg_opt *cl_arg_opt_create_required_numeric(const char *desc, double numeric_min, double numeric_max, cl_arg_opt *next);

/*
 * Summary:
 *   Gets the next argument option after the given option.
 * Parameters:
 *   p: The option that precedes the returned option.
 * Returns:
 *   A pointer to the option that the given option precedes,
 *   or NULL of no further options exist.
 */
CL_EXPORTED cl_arg_opt *cl_arg_opt_get_next(cl_arg_opt* p);

/*
 * Summary:
 *   Releases memory used by the argument option.
 * Parameters:
 *   p: The argument option to be released.
 */
CL_EXPORTED void cl_arg_opt_destroy(cl_arg_opt *p);

/*
 * Summary:
 *   Releases memory used by the argument option
 *   and all proceeding options in the list.
 * Parameters:
 *   p: The first argument option in the list.
 */
CL_EXPORTED void cl_arg_opt_destroy_chain(cl_arg_opt *p);

#endif
