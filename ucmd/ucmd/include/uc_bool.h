#ifndef UC_BOOL_H
#define UC_BOOL_H

/*
 * Summary:
 *   Boolean type definition. This definition is used
 *   to increase the readability of the source by replacing
 *   integer representations of boolean values with the
 *   more familiar "true" and "false" values.
 */
typedef enum uc_bool {
    UC_FALSE = 0,
    UC_TRUE = !UC_FALSE
} uc_bool;

#endif
