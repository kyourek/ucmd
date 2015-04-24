#ifndef UCARGOPTOWNER_H
#define UCARGOPTOWNER_H

#include "ucArgOpt.h"

/*
 * Summary:
 *   Base structure for options (switches and commands)
 *   that contain argument options. This type is a child
 *   of the base option type.
 */
typedef struct ucArgOptOwner ucArgOptOwner;

/*
 * Summary:
 *   Gets the first argument option of the given option owner.
 * Returns:
 *   A pointer to the first argument option of the given option owner.
 */
uc_EXPORTED ucArgOpt *ucArgOptOwner_get_arg_opt(ucArgOptOwner*);

#endif
