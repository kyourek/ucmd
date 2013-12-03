#ifndef CL_BOOL_H
#define CL_BOOL_H

/*
 * Summary:
 *   Boolean type definition. This definition is used
 *   to increase the readability of the source by replacing
 *   integer representations of boolean values with the
 *   more familiar "true" and "false" values.
 */
typedef enum cl_bool {
    CL_FALSE = 0,
    CL_TRUE = !CL_FALSE
} cl_bool;

#endif
