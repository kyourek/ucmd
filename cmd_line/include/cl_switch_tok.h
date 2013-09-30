#ifndef CL_SWITCH_TOK_H
#define CL_SWITCH_TOK_H

#include "cl_arg_tok_owner.h"

/*
 * Summary:
 *   A switch token.
 */
typedef const char cl_switch_tok;

/*
 * Summary:
 *   Gets the next switch token after the given token.
 * Parameters:
 *   p: The object whose property is to be returned.
 * Returns:
 *   The next switch token after the given token.
 */
CL_EXPORTED cl_switch_tok *cl_switch_tok_get_next(cl_switch_tok *p);

/*
 * Summary:
 *   Counts the number of switches in the linked list.
 * Parameters:
 *   p: The first switch in the list.
 * Returns:
 *   The number of switches in the list.
 */
CL_EXPORTED int cl_switch_tok_count(cl_switch_tok* p);

/*
 * Summary:
 *   Finds the switch with the specified value.
 * Parameters:
 *   p: The first switch in the list.
 *   switch_value: The value of the switch to be found.
 * Returns:
 *   The switch with the specified value, or NULL if none exist.
 */
CL_EXPORTED cl_switch_tok *cl_switch_tok_find(cl_switch_tok *p, const char *switch_value);

/*
 * Summary:
 *   Gets a value indicating whether or not a value exists in the switch list.
 * Parameters:
 *   p: A pointer to the first switch token in the list.
 *   switch_value: The value of the switch to be found.
 * Returns:
 *   CL_TRUE if a switch with the given value was found in the list. Otherwise,
 *   CL_FALSE.
 */
CL_EXPORTED CL_BOOL cl_switch_tok_contains(cl_switch_tok* p, const char *switch_value);

/*
 * Summary:
 *   Gets the first argument token of the switch.
 * Parameters:
 *   p: The switch token whose argument is returned.
 * Returns:
 *   A pointer to the first argument of the switch, or NULL
 *   if no arguments exist.
 */
CL_EXPORTED cl_arg_tok *cl_switch_tok_get_arg(cl_switch_tok *p);

#endif
