#ifndef UC_ARG_OPT_OWNER_H
#define UC_ARG_OPT_OWNER_H

#include "uc_arg_opt.h"

/*
 * Summary:
 *   Base structure for options (switches and commands)
 *   that contain argument options. This type is a child
 *   of the base option type.
 */
typedef struct uc_arg_opt_owner uc_arg_opt_owner;

/*
 * Summary:
 *   Gets the first argument option of the given option owner.
 * Parameters:
 *   p: A pointer to the option owner whose argument option is to be returned.
 * Returns:
 *   A pointer to the first argument option of the given option owner.
 */
UC_EXPORTED uc_arg_opt *uc_arg_opt_owner_get_arg_opt(uc_arg_opt_owner *p);

#endif
