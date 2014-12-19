#ifndef UCARGTOKOWNER_H
#define UCARGTOKOWNER_H

#include "ucArgTok.h"

/*
 * Summary:
 *   Type definition for tokens (i.e. switch and command tokens)
 *   that contain arguments. This type is a child of the base
 *   token type. All functions that take an instance of the base
 *   type can be used with an instance of this type.
 */
typedef const char ucArgTokOwner;

/*
 * Summary
 *   Gets the first argument that belongs to the given owner.
 * Parameters:
 *   p: The object whose property is to be returned.
 * Returns:
 *   The first argument that belongs to the owner, or NULL if
 *   no arguments exist.
 */
uc_EXPORTED ucArgTok *ucArgTokOwner_get_arg(ucArgTokOwner *p);

#endif
