#ifndef UC_SWITCH_TOK_H
#define UC_SWITCH_TOK_H

#include "ucArgTokOwner.h"

/*
 * Summary:
 *   A switch token. This type is a child of the
 *   base token type. All functions that take an 
 *   instance of the base type can be used with an 
 *   instance of this type.
 */
typedef const char uc_switch_tok;

/*
 * Summary:
 *   Gets the next switch token after the given token.
 * Parameters:
 *   p: The object whose property is to be returned.
 * Returns:
 *   The next switch token after the given token.
 */
UC_EXPORTED uc_switch_tok *uc_switch_tok_get_next(uc_switch_tok *p);

/*
 * Summary:
 *   Counts the number of switches in the linked list.
 * Parameters:
 *   p: The first switch in the list.
 * Returns:
 *   The number of switches in the list.
 */
UC_EXPORTED int uc_switch_tok_count(uc_switch_tok* p);

/*
 * Summary:
 *   Finds the switch with the specified value.
 * Parameters:
 *   p: The first switch in the list.
 *   switch_value: The value of the switch to be found.
 * Returns:
 *   The switch with the specified value, or NULL if none exist.
 */
UC_EXPORTED uc_switch_tok *uc_switch_tok_find(uc_switch_tok *p, const char *switch_value);

/*
 * Summary:
 *   Gets a value indicating whether or not a value exists in the switch list.
 * Parameters:
 *   p: A pointer to the first switch token in the list.
 *   switch_value: The value of the switch to be found.
 * Returns:
 *   UC_TRUE if a switch with the given value was found in the list. Otherwise,
 *   UC_FALSE.
 */
UC_EXPORTED uc_bool uc_switch_tok_contains(uc_switch_tok* p, const char *switch_value);

/*
 * Summary:
 *   Gets the first argument token of the switch.
 * Parameters:
 *   p: The switch token whose argument is returned.
 * Returns:
 *   A pointer to the first argument of the switch, or NULL
 *   if no arguments exist.
 */
UC_EXPORTED ucArgTok *uc_switch_tok_get_arg(uc_switch_tok *p);

#endif
