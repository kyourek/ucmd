#ifndef UCARGTOK_H
#define UCARGTOK_H

#include "uc_tok.h"

/*
 * Summary:
 *   An argument token. This type is a child of the
 *   base token type. All functions that take an 
 *   instance of the base type can be used with an 
 *   instance of this type.
 */
typedef const char ucArgTok;

/*
 * Summary:
 *   Gets the next argument after the given argument.
 * Parameters:
 *   p: The argument token that precedes the one that is returned.
 * Returns:
 *   The next argument in the list.
 */
UC_EXPORTED ucArgTok *ucArgTok_get_next(ucArgTok* p);

/*
 * Summary:
 *   Counts the number of arguments in the linked list.
 * Parameters:
 *   p: The first argument in the list.
 * Returns:
 *   The number of arguments in the list.
 */
UC_EXPORTED int ucArgTok_count(ucArgTok* p);

/*
 * Summary:
 *   Finds the argument in the list with the specified value.
 * Parameters:
 *   p: The first argument in the list.
 *   arg_value: The value of the argument to find.
 * Returns:
 *   The argument with the specified value, or NULL if none exists.
 */
UC_EXPORTED ucArgTok *ucArgTok_find(ucArgTok *p, const char *arg_value);

/*
 * Summary:
 *   Gets a value indicating whether or not the value exists in
 *   the argument list.
 * Parameters:
 *   p: A pointer to the argument token where the search begins.
 *   arg_value: The value of the argument to be found.
 * Returns:
 *   UC_TRUE if an argument token with the given value is found.
 *   Otherwise, UC_FALSE.
 */
UC_EXPORTED uc_bool ucArgTok_contains(ucArgTok* p, const char *arg_value);

#endif
