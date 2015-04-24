#ifndef UCSWITCHTOK_H
#define UCSWITCHTOK_H

#include "ucArgTokOwner.h"

/*
 * Summary:
 *   A switch token. This type is a child of the
 *   base token type. All functions that take an 
 *   instance of the base type can be used with an 
 *   instance of this type.
 */
typedef const char ucSwitchTok;

/*
 * Summary:
 *   Gets the next switch token after the given token.
 * Returns:
 *   The next switch token after the given token.
 */
uc_EXPORTED ucSwitchTok *ucSwitchTok_get_next(ucSwitchTok*);

/*
 * Summary:
 *   Counts the number of switches in the linked list.
 * Returns:
 *   The number of switches in the list.
 */
uc_EXPORTED int ucSwitchTok_count(ucSwitchTok*);

/*
 * Summary:
 *   Finds the switch with the specified value.
 * Parameters:
 *   switch_value: The value of the switch to be found.
 * Returns:
 *   The switch with the specified value, or NULL if none exist.
 */
uc_EXPORTED ucSwitchTok *ucSwitchTok_find(ucSwitchTok*, const char *switch_value);

/*
 * Summary:
 *   Gets a value indicating whether or not a value exists in the switch list.
 * Parameters:
 *   switch_value: The value of the switch to be found.
 * Returns:
 *   ucBool_TRUE if a switch with the given value was found in the list. Otherwise,
 *   ucBool_FALSE.
 */
uc_EXPORTED ucBool ucSwitchTok_contains(ucSwitchTok*, const char *switch_value);

/*
 * Summary:
 *   Gets the first argument token of the switch.
 * Returns:
 *   A pointer to the first argument of the switch, or NULL
 *   if no arguments exist.
 */
uc_EXPORTED ucArgTok *ucSwitchTok_get_arg(ucSwitchTok*);

#endif
