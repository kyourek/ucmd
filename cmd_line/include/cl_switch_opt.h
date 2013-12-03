#ifndef CL_SWITCH_OPT_H
#define CL_SWITCH_OPT_H

#include "cl_arg_opt_owner.h"

/*
 * Summary:
 *   A command switch option. This type is a child
 *   of the base option type.
 */
typedef struct cl_switch_opt cl_switch_opt;

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
CL_EXPORTED cl_switch_opt *cl_switch_opt_create(const char *name, const char *desc, cl_arg_opt *arg_opt, cl_switch_opt *next);

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
CL_EXPORTED cl_switch_opt *cl_switch_opt_create_required(const char *name, const char *desc, cl_arg_opt *arg_opt, cl_switch_opt *next);

/*
 * Summary:
 *   Gets the first argument option of the given switch.
 * Parameters:
 *   p: A pointer to the switch option whose argument is returned.
 * Returns:
 *   A pointer to the first argument option of the switch, or NULL
 *   if no argument options exist.
 */
CL_EXPORTED cl_arg_opt *cl_switch_opt_get_arg_opt(cl_switch_opt *p);

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
CL_EXPORTED cl_switch_opt *cl_switch_opt_find(cl_switch_opt *p, const char *name);

/*
 * Summary:
 *   Gets the next switch option.
 * Parameters:
 *   p: The switch option that precedes the returned option.
 * Returns:
 *   A pointer to the next switch option in the list, or NULL
 *   if no further options exist.
 */
CL_EXPORTED cl_switch_opt *cl_switch_opt_get_next(cl_switch_opt *p);

/*
 * Summary:
 *   Releases memory used by the switch option.
 * Parameters:
 *   p: A pointer to the switch option whose memory is released.
 */
CL_EXPORTED void cl_switch_opt_destroy(cl_switch_opt *p);

/*
 * Summary:
 *   Releases memory used by the switch option and all
 *   proceeding options in the list. All memory used by
 *   any argument options belonging to the switches in
 *   the list is also released.
 * Parameters:
 *   p: A pointer to the first switch option in the list.
 */
CL_EXPORTED void cl_switch_opt_destroy_chain(cl_switch_opt *p);

#endif
