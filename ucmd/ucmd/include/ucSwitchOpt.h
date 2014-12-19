#ifndef UCSWITCHOPT_H
#define UCSWITCHOPT_H

#include "ucArgOptOwner.h"

/*
 * Summary:
 *   A command switch option. This type is a child
 *   of the base option type.
 */
typedef struct ucSwitchOpt ucSwitchOpt;

/*
 * Summary:
 *   Creates a new switch option.
 * Parameters:
 *   name: The name of the switch.
 *   desc: A description of the switch.
 *   arg_opt: The first argument option of the switch.
 *   next: The switch option that the created option precedes,
 *         or NULL if no further switch options exist.
 * Returns:
 *   A pointer to the newly created switch option.
 */
uc_EXPORTED ucSwitchOpt *ucSwitchOpt_create(const char *name, const char *desc, ucArgOpt *arg_opt, ucSwitchOpt *next);

/*
 * Summary:
 *   Creates a new, required switch option.
 * Parameters:
 *   name: The name of the switch.
 *   desc: A description of the switch.
 *   arg_opt: The first argument option of the switch.
 *   next: The switch option that the created option precedes,
 *         or NULL if no further switch options exist.
 * Returns:
 *   A pointer to the newly created switch option. The option's
 *   'required' property will be set to true.
 */
uc_EXPORTED ucSwitchOpt *ucSwitchOpt_create_required(const char *name, const char *desc, ucArgOpt *arg_opt, ucSwitchOpt *next);

/*
 * Summary:
 *   Gets the first argument option of the given switch.
 * Parameters:
 *   p: A pointer to the switch option whose argument is returned.
 * Returns:
 *   A pointer to the first argument option of the switch, or NULL
 *   if no argument options exist.
 */
uc_EXPORTED ucArgOpt *ucSwitchOpt_get_arg_opt(ucSwitchOpt *p);

/*
 * Summary:
 *   Finds the switch option in the linked list with the given name.
 * Parameters:
 *   p: A pointer to the first switch option in the list.
 *   name: The name of the switch option to be found.
 * Returns:
 *   The switch option with the given name, or NULL if
 *   no switch option is found.
 */
uc_EXPORTED ucSwitchOpt *ucSwitchOpt_find(ucSwitchOpt *p, const char *name);

/*
 * Summary:
 *   Gets the next switch option.
 * Parameters:
 *   p: The switch option that precedes the returned option.
 * Returns:
 *   A pointer to the next switch option in the list, or NULL
 *   if no further options exist.
 */
uc_EXPORTED ucSwitchOpt *ucSwitchOpt_get_next(ucSwitchOpt *p);

/*
 * Summary:
 *   Releases memory used by the switch option.
 * Parameters:
 *   p: A pointer to the switch option whose memory is released.
 */
uc_EXPORTED void ucSwitchOpt_destroy(ucSwitchOpt *p);

/*
 * Summary:
 *   Releases memory used by the switch option and all
 *   proceeding options in the list. All memory used by
 *   any argument options belonging to the switches in
 *   the list is also released.
 * Parameters:
 *   p: A pointer to the first switch option in the list.
 */
uc_EXPORTED void ucSwitchOpt_destroy_chain(ucSwitchOpt *p);

#endif
