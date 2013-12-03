#ifndef CL_ARG_TOK_OWNER_H
#define CL_ARG_TOK_OWNER_H

#include "cl_arg_tok.h"

/*
 * Summary:
 *   Type definition for tokens (i.e. switch and command tokens)
 *   that contain arguments. This type is a child of the base
 *   token type. All functions that take an instance of the base
 *   type can be used with an instance of this type.
 */
typedef const char cl_arg_tok_owner;

/*
 * Summary
 *   Gets the first argument that belongs to the given owner.
 * Parameters:
 *   p: The object whose property is to be returned.
 * Returns:
 *   The first argument that belongs to the owner, or NULL if
 *   no arguments exist.
 */
CL_EXPORTED cl_arg_tok *cl_arg_tok_owner_get_arg(cl_arg_tok_owner *p);

#endif
