#include <stdio.h>
#include "cl_tests.h"

int main (int argc, const char * argv[]) {
    cl_tests_err result = cl_tests();

    if (result) {
        printf("%s\n", result);
    }
    else {
        printf("All tests passed.\n");
    }
    printf("Tests run: %d\n", cl_tests_run_count);
    printf("Assertions made: %d\n", cl_tests_assertions_made);

    printf("\nPress any key to exit.");
    getchar();

    return result ? -1 : 0;
}
