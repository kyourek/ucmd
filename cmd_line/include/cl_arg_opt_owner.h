#ifndef CL_ARG_OPT_OWNER_H
#define CL_ARG_OPT_OWNER_H

#include "cl_arg_opt.h"

/*
 * Summary:
 *   Base structure for options (switches and commands)
 *   that contain argument options. This type is a child
 *   of the base option type.
 */
typedef struct cl_arg_opt_owner cl_arg_opt_owner;

/*
 * Summary:
 *   Gets the first argument option of the given option owner.
 * Parameters:
 *   p: A pointer to the option owner whose argument option is to be returned.
 * Returns:
 *   A pointer to the first argument option of the given option owner.
 */
CL_EXPORTED cl_arg_opt *cl_arg_opt_owner_get_arg_opt(cl_arg_opt_owner *p);

#endif
