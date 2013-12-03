#ifndef UC_TEST_H
#define UC_TEST_H

#include "uc_bool.h"
#include "uc_test_group.h"

typedef struct uc_test uc_test;

#define UC_TEST_FAIL() do { return -1; } while (0)

#define UC_TEST_ASSERT(TRUE) do { if (!(TRUE)) return -1; } while (0)

/*
 * Summary:
 *   The type of functions used by a tests structure to print
 *   information.
 * Parameters:
 *   str: The information string to be printed.
 *   state: The tests structure's print state.
 */
typedef void (uc_test_print_func)(const char *str, void *state);

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
typedef uc_bool (uc_test_exit_func)(void* state);

/*
 * Summary:
 *   Sets the function used by the tests structure to print information.
 * Parameters:
 *   p: A pointer to the tests structure.
 *   value: The print function used to print information.
 */
UC_EXPORTED void uc_test_set_print_func(uc_test *p, uc_test_print_func *value);

/* Summary:
 *   Sets the state object that is given as a parameter to the print function.
 * Parameters:
 *   p: A pointer to the tests structure.
 *   value: The state object that is given as a parameter to the structure's print function.
 */
UC_EXPORTED void uc_test_set_print_state(uc_test *p, void *value);

/*
 * Summary:
 *   Sets the function used by the tests structure when exiting.
 * Parameters:
 *   p: A pointer to the tests structure.
 *   value: The function that is called before the test procedure exits.
 */
UC_EXPORTED void uc_test_set_exit_func(uc_test *p, uc_test_exit_func *value);

/*
 * Summary:
 *   Sets the state object that is given as a paramter to the tests structure's exit function.
 * Parameters:
 *   p: A pointer to the tests structure.
 *   value: The state object that is given as a parameter to the exit function.
 */
UC_EXPORTED void uc_test_set_exit_state(uc_test *p, void *value);

UC_EXPORTED uc_test_group **uc_test_get_groups(uc_test *p);

UC_EXPORTED uc_test_state *uc_test_get_state(uc_test *p);

UC_EXPORTED const char *uc_test_get_label(uc_test *p);

UC_EXPORTED uc_test *uc_test_init(uc_test *p, const char *label, uc_test_group **groups);

UC_EXPORTED uc_test_err uc_test_run(uc_test *p);

struct uc_test {
    const char *label;
    uc_test_state state;

    uc_test_print_func *print_func;
    void *print_state;
    
    uc_test_exit_func *exit_func;
    void *exit_state;

    uc_test_group **groups;
};

#endif
