#ifndef UCTOK_H
#define UCTOK_H

#include "uc.h"
#include "ucBool.h"

/*
 * Summary:
 *   Base structure for tokenized values in a command.
 */
typedef const char ucTok;

/*
 * Summary:
 *   Gets the length of the token.
 * Parameters:
 *   p: A pointer to the token whose length is returned.
 * Returns:
 *   The number of characters in the token.
 */
uc_EXPORTED int ucTok_get_length(ucTok *p);

/*
 * Summary:
 *   Determines whether or not the given token equals the value.
 * Parameters:
 *   p: A pointer to the token.
 *   value: The value against which the token is checked for equality.
 * Returns:
 *   ucBool_TRUE if the token value equals the given value. Otherwise, ucBool_FALSE.
 */
uc_EXPORTED ucBool ucTok_equals(ucTok *p, const char *value);

/*
 * Summary:
 *   Determines whether or not the given token is considered numeric.
 * Parameters:
 *   p: A pointer to the token.
 * Returns:
 *   ucBool_TRUE if the token is considered numeric. Otherwise, ucBool_FALSE.
 */
uc_EXPORTED ucBool ucTok_is_numeric(ucTok *p);

/*
 * Summary:
 *   Determines whether or not the given token is considered a switch.
 * Parameters:
 *   p: A pointer to the token.
 * Returns:
 *   ucBool_TRUE if the token is a switch. Otherwise, ucBool_FALSE.
 */
uc_EXPORTED ucBool ucTok_is_switch(ucTok* p);

/*
 * Summary:
 *   Gets the next token in the list.
 * Parameters:
 *   p: A pointer to the token that precedes the one returned.
 * Returns:
 *   A pointer to the token that comes next in the list, or NULL
 *   if no further tokens exist.
 */
uc_EXPORTED ucTok *ucTok_get_next(ucTok *p);

/*
 * Summary:
 *   Counts the number of tokens in the linked list.
 * Parameters:
 *   p: A pointer to the first token in the list.
 * Returns:
 *   The number of tokens in the list.
 */
uc_EXPORTED int ucTok_count(ucTok *p);

/*
 * Summary:
 *   Gets the value of the token.
 * Parameters:
 *   p: A pointer to the token whose value is returned.
 * Returns:
 *   The string value of the token.
 */
uc_EXPORTED const char *ucTok_get_value(ucTok* p);

#endif