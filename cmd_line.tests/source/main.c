#include <stdio.h>
#include "cl_tests.h"

static void print_func(const char *str, void *state) {
    printf("%s", str);
}

static cl_bool exit_func(void *state) {
    getchar();
    return CL_TRUE;
}

int main (int argc, const char * argv[]) {
    cl_test *test = cl_tests_get_test();
    cl_test_set_print_func(test, print_func);
    cl_test_set_exit_func(test, exit_func);
    return cl_test_run(test) ? -1 : 0;
}
