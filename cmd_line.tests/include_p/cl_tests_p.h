#ifndef CL_TESTS_P_H
#define CL_TESTS_P_H

#include "cl_tests.h"

struct cl_tests {
    int run_count;
    int run_group_count;
    int assertions_made;
    int group_run_count;
    int group_assertions_made;
    CL_BOOL unattended;
    CL_TESTS_ERR err_returned;
    cl_tests_print_func *print;
    cl_tests_exit_func *exit;
};

#define CL_TESTS_FAIL() \
    do { \
        return -1; \
    } while (0) \

#define CL_TESTS_ASSERT(assertion) \
    do { \
        if (!(assertion)) { \
            return -1; \
        } \
        cl_tests_get_instance()->assertions_made++; \
        cl_tests_get_instance()->group_assertions_made++; \
    } while (0) \

#define CL_TESTS_RUN(test) \
    do { \
        if ((test())) { \
            return -1; \
        } \
        cl_tests_get_instance()->run_count++; \
        cl_tests_get_instance()->group_run_count++; \
    } while (0) \

#define CL_TESTS_RUN_GROUP(test_group) \
    do { \
        cl_tests_get_instance()->group_run_count = 0; \
        cl_tests_get_instance()->group_assertions_made = 0; \
        if ((test_group())) { \
            return -1; \
        } \
        cl_tests_get_instance()->run_group_count++; \
    } while (0) \

#endif