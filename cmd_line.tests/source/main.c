#include <stdio.h>
#include "cl_common.h"
#include "cl_tests.h"
#include "cl_tests_p.h"

static void tests_print(const char *str) {
    printf("%s", str);
}

static CL_BOOL tests_exit(void) {
    getchar();
    return CL_TRUE;
}

int main (int argc, const char * argv[]) {
	cl_tests *tests = cl_tests_get_instance();
	cl_tests_set_print(tests, tests_print);
	cl_tests_set_exit(tests, tests_exit);
    return cl_tests_main(tests) ? -1 : 0;
}
