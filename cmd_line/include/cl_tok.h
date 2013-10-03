#ifndef CL_TOK_H
#define CL_TOK_H

#include <stdlib.h>
#include "cl_bool.h"
#include "cl_common.h"

/*
 * Summary:
 *   Base structure for tokenized values in a command.
 */
typedef const char cl_tok;

/*
 * Summary:
 *   Gets the length of the token.
 * Parameters:
 *   p: A pointer to the token whose length is returned.
 * Returns:
 *   The number of characters in the token.
 */
CL_EXPORTED size_t cl_tok_get_length(cl_tok *p);

/*
 * Summary:
 *   Determines whether or not the given token equals the value.
 * Parameters:
 *   p: A pointer to the token.
 *   value: The value against which the token is checked for equality.
 * Returns:
 *   CL_TRUE if the token value equals the given value. Otherwise, CL_FALSE.
 */
CL_EXPORTED cl_bool cl_tok_equals(cl_tok *p, const char *value);

/*
 * Summary:
 *   Determines whether or not the given token is considered numeric.
 * Parameters:
 *   p: A pointer to the token.
 * Returns:
 *   CL_TRUE if the token is considered numeric. Otherwise, CL_FALSE.
 */
CL_EXPORTED cl_bool cl_tok_is_numeric(cl_tok *p);

/*
 * Summary:
 *   Determines whether or not the given token is considered a switch.
 * Parameters:
 *   p: A pointer to the token.
 * Returns:
 *   CL_TRUE if the token is a switch. Otherwise, CL_FALSE.
 */
CL_EXPORTED cl_bool cl_tok_is_switch(cl_tok* p);

/*
 * Summary:
 *   Gets the next token in the list.
 * Parameters:
 *   p: A pointer to the token that precedes the one returned.
 * Returns:
 *   A pointer to the token that comes next in the list, or NULL
 *   if no further tokens exist.
 */
CL_EXPORTED cl_tok *cl_tok_get_next(cl_tok *p);

/*
 * Summary:
 *   Counts the number of tokens in the linked list.
 * Parameters:
 *   p: A pointer to the first token in the list.
 * Returns:
 *   The number of tokens in the list.
 */
CL_EXPORTED int cl_tok_count(cl_tok *p);

/*
 * Summary:
 *   Gets the value of the token.
 * Parameters:
 *   p: A pointer to the token whose value is returned.
 * Returns:
 *   The string value of the token.
 */
CL_EXPORTED const char *cl_tok_get_value(cl_tok* p);

#endif
