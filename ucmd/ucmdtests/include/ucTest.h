#ifndef UC_TEST_H
#define UC_TEST_H

#include "ucBool.h"
#include "ucTestGroup.h"

typedef struct ucTest ucTest;

#define ucTest_FAIL() do { return -1; } while (0)

#define ucTest_ASSERT(TRUE) do { if (!(TRUE)) return -1; } while (0)

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
typedef ucBool (ucTest_ExitFunc)(void* state);

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
