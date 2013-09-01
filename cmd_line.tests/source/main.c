#include <stdio.h>
#include "cl_common.h"
#include "cl_tests.h"
#include "cl_tests_p.h"

int main (int argc, const char * argv[]) {
    char str[50];
    cl_tests *p = cl_tests_get_instance();
    cl_tests_exit_func *exit = cl_tests_get_exit(p);
    cl_tests_print_func *print = cl_tests_get_print(p);

    CL_TESTS_ERR err = cl_tests_run(p);

    if (NULL != print) {
        if (err) {
            sprintf(str, "Test failed in group %d.\r\n", cl_tests_get_run_group_count(p));
            print(str);
        }
        else {
            sprintf(str, "%s", "All tests passed.\r\n");
            print(str);
        }
        sprintf(str, "Tests run: %d\r\n", cl_tests_get_run_count(p));
        print(str);
        sprintf(str, "Assertions made: %d\r\n", cl_tests_get_assertions_made(p));
        print(str);

        if (!cl_tests_get_unattended(p)) {
            if (NULL != exit) {
                print("Exit?");
                while (!exit()) {
                    continue;
                }
            }
        }
    }

    return err ? -1 : 0;
}
