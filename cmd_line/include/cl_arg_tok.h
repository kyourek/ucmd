#ifndef CL_ARG_TOK_H
#define CL_ARG_TOK_H

#include "cl_bool.h"
#include "cl_common.h"

/*
 * Summary:
 *   An argument token.
 */
typedef const char cl_arg_tok;

/*
 * Summary:
 *   Gets the next argument after the given argument.
 * Parameters:
 *   p: The argument token that precedes the one that is returned.
 * Returns:
 *   The next argument in the list.
 */
CL_EXPORTED cl_arg_tok *cl_arg_tok_get_next(cl_arg_tok* p);

/*
 * Summary:
 *   Counts the number of arguments in the linked list.
 * Parameters:
 *   p: The first argument in the list.
 * Returns:
 *   The number of arguments in the list.
 */
CL_EXPORTED int cl_arg_tok_count(cl_arg_tok* p);

/*
 * Summary:
 *   Finds the argument in the list with the specified value.
 * Parameters:
 *   p: The first argument in the list.
 *   arg_value: The value of the argument to find.
 * Returns:
 *   The argument with the specified value, or NULL if none exists.
 */
CL_EXPORTED cl_arg_tok *cl_arg_tok_find(cl_arg_tok *p, const char *arg_value);

/*
 * Summary:
 *   Gets a value indicating whether or not the value exists in
 *   the argument list.
 * Parameters:
 *   p: A pointer to the argument token where the search begins.
 *   arg_value: The value of the argument to be found.
 * Returns:
 *   CL_TRUE if an argument token with the given value is found.
 *   Otherwise, CL_FALSE.
 */
CL_EXPORTED CL_BOOL cl_arg_tok_contains(cl_arg_tok* p, const char *arg_value);

#endif
