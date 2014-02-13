#ifndef UC_ARG_OPT_H
#define UC_ARG_OPT_H

#include "uc_opt.h"

/*
 * Summary:
 *   An argument option. This type is a child
 *   of the base option type.
 */
typedef struct uc_arg_opt uc_arg_opt;

/*
 * Summary:
 *   Gets the minimum allowed token count of the argument option.
 * Parameters:
 *   p: A pointer to the argument option in question.
 * Returns:
 *   The minimum number of argument tokens allowed for this option.
 */
UC_EXPORTED int uc_arg_opt_get_min_tok_count(uc_arg_opt *p);

/*
 * Summary:
 *   Gets the maximum allowed token count of the argument option.
 * Parameters:
 *   p: A pointer to the argument option in question.
 * Returns:
 *   The maximum number of argument tokens allowed for this option.
 */
UC_EXPORTED int uc_arg_opt_get_max_tok_count(uc_arg_opt *p);

/*
 * Summary:
 *   Gets a flag that indicates whether or not this argument option
 *   is numeric.
 * Parameters:
 *   p: A pointer to the argument option in question.
 * Returns:
 *   UC_TRUE if the argument is numeric. Otherwise, UC_FALSE.
 */
UC_EXPORTED uc_bool uc_arg_opt_is_numeric(uc_arg_opt *p);

/*
 * Summary:
 *   Gets the minimum value if this argument option is numeric.
 * Parameters:
 *   p: A pointer to the argument option in question.
 * Returns:
 *   The minimum numeric value of the argument.
 */
UC_EXPORTED UC_ARG_OPT_NUMERIC_TYPE uc_arg_opt_get_numeric_min(uc_arg_opt *p);

/*
 * Summary:
 *   Gets the maximum value if this argument is numeric.
 * Parameters:
 *   p: A pointer to the argument option in question.
 * Returns:
 *   The maximum numeric value of the argument.
 */
UC_EXPORTED UC_ARG_OPT_NUMERIC_TYPE uc_arg_opt_get_numeric_max(uc_arg_opt* p);

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
UC_EXPORTED uc_arg_opt *uc_arg_opt_create(const char *name, const char *desc, uc_arg_opt *next);

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
UC_EXPORTED uc_arg_opt *uc_arg_opt_create_multiple(const char *name, const char *desc, int min_tok_count, int max_tok_count);

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
UC_EXPORTED uc_arg_opt *uc_arg_opt_create_required(const char *name, const char *desc, uc_arg_opt *next);

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
UC_EXPORTED uc_arg_opt *uc_arg_opt_create_numeric(const char *desc, UC_ARG_OPT_NUMERIC_TYPE numeric_min, UC_ARG_OPT_NUMERIC_TYPE numeric_max, uc_arg_opt *next);

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
UC_EXPORTED uc_arg_opt *uc_arg_opt_create_multiple_numeric(const char *desc, int min_tok_count, int max_tok_count, UC_ARG_OPT_NUMERIC_TYPE numeric_min, UC_ARG_OPT_NUMERIC_TYPE numeric_max);

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
UC_EXPORTED uc_arg_opt *uc_arg_opt_create_required_numeric(const char *desc, UC_ARG_OPT_NUMERIC_TYPE numeric_min, UC_ARG_OPT_NUMERIC_TYPE numeric_max, uc_arg_opt *next);

/*
 * Summary:
 *   Gets the next argument option after the given option.
 * Parameters:
 *   p: The option that precedes the returned option.
 * Returns:
 *   A pointer to the option that the given option precedes,
 *   or NULL of no further options exist.
 */
UC_EXPORTED uc_arg_opt *uc_arg_opt_get_next(uc_arg_opt* p);

/*
 * Summary:
 *   Releases memory used by the argument option.
 * Parameters:
 *   p: The argument option to be released.
 */
UC_EXPORTED void uc_arg_opt_destroy(uc_arg_opt *p);

/*
 * Summary:
 *   Releases memory used by the argument option
 *   and all proceeding options in the list.
 * Parameters:
 *   p: The first argument option in the list.
 */
UC_EXPORTED void uc_arg_opt_destroy_chain(uc_arg_opt *p);

#endif
