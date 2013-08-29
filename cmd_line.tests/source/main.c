#include <stdio.h>
#include "cl_tests.h"

int main (int argc, const char * argv[]) {
    char *result = cl_run_all_tests();

    if (result) {
        printf("%s\n", result);
    }
    else {
        printf("All tests passed.\n");
    }
    printf("Tests run: %d\n", cl_tests_run);
    printf("Assertions made: %d\n", cl_assertions);

    printf("\nPress any key to exit.");
    getchar();

    return result ? -1 : 0;
}
