#ifndef CL_TEST_H
#define CL_TEST_H

#include "cl_bool.h"
#include "cl_test_group.h"

typedef struct cl_test cl_test;

#define CL_TEST_FAIL() do { return -1; } while (0)

#define CL_TEST_ASSERT(TRUE) do { if (!(TRUE)) return -1; } while (0)

/*
 * Summary:
 *   The type of functions used by a tests structure to print
 *   information.
 * Parameters:
 *   str: The information string to be printed.
 *   state: The tests structure's print state.
 */
typedef void (cl_test_print_func)(const char *str, void *state);

/*
 * Summary:
 *   The type of function used by a tests structure when all
 *   tests have completed and the test procedure is about to
 *   exit.
 * Parameters:
 *   state: The test structure's exit state.
 * Returns:
 *   A boolean true value if the test procedure should exit.
 *   Otherwise, a false value.
 */
typedef cl_bool (cl_test_exit_func)(void* state);

/*
 * Summary:
 *   Sets the function used by the tests structure to print information.
 * Parameters:
 *   p: A pointer to the tests structure.
 *   value: The print function used to print information.
 */
CL_EXPORTED void cl_test_set_print_func(cl_test *p, cl_test_print_func *value);

/* Summary:
 *   Sets the state object that is given as a parameter to the print function.
 * Parameters:
 *   p: A pointer to the tests structure.
 *   value: The state object that is given as a parameter to the structure's print function.
 */
CL_EXPORTED void cl_test_set_print_state(cl_test *p, void *value);

/*
 * Summary:
 *   Sets the function used by the tests structure when exiting.
 * Parameters:
 *   p: A pointer to the tests structure.
 *   value: The function that is called before the test procedure exits.
 */
CL_EXPORTED void cl_test_set_exit_func(cl_test *p, cl_test_exit_func *value);

/*
 * Summary:
 *   Sets the state object that is given as a paramter to the tests structure's exit function.
 * Parameters:
 *   p: A pointer to the tests structure.
 *   value: The state object that is given as a parameter to the exit function.
 */
CL_EXPORTED void cl_test_set_exit_state(cl_test *p, void *value);

CL_EXPORTED cl_test_group **cl_test_get_groups(cl_test *p);

CL_EXPORTED cl_test_state *cl_test_get_state(cl_test *p);

CL_EXPORTED const char *cl_test_get_label(cl_test *p);

CL_EXPORTED cl_test *cl_test_init(cl_test *p, const char *label, cl_test_group **groups);

CL_EXPORTED cl_test_err cl_test_run(cl_test *p);

struct cl_test {
    const char *label;
    cl_test_state state;

    cl_test_print_func *print_func;
    void *print_state;
    
    cl_test_exit_func *exit_func;
    void *exit_state;

    cl_test_group **groups;
};

#endif
