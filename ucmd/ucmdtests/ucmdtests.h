#ifndef UCMDTESTS_H
#define UCMDTESTS_H

#include "ucmd.h"
#include "ucmd_internal.h"

/*
* Summary:
*   Defines the type returned by test functions.
*/
typedef int ucTestErr;

/*
* Summary:
*   Defines the value that indicates no test error.
*/
#define ucTestErr_NONE   0

typedef struct ucTest ucTest;
typedef struct ucTestState ucTestState;

/*
* Summary:
*   Structure for holding information about a group
*   of unit tests.
*/
typedef struct ucTestGroup ucTestGroup;

uc_EXPORTED int ucTestState_get_run_test_count(ucTestState *p);

uc_EXPORTED int ucTestState_get_run_group_count(ucTestState *p);

uc_EXPORTED int ucTestState_get_run_group_test_count(ucTestState *p);

uc_EXPORTED void ucTestState_set_run_test_count(ucTestState *p, int value);

uc_EXPORTED void ucTestState_set_run_group_count(ucTestState *p, int value);

uc_EXPORTED void ucTestState_set_run_group_test_count(ucTestState *p, int value);

uc_EXPORTED void ucTestState_reset(ucTestState *p);

uc_EXPORTED ucTestState *ucTestState_init(ucTestState *p);

struct ucTestState {
    int run_test_count;
    int run_group_count;
    int run_group_test_count;
};

uc_EXPORTED ucTestGroup *ucArgOpt_tests_get_group(void);
uc_EXPORTED ucTestGroup *ucArgTokOwner_tests_get_group(void);
uc_EXPORTED ucTestGroup *ucArgTok_tests_get_group(void);
uc_EXPORTED ucTestGroup *ucBool_tests_get_group(void);
uc_EXPORTED ucTestGroup *ucCmdLineApp_tests_get_group(void);
uc_EXPORTED ucTestGroup *ucCmdLineOpt_tests_get_group(void);
uc_EXPORTED ucTestGroup *ucCmdLine_tests_get_group(void);
uc_EXPORTED ucTestGroup *ucCmdLineToks_tests_get_group(void);
uc_EXPORTED ucTestGroup *ucCmdParser_tests_get_group(void);
uc_EXPORTED ucTestGroup *ucCmdTok_tests_get_group(void);
uc_EXPORTED ucTestGroup *ucOpt_tests_get_group(void);
uc_EXPORTED ucTestGroup *ucSwitchOpt_tests_get_group(void);
uc_EXPORTED ucTestGroup *ucSwitchTok_tests_get_group(void);
uc_EXPORTED ucTestGroup *ucTok_tests_get_group(void);

uc_EXPORTED ucTest *uc_tests_get_test(void);

/*
* Summary:
*   The type of function that is used as a callback at certain times
*   while running a group of unit tests.
* Parameters:
*   p: A pointer to the running test group.
* Returns:
*   An error number to cancel pending tests, or no error to continue.
*/
typedef ucTestErr(ucTestGroup_CallbackFunc)(ucTestGroup *p);

/*
* Summary:
*   The type of function that runs a unit test.
* Parameters:
*   p: A pointer to the running test group.
* Returns:
*   The error number resulting from the test, or no
*   error if an error did not occur.
*/
typedef ucTestErr(ucTestGroup_TestFunc)(ucTestGroup *p);

/*
* Summary:
*   Performs actions that should be performed before any tests are
*   run in a group.
* Parameters:
*   p: A pointer to the running test group.
* Returns:
*   An error number to cancel pending tests, or no error to continue.
*/
uc_EXPORTED ucTestErr ucTestGroup_before_all_tests(ucTestGroup *p);

/*
* Summary:
*   Performs actions that should be performed after all tests have
*   been run in a group.
* Parameters:
*   p: A pointer to the running test group.
* Returns:
*   An error number to cancel pending tests, or no error to continue.
*/
uc_EXPORTED ucTestErr ucTestGroup_after_all_tests(ucTestGroup *p);

/*
* Summary:
*   Performs actions that should be performed before each test
*   is run in a group.
* Parameters:
*   p: A pointer to the running test group.
* Returns:
*   An error number to cancel pending tests, or no error to continue.
*/
uc_EXPORTED ucTestErr ucTestGroup_before_each_test(ucTestGroup *p);

/*
* Summary:
*   Performs actions that should be performed after each test
*   is run in a group.
* Parameters:
*   p: A pointer to the running test group.
* Returns:
*   An error number to cancel pending tests, or no error to continue.
*/
uc_EXPORTED ucTestErr ucTestGroup_after_each_test(ucTestGroup *p);

/*
* Summary:
*   Gets a NULL-terminated list of tests in the group.
* Parameters:
*   p: A pointer to the running test group.
* Returns:
*   The lists of tests to run for the group.
*/
uc_EXPORTED ucTestGroup_TestFunc **ucTestGroup_get_tests(ucTestGroup *p);

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
uc_EXPORTED ucTestGroup *ucTestGroup_init(
    ucTestGroup *p,
    ucTestGroup_CallbackFunc *before_all_tests,
    ucTestGroup_CallbackFunc *after_all_tests,
    ucTestGroup_CallbackFunc *before_each_test,
    ucTestGroup_CallbackFunc *after_each_test,
    ucTestGroup_TestFunc **tests
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
uc_EXPORTED ucTestErr ucTestGroup_run(ucTestGroup *p, ucTestState *state);

/*
* Summary:
*   Structure for holding information about a group
*   of unit tests.
*/
struct ucTestGroup {
    ucTestGroup_CallbackFunc *before_all_tests;
    ucTestGroup_CallbackFunc *after_all_tests;
    ucTestGroup_CallbackFunc *before_each_test;
    ucTestGroup_CallbackFunc *after_each_test;
    ucTestGroup_TestFunc **tests;
};

#define ucTest_FAIL() do { return -1; } while (0)

/* #include <stdio.h> */
#define ucTest_ASSERT(TRUE) do { if (!(TRUE)) { /* printf("%s\n", #TRUE); */ return -1; } } while (0)

/*
* Summary:
*   The type of functions used by a tests structure to print
*   information.
* Parameters:
*   str: The information string to be printed.
*   state: The tests structure's print state.
*/
typedef void (ucTest_PrintFunc)(const char *str, void *state);

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
typedef ucBool(ucTest_ExitFunc)(void* state);

/*
* Summary:
*   Sets the function used by the tests structure to print information.
* Parameters:
*   p: A pointer to the tests structure.
*   value: The print function used to print information.
*/
uc_EXPORTED void ucTest_set_print_func(ucTest *p, ucTest_PrintFunc *value);

/* Summary:
*   Sets the state object that is given as a parameter to the print function.
* Parameters:
*   p: A pointer to the tests structure.
*   value: The state object that is given as a parameter to the structure's print function.
*/
uc_EXPORTED void ucTest_set_print_state(ucTest *p, void *value);

/*
* Summary:
*   Sets the function used by the tests structure when exiting.
* Parameters:
*   p: A pointer to the tests structure.
*   value: The function that is called before the test procedure exits.
*/
uc_EXPORTED void ucTest_set_exit_func(ucTest *p, ucTest_ExitFunc *value);

/*
* Summary:
*   Sets the state object that is given as a paramter to the tests structure's exit function.
* Parameters:
*   p: A pointer to the tests structure.
*   value: The state object that is given as a parameter to the exit function.
*/
uc_EXPORTED void ucTest_set_exit_state(ucTest *p, void *value);

uc_EXPORTED ucTestGroup **ucTest_get_groups(ucTest *p);

uc_EXPORTED ucTestState *ucTest_get_state(ucTest *p);

uc_EXPORTED const char *ucTest_get_label(ucTest *p);

uc_EXPORTED ucTest *ucTest_init(ucTest *p, const char *label, ucTestGroup **groups);

uc_EXPORTED ucTestErr ucTest_run(ucTest *p);

struct ucTest {
    const char *label;
    ucTestState state;

    ucTest_PrintFunc *print_func;
    void *print_state;

    ucTest_ExitFunc *exit_func;
    void *exit_state;

    ucTestGroup **groups;
};

#endif
