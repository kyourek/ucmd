#include <stdio.h>
#include "ucmdtests.h"

static void print_func(const char *format, va_list va, void *state) {
    vprintf(format, va);
    printf("%s", "\n");
}

static void close_func(void *state) {
    getchar();
}

int main (int argc, const char * argv[]) {
    ucTestRunner *test_runner = ucTestRunner_instance;
    ucTestRunner_set_print(test_runner, print_func);
    ucTestRunner_set_close(test_runner, close_func);
    ucTestRunner_run(test_runner);
    return 0;
}
