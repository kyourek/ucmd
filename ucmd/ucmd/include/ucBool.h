#ifndef UCBOOL_H
#define UCBOOL_H

/*
 * Summary:
 *   Boolean type definition. This definition is used
 *   to increase the readability of the source by replacing
 *   integer representations of boolean values with the
 *   more familiar "true" and "false" values.
 */
typedef enum ucBool {
    ucBool_FALSE = 0,
    ucBool_TRUE = !ucBool_FALSE
} ucBool;

#endif
