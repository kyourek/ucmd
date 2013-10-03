#ifndef CL_BOOL_H
#define CL_BOOL_H

/*
 * Summary:
 *   Boolean type definition.
 */
typedef enum cl_bool {
    CL_FALSE = 0,
    CL_TRUE = !CL_FALSE
} cl_bool;

#endif
