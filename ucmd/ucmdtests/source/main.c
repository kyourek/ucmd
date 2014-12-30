#include <stdio.h>
#include "uc_tests.h"

static void print_func(const char *str, void *state) {
    printf("%s", str);
}

static ucBool exit_func(void *state) {
    getchar();
    return ucBool_TRUE;
}

int main (int argc, const char * argv[]) {
    ucTest *test = uc_tests_get_test();
    ucTest_set_print_func(test, print_func);
    ucTest_set_exit_func(test, exit_func);
    return ucTest_run(test) ? -1 : 0;
}
