#include <stdio.h>
#include "uc_tests.h"

static void print_func(const char *str, void *state) {
    printf("%s", str);
}

static uc_bool exit_func(void *state) {
    getchar();
    return UC_TRUE;
}

int main (int argc, const char * argv[]) {
    uc_test *test = uc_tests_get_test();
    uc_test_set_print_func(test, print_func);
    uc_test_set_exit_func(test, exit_func);
    return uc_test_run(test) ? -1 : 0;
}
