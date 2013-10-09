#ifndef CL_TESTS_H
#define CL_TESTS_H

#include "cl_bool.h"
#include "cl_test_group.h"        

/*
 * Summary:
 *   A structure that can run and keep track of unit tests
 *   and their results.
 */
typedef struct cl_tests cl_tests;

/*
 * Summary:
 *   The type of functions used by a tests structure to print
 *   information.
 * Parameters:
 *   str: The information string to be printed.
 *   state: The tests structure's print state.
 */
typedef void (cl_tests_print_func)(const char *str, void *state);

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
typedef cl_bool (cl_tests_exit_func)(void* state);

/*
 * Summary:
 *   Sets the function used by the tests structure to print information.
 * Parameters:
 *   p: A pointer to the tests structure.
 *   value: The print function used to print information.
 */
CL_EXPORTED void cl_tests_set_print_func(cl_tests *p, cl_tests_print_func *value);

/* Summary:
 *   Sets the state object that is given as a parameter to the print function.
 * Parameters:
 *   p: A pointer to the tests structure.
 *   value: The state object that is given as a parameter to the structure's print function.
 */
CL_EXPORTED void cl_tests_set_print_state(cl_tests *p, void *value);

/*
 * Summary:
 *   Sets the function used by the tests structure when exiting.
 * Parameters:
 *   p: A pointer to the tests structure.
 *   value: The function that is called before the test procedure exits.
 */
CL_EXPORTED void cl_tests_set_exit_func(cl_tests *p, cl_tests_exit_func *value);

/*
 * Summary:
 *   Sets the state object that is given as a paramter to the tests structure's exit function.
 * Parameters:
 *   p: A pointer to the tests structure.
 *   value: The state object that is given as a paramter to the exit function.
 */
CL_EXPORTED void cl_tests_set_exit_state(cl_tests *p, void *value);

/*
 * Summary:
 *   Runs a group of unit tests.
 * Parameters:
 *   p: A pointer to the tests structure.
 *   group: The group of tests to run.
 * Returns:
 *   The result of the first unit test that failed, or no error if all tests passed.
 */
CL_EXPORTED cl_tests_err cl_tests_run_group(cl_tests *p, cl_test_group *group);

CL_EXPORTED cl_tests *cl_tests_get_instance(void);

CL_EXPORTED int cl_tests_get_run_count(cl_tests *p);

CL_EXPORTED int cl_tests_get_run_group_count(cl_tests *p);

CL_EXPORTED int cl_tests_get_assertions_made(cl_tests *p);

CL_EXPORTED int cl_tests_get_group_run_count(cl_tests *p);

CL_EXPORTED int cl_tests_get_group_assertions_made(cl_tests *p);

CL_EXPORTED cl_bool cl_tests_get_unattended(cl_tests *p);

CL_EXPORTED void cl_tests_set_unattended(cl_tests *p, cl_bool value);

CL_EXPORTED cl_tests_err cl_tests_get_err_returned(cl_tests *p);

CL_EXPORTED cl_tests_err cl_tests_run(cl_tests *p);

CL_EXPORTED cl_tests_err cl_tests_main(cl_tests *p);

#endif
