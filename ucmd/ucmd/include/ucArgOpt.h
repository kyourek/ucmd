#ifndef UCARGOPT_H
#define UCARGOPT_H

#include "ucOpt.h"

/*
 * Summary:
 *   An argument option. This type is a child
 *   of the base option type.
 */
typedef struct ucArgOpt ucArgOpt;

/*
 * Summary:
 *   Gets the minimum allowed token count of the argument option.
 * Parameters:
 *   p: A pointer to the argument option in question.
 * Returns:
 *   The minimum number of argument tokens allowed for this option.
 */
uc_EXPORTED int ucArgOpt_get_min_tok_count(ucArgOpt *p);

/*
 * Summary:
 *   Gets the maximum allowed token count of the argument option.
 * Parameters:
 *   p: A pointer to the argument option in question.
 * Returns:
 *   The maximum number of argument tokens allowed for this option.
 */
uc_EXPORTED int ucArgOpt_get_max_tok_count(ucArgOpt *p);

/*
 * Summary:
 *   Gets a flag that indicates whether or not this argument option
 *   is numeric.
 * Parameters:
 *   p: A pointer to the argument option in question.
 * Returns:
 *   ucBool_TRUE if the argument is numeric. Otherwise, ucBool_FALSE.
 */
uc_EXPORTED ucBool ucArgOpt_is_numeric(ucArgOpt *p);

/*
 * Summary:
 *   Gets the minimum value if this argument option is numeric.
 * Parameters:
 *   p: A pointer to the argument option in question.
 * Returns:
 *   The minimum numeric value of the argument.
 */
uc_EXPORTED ucArgOpt_NUMERIC_TYPE ucArgOpt_get_numeric_min(ucArgOpt *p);

/*
 * Summary:
 *   Gets the maximum value if this argument is numeric.
 * Parameters:
 *   p: A pointer to the argument option in question.
 * Returns:
 *   The maximum numeric value of the argument.
 */
uc_EXPORTED ucArgOpt_NUMERIC_TYPE ucArgOpt_get_numeric_max(ucArgOpt* p);

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
uc_EXPORTED ucArgOpt *ucArgOpt_create(const char *name, const char *desc, ucArgOpt *next);

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
uc_EXPORTED ucArgOpt *ucArgOpt_create_multiple(const char *name, const char *desc, int min_tok_count, int max_tok_count);

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
uc_EXPORTED ucArgOpt *ucArgOpt_create_required(const char *name, const char *desc, ucArgOpt *next);

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
uc_EXPORTED ucArgOpt *ucArgOpt_create_numeric(const char *desc, ucArgOpt_NUMERIC_TYPE numeric_min, ucArgOpt_NUMERIC_TYPE numeric_max, ucArgOpt *next);

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
uc_EXPORTED ucArgOpt *ucArgOpt_create_multiple_numeric(const char *desc, int min_tok_count, int max_tok_count, ucArgOpt_NUMERIC_TYPE numeric_min, ucArgOpt_NUMERIC_TYPE numeric_max);

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
uc_EXPORTED ucArgOpt *ucArgOpt_create_required_numeric(const char *desc, ucArgOpt_NUMERIC_TYPE numeric_min, ucArgOpt_NUMERIC_TYPE numeric_max, ucArgOpt *next);

/*
 * Summary:
 *   Gets the next argument option after the given option.
 * Parameters:
 *   p: The option that precedes the returned option.
 * Returns:
 *   A pointer to the option that the given option precedes,
 *   or NULL of no further options exist.
 */
uc_EXPORTED ucArgOpt *ucArgOpt_get_next(ucArgOpt* p);

/*
 * Summary:
 *   Releases memory used by the argument option.
 * Parameters:
 *   p: The argument option to be released.
 */
uc_EXPORTED void ucArgOpt_destroy(ucArgOpt *p);

/*
 * Summary:
 *   Releases memory used by the argument option
 *   and all proceeding options in the list.
 * Parameters:
 *   p: The first argument option in the list.
 */
uc_EXPORTED void ucArgOpt_destroy_chain(ucArgOpt *p);

#endif
