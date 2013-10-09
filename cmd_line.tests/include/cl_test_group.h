#ifndef CL_TEST_GROUP_H
#define CL_TEST_GROUP_H

#include "cl_test_state.h"
#include "cl_tests_err.h"

/*
 * Summary:
 *   Structure for holding information about a group
 *   of unit tests.
 */
typedef struct cl_test_group cl_test_group;

/*
 * Summary:
 *   The type of function that is used as a callback at certain times
 *   while running a group of unit tests.
 * Parameters:
 *   p: A pointer to the running test group.
 * Returns:
 *   An error number to cancel pending tests, or no error to continue.
 */
typedef cl_tests_err (cl_test_group_callback_func)(cl_test_group *p);

/*
 * Summary:
 *   The type of function that runs a unit test.
 * Parameters:
 *   p: A pointer to the running test group.
 * Returns:
 *   The error number resulting from the test, or no
 *   error if an error did not occur.
 */
typedef cl_tests_err (cl_test_group_test_func)(cl_test_group *p);

/*
 * Summary:
 *   Performs actions that should be performed before any tests are
 *   run in a group.
 * Parameters:
 *   p: A pointer to the running test group.
 * Returns:
 *   An error number to cancel pending tests, or no error to continue.
 */
CL_EXPORTED cl_tests_err cl_test_group_before_all_tests(cl_test_group *p);

/*
 * Summary:
 *   Performs actions that should be performed after all tests have
 *   been run in a group.
 * Parameters:
 *   p: A pointer to the running test group.
 * Returns:
 *   An error number to cancel pending tests, or no error to continue.
 */
CL_EXPORTED cl_tests_err cl_test_group_after_all_tests(cl_test_group *p);

/*
 * Summary:
 *   Performs actions that should be performed before each test
 *   is run in a group.
 * Parameters:
 *   p: A pointer to the running test group.
 * Returns:
 *   An error number to cancel pending tests, or no error to continue.
 */
CL_EXPORTED cl_tests_err cl_test_group_before_each_test(cl_test_group *p);

/*
 * Summary:
 *   Performs actions that should be performed after each test
 *   is run in a group.
 * Parameters:
 *   p: A pointer to the running test group.
 * Returns:
 *   An error number to cancel pending tests, or no error to continue.
 */
CL_EXPORTED cl_tests_err cl_test_group_after_each_test(cl_test_group *p);

/*
 * Summary:
 *   Gets a NULL-terminated list of tests in the group.
 * Parameters:
 *   p: A pointer to the running test group.
 * Returns:
 *   The lists of tests to run for the group.
 */
CL_EXPORTED cl_test_group_test_func **cl_test_group_get_tests(cl_test_group *p);

/*
 * Summary:
 *   Initializes a test group structure.
 * Parameters:
 *   p: A pointer to the test group to be initialized.
 *   before_all_tests: A function that is run before any tests in the group.
 *   after_all_tests: A function that is run after all tests in the group.
 *   before_each_test: A function that is run before each test in the group.
 *   after_each_test: A function that is run after each test in the group.
 *   tests: A NULL-terminated list of tests in the group.
 * Returns:
 *   A pointer to the initialized test group.
 */
CL_EXPORTED cl_test_group *cl_test_group_init(
    cl_test_group *p, 
    cl_test_group_callback_func *before_all_tests, 
    cl_test_group_callback_func *after_all_tests, 
    cl_test_group_callback_func *before_each_test, 
    cl_test_group_callback_func *after_each_test,
    cl_test_group_test_func **tests
);

/*
 * Summary:
 *   Runs the tests in the group.
 * Parameters:
 *   p: A pointer to the test group.
 *   state: The object that maintains test state.
 * Returns:
 *   The error number of the failed test, or no error if all
 *   tests passed.
 */
CL_EXPORTED cl_tests_err cl_test_group_run(cl_test_group *p, cl_test_state *state);

/*
 * Summary:
 *   Structure for holding information about a group
 *   of unit tests.
 */
struct cl_test_group {
    cl_test_group_callback_func *before_all_tests;
    cl_test_group_callback_func *after_all_tests;
    cl_test_group_callback_func *before_each_test;
    cl_test_group_callback_func *after_each_test;
    cl_test_group_test_func **tests;
};

#endif
