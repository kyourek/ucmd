#include <stdio.h>
#include "cl_common.h"
#include "cl_tests.h"
#include "cl_tests_p.h"

int main (int argc, const char * argv[]) {
    return cl_tests_main(cl_tests_get_instance()) ? -1 : 0;
}
